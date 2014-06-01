#ifndef __LOGINCOMMAND_H__
#define __LOGINCOMMAND_H__

#include "BaseCmd.h"

#pragma pack(1)

namespace LoginCmd
{
	//enum CmdLogin
	//{
	//	LOGIN_GAME_CMD = 181,		// 登陆服和游戏服之间的消息
	//	LOGIN_GATEWAY_CMD = 183,	// 登陆服和网关服之间的消息
	//};


	//////////////////////////////////////////////////////////////////////////
	//			游戏服务器和登录服务器之间的消息		开始
	//////////////////////////////////////////////////////////////////////////

	/*
	// 游戏服务器请求注册到登陆服务器
	const byte REGISTER_GAMESERVER = 1;		// gameserver -> loginserver
	struct stRegisterGameServer : stBaseCmd
	{
		uint id;	// zonieid
		char name[50];
		ushort port;
		bool isNew;

		stRegisterGameServer() : stBaseCmd(LOGIN_GAME_CMD, REGISTER_GAMESERVER)
		{
			id = 0;
			port = 0;
			bzero(name,sizeof(name));
			isNew = false;
		}
	};

	// 登陆服务器返回验证结果
	const byte REGISTER_GAMESERVER_RESULT = 2;		// loginserver -> gameserver
	struct stRegisterGameServerResult : stBaseCmd
	{
		byte result;	// 0 成功，1 失败

		stRegisterGameServerResult() : stBaseCmd(LOGIN_GAME_CMD, REGISTER_GAMESERVER_RESULT)
		{
			result = 0;
		}
	};
	*/


	/*
	// 服务器发送当前状态给登陆服务器
	const byte UPDATE_GAMESERVER_INFO = 3;		// gameserver -> loginserver
	struct stUpdateGameServerInfo : stBaseCmd
	{
		uint serverId;
		uint onlineNum;
		uint maxNum;
		byte state;	// enum ServerState

		stUpdateGameServerInfo() : stBaseCmd(LOGIN_GAME_CMD, UPDATE_GAMESERVER_INFO)
		{
			serverId = 0;
			onlineNum = 0;
			maxNum = 0;
			state = 0;
		}
	};
	*/


	/*
	// 游戏服务器请求用户游戏结束时间
	const byte GAMESERVER_USERENDTIME = 4;		// gameserver -> loginserver
	struct stGameServerUserEndTime : stBaseCmd
	{
		uint charid;
		char account[ACCNAME_LEN+1];
		time_t endtime;

		stGameServerUserEndTime() : stBaseCmd(LOGIN_GAME_CMD, GAMESERVER_USERENDTIME)
		{
			charid = 0;
			endtime = 0;
		}
	};
	*/


	/*
	// 向登陆服务器请求账号的游戏点数
	const byte GAMESERVER_GET_USER_CASHINFO = 5;		// gameserver -> loginserver
	struct stGameServerGetUserCashInfo : stBaseCmd
	{
		uint charid;
		char account[ACCNAME_LEN+1];
		uint cashpoint;	// 游戏点数
		uint hongli;	// 积分点

		stGameServerGetUserCashInfo() : stBaseCmd(LOGIN_GAME_CMD, GAMESERVER_GET_USER_CASHINFO)
		{
			bzero(account,sizeof(account));
			charid = 0;
			cashpoint = 0;
			hongli = 0;
		}
	};
	*/

	/*
	// 向登陆服务器请求更新账号的游戏点数
	// gameserver -> loginserver   填入的是需要扣除的点数和积分
	// loginserver -> gameserver   返回的剩余的点数和积分
	const byte GAMESERVER_UPDATE_USER_CASHINFO = 6;		// gameserver -> loginserver
	struct stGameServerUpdateUserCashInfo : stBaseCmd
	{
		uint charid;
		char account[ACCNAME_LEN+1];
		uint cashpoint;	// 游戏点数(需要扣除的数值)
		uint hongli;	// 积分点(需要扣除的数值)
		byte type;		// 区分功能类型(0 - 商城购买, 1 - 点数兑换金币)
		byte result;	// 0 - 成功, 其他 - 错误编号

		stGameServerUpdateUserCashInfo() : stBaseCmd(LOGIN_GAME_CMD, GAMESERVER_UPDATE_USER_CASHINFO)
		{
			bzero(account,sizeof(account));
			charid = 0;
			cashpoint = 0;
			hongli = 0;
			type = 0;
			result = 0;
		}
	};
	*/


/*
	// 请求封号
	const byte GAMESERVER_STOP_ACCOUNT = 7;		// gameserver -> loginserver
	struct stGameServerStopAccount : stBaseCmd
	{
		char charname[CHARNAME_LEN+1];
		char account[ACCNAME_LEN+1];

		stGameServerStopAccount() : stBaseCmd(LOGIN_GAME_CMD, GAMESERVER_STOP_ACCOUNT)
		{
			bzero(charname,sizeof(charname));
			bzero(account,sizeof(account));
		}
	};
	*/


/*
	// 游戏服务器ping登录服务器
	const byte GAMESERVER_PING = 100;		// gameserver -> loginserver
	struct stGameServerPing : stBaseCmd
	{
		stGameServerPing() : stBaseCmd(LOGIN_GAME_CMD, GAMESERVER_PING)
		{
		}
	};
	*/


	//////////////////////////////////////////////////////////////////////////
	//			游戏服务器和登录服务器之间的消息		结束
	//////////////////////////////////////////////////////////////////////////




	//////////////////////////////////////////////////////////////////////////
	//			网关服务器和登录服务器之间的消息		开始
	//////////////////////////////////////////////////////////////////////////

/*
	// 网关服务器请求注册到登陆服务器
	const byte REGISTER_GATEWAYSERVER = 1;		// gatewayserver -> loginserver
	struct stRegisterGatewayServer : stBaseCmd
	{
		uint zoneid;
		uint id;
		char name[50];
		char ip[128];
		ushort port;
		bool isNew;

		stRegisterGatewayServer() : stBaseCmd(LOGIN_GATEWAY_CMD, REGISTER_GATEWAYSERVER)
		{
			zoneid = 0;
			id = 0;
			port = 0;
			bzero(name,sizeof(name));
			bzero(ip,sizeof(ip));
			isNew = false;
		}
	};

	// 登陆服务器返回验证结果
	const byte REGISTER_GATEWAYSERVER_RESULT = 2;		// loginserver -> gatewayserver
	struct stRegisterGatewayServerResult : stBaseCmd
	{
		byte result;	// 0 成功，1 失败

		stRegisterGatewayServerResult() : stBaseCmd(LOGIN_GATEWAY_CMD, REGISTER_GATEWAYSERVER_RESULT)
		{
			result = 0;
		}
	};
	*/

/*
	// 登陆服务器同步玩家的登陆密钥到网关服务器
	const byte REG_USER_TO_GATEWAY = 3;		// loginserver -> gatewayserver, gatewayserver -> loginserver
	struct stRegUserToGateway : stBaseCmd
	{
		uint zoneId;						// 大区ID
		uint gatewayId;						// 网关
		char account[ACCNAME_LEN+1];	// 账号名
		char key[13];						// 密钥
		byte result;						// 返回值 0 - 成功, 1 - 失败

		stRegUserToGateway() : stBaseCmd(LOGIN_GATEWAY_CMD, REG_USER_TO_GATEWAY)
		{
			zoneId = 0;
			gatewayId = 0;
			bzero(account,sizeof(account));
			bzero(key,sizeof(key));
			result = 0;
		}
	};
	*/

	/*
	// 网关通知登陆服务器注销用户	gateway -> login
	const byte UNREG_USER = 4;
	struct stUnregUser : stBaseCmd 
	{
		char account[ACCNAME_LEN+1];	// 账号名
		char key[13];						// 密钥

		stUnregUser() : stBaseCmd(LOGIN_GATEWAY_CMD, UNREG_USER)
		{
			bzero(account,sizeof(account));
			bzero(key,sizeof(key));
		}
	};


	// 登陆服务器通知网关注销用户	login -> gateway
	const byte UNREG_GATEWAYUSER = 5;
	struct stUnregGatewayUser : stBaseCmd 
	{
		char account[ACCNAME_LEN+1];	// 账号名
		char key[13];						// 密钥

		stUnregGatewayUser() : stBaseCmd(LOGIN_GATEWAY_CMD, UNREG_GATEWAYUSER)
		{
			bzero(account,sizeof(account));
			bzero(key,sizeof(key));
		}
	};


	// 游戏服务器ping登录服务器
	const byte GATEWAYSERVER_PING = 100;		// gatewayserver -> loginserver
	struct stGatewayServerPing : stBaseCmd
	{
		stGatewayServerPing() : stBaseCmd(LOGIN_GATEWAY_CMD, GATEWAYSERVER_PING)
		{
		}
	};
	*/

	//////////////////////////////////////////////////////////////////////////
	//			网关服务器和登录服务器之间的消息		结束
	//////////////////////////////////////////////////////////////////////////


};

#pragma pack()

#endif