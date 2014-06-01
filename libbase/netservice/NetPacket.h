#ifndef __NET_PACKET_H__
#define __NET_PACKET_H__
#include "BaseTypes.h"
#include "BaseDefine.h"
#include <string.h>

namespace easygame {

//! ��Ϣ����������С

#pragma pack(1)
	// һ�����������ӦHEADER_SIZE + cmd
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

	// ���ջ���
	struct stRecvBuff
	{
		stRecvBuff()
		{
			//memset(packet.cmd,0,sizeof(packet.cmd));
			readyRecvSize();
		}

		// ��������֤�ɹ����ٷ����ڴ棬��Ȼ���ױ��Ƿ�����
		void initBuffer()
		{
			packet.initCmdBuffer();
		}

		// ���õ�ʱ����Ҫ�ͷ�����ڴ�
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
		//! ÿ����Ϣ�������ηֱ��ȡͷ���Ͱ���
		byte type;	// 1 - ����size, 2 - ����cmd

		// ��ǰ���ܵ��Ĵ�С
		uint recvedCmdSize;

		// ���ݻ�����
		stNetPacket packet;
	};


	/*
	// ���������Ϣָ��Ľṹ
	#define CMD_USER	100			// 100���µĶ���ϵͳ�ײ�ʹ�õ�����Ϣ��
	typedef byte cmd_type;
	typedef byte para_type;

	const cmd_type		CMD_NULL = 0;	//��ָ����
	const para_type		PARA_NULL = 0;	//�ղ���ָ����
	struct stBaseCmd 
	{
		cmd_type	cmd;	// ����Ϣ��
		para_type	para;	// ����Ϣ��

		stBaseCmd(cmd_type _cmd, para_type _para)
			:cmd(_cmd), para(_para)
		{

		}
	};
	*/

#pragma pack()


}	// namespace


#endif