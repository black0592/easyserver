#ifndef __SVR_GAME_CMD_H__
#define __SVR_GAME_CMD_H__
#include "BaseCmd.h"
#include "BaseStruct.h"
#include "ServerDefine.h"

namespace GameCmd
{

#pragma pack(1)


	// 定义服务器消息结构基类
	struct stGameBaseCmd : public stBaseCmd
	{
		stGameBaseCmd(para_type para) 
			: stBaseCmd(GAME_SERVICE, para)
		{

		}

	};


	//////////////////////////////////////////////////////////////////////////
	//             所有和Game服务器通讯的消息		开始
	//////////////////////////////////////////////////////////////////////////

	// 网关和游戏服务器之间的转发
	const para_type GATEWAY_FORWARD_CMD = 1;	// gateway -> game, game -> gateway
	struct stGatewayForwardCmd : public stGameBaseCmd
	{
		uint clientTaskId;				// task编号
		uint charId;					// 角色唯一ID号
		uint count;		
		byte data[0];

		stGatewayForwardCmd() : stGameBaseCmd(GATEWAY_FORWARD_CMD)
		{
			clientTaskId = 0;
			charId = 0;
			count = 0;
		}

		uint getSize() { return sizeof(*this) + count * sizeof(data[0]); }
	};


	// 通知网关进行广播的消息
	struct stTaskInfo 
	{
		uint taskId;
		uint charId;
	};

	const para_type GATEWAY_BROADCAST_CMD = 2;	// game -> gateway
	struct stGatewayBroadcastCmd : public stGameBaseCmd
	{
		uint taskCount;	// 客户端task数量
		uint protoSize;	// 转发消息的长度
		byte data[0];

		stGatewayBroadcastCmd() : stGameBaseCmd(GATEWAY_BROADCAST_CMD)
		{
			taskCount = 0;
			protoSize = 0;
		}

		uint getTaskIdSize() { return taskCount * sizeof(stTaskInfo); }
		uint getProtoMsgSize() { return protoSize * sizeof(byte); }

		uint getSize() { return sizeof(*this) + getTaskIdSize() + getProtoMsgSize(); }
	};

	//////////////////////////////////////////////////////////////////////////
	//			所有和Game服务器通讯的消息		结束
	//////////////////////////////////////////////////////////////////////////



#pragma pack()

};	// namespace


#endif