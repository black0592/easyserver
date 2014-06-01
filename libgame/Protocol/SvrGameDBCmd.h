#ifndef __SVR_GAMEDB_CMD_H__
#define __SVR_GAMEDB_CMD_H__
#include "BaseCmd.h"
#include "BaseStruct.h"
#include "ServerDefine.h"

namespace GameDBCmd
{

#pragma pack(1)


	// 定义服务器消息结构基类
	struct stGameDBBaseCmd : public stBaseCmd
	{
		stGameDBBaseCmd(para_type para) 
			: stBaseCmd(GAMEDB_SERVICE, para)
		{

		}

	};


	//////////////////////////////////////////////////////////////////////////
	//             所有和GameDB服务器通讯的消息		开始
	//////////////////////////////////////////////////////////////////////////

	// 网关服务器和GameDB服务器之间的转发
	const para_type GATEWAY_FORWARD_CMD = 1;	// gateway -> gamedb, gamedb -> gateway
	struct stGatewayForwardCmd : public stGameDBBaseCmd
	{
		char account[ACCNAME_LEN+1];
		uint clientTaskId;				// task编号
		uint charId;					// 角色唯一ID号
		uint count;		
		byte data[0];

		stGatewayForwardCmd() : stGameDBBaseCmd(GATEWAY_FORWARD_CMD)
		{
			bzero(account, sizeof(account));
			clientTaskId = 0;
			charId = 0;
			count = 0;
		}

		uint getSize() { return sizeof(*this) + count * sizeof(data[0]); }
	};

	// 游戏服务器和GameDB服务器之间的转发
	const para_type GAME_FORWARD_CMD = 2;	// game -> gamedb, gamedb -> game
	struct stGameForwardCmd : public stGameDBBaseCmd
	{
		uint clientTaskId;				// task编号
		uint charId;					// 角色唯一ID号
		uint count;		
		byte data[0];

		stGameForwardCmd() : stGameDBBaseCmd(GAME_FORWARD_CMD)
		{
			clientTaskId = 0;
			charId = 0;
			count = 0;
		}

		uint getSize() { return sizeof(*this) + count * sizeof(data[0]); }
	};


	//////////////////////////////////////////////////////////////////////////
	//			所有和Game服务器通讯的消息		结束
	//////////////////////////////////////////////////////////////////////////



#pragma pack()

};	// namespace


#endif