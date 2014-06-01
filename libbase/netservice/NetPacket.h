#ifndef __NET_PACKET_H__
#define __NET_PACKET_H__
#include "BaseTypes.h"
#include "BaseDefine.h"
#include <string.h>

namespace easygame {

//! 消息包缓冲区大小

#pragma pack(1)
	// 一个网络包，对应HEADER_SIZE + cmd
	struct stNetPacket
	{
		ushort size;
		char* cmd;

		stNetPacket()
		{
			size = 0;
			cmd = NULL;
		}

		void initCmdBuffer()
		{
			cmd = new char[MAX_USER_CMD_SIZE];
		}

		void releaseCmdBuff()
		{
			delete[] cmd;
			cmd = NULL;
		}
	};

	// 接收缓存
	struct stRecvBuff
	{
		stRecvBuff()
		{
			//memset(packet.cmd,0,sizeof(packet.cmd));
			readyRecvSize();
		}

		// 在连接验证成功后，再分配内存，不然容易被非法攻击
		void initBuffer()
		{
			packet.initCmdBuffer();
		}

		// 不用的时候，需要释放这块内存
		void releaseBuffer()
		{
			packet.releaseCmdBuff();
		}

		void readyRecvSize()
		{
			type = 1;
			recvedCmdSize = 0;
		}

		void readyRecvCmd()
		{
			type = 2;
			recvedCmdSize = 0;
		}

		bool isRecvSizeType()
		{
			return type == 1;
		}

		bool isRecvCmdType()
		{
			return type == 2;
		}

	public:
		//! 每个消息包分两次分别读取头部和包体
		byte type;	// 1 - 接收size, 2 - 接收cmd

		// 当前接受到的大小
		uint recvedCmdSize;

		// 数据缓冲区
		stNetPacket packet;
	};


	/*
	// 最基础的消息指令的结构
	#define CMD_USER	100			// 100以下的都是系统底层使用的主消息号
	typedef byte cmd_type;
	typedef byte para_type;

	const cmd_type		CMD_NULL = 0;	//空指令编号
	const para_type		PARA_NULL = 0;	//空参数指令编号
	struct stBaseCmd 
	{
		cmd_type	cmd;	// 主消息号
		para_type	para;	// 子消息号

		stBaseCmd(cmd_type _cmd, para_type _para)
			:cmd(_cmd), para(_para)
		{

		}
	};
	*/

#pragma pack()


}	// namespace


#endif