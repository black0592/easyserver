#ifndef __LOGINCOMMAND_H__
#define __LOGINCOMMAND_H__

#include "BaseCmd.h"

#pragma pack(1)

namespace LoginCmd
{
	//enum CmdLogin
	//{
	//	LOGIN_GAME_CMD = 181,		// ��½������Ϸ��֮�����Ϣ
	//	LOGIN_GATEWAY_CMD = 183,	// ��½�������ط�֮�����Ϣ
	//};


	//////////////////////////////////////////////////////////////////////////
	//			��Ϸ�������͵�¼������֮�����Ϣ		��ʼ
	//////////////////////////////////////////////////////////////////////////

	/*
	// ��Ϸ����������ע�ᵽ��½������
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

	// ��½������������֤���
	const byte REGISTER_GAMESERVER_RESULT = 2;		// loginserver -> gameserver
	struct stRegisterGameServerResult : stBaseCmd
	{
		byte result;	// 0 �ɹ���1 ʧ��

		stRegisterGameServerResult() : stBaseCmd(LOGIN_GAME_CMD, REGISTER_GAMESERVER_RESULT)
		{
			result = 0;
		}
	};
	*/


	/*
	// ���������͵�ǰ״̬����½������
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
	// ��Ϸ�����������û���Ϸ����ʱ��
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
	// ���½�����������˺ŵ���Ϸ����
	const byte GAMESERVER_GET_USER_CASHINFO = 5;		// gameserver -> loginserver
	struct stGameServerGetUserCashInfo : stBaseCmd
	{
		uint charid;
		char account[ACCNAME_LEN+1];
		uint cashpoint;	// ��Ϸ����
		uint hongli;	// ���ֵ�

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
	// ���½��������������˺ŵ���Ϸ����
	// gameserver -> loginserver   ���������Ҫ�۳��ĵ����ͻ���
	// loginserver -> gameserver   ���ص�ʣ��ĵ����ͻ���
	const byte GAMESERVER_UPDATE_USER_CASHINFO = 6;		// gameserver -> loginserver
	struct stGameServerUpdateUserCashInfo : stBaseCmd
	{
		uint charid;
		char account[ACCNAME_LEN+1];
		uint cashpoint;	// ��Ϸ����(��Ҫ�۳�����ֵ)
		uint hongli;	// ���ֵ�(��Ҫ�۳�����ֵ)
		byte type;		// ���ֹ�������(0 - �̳ǹ���, 1 - �����һ����)
		byte result;	// 0 - �ɹ�, ���� - ������

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
	// ������
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
	// ��Ϸ������ping��¼������
	const byte GAMESERVER_PING = 100;		// gameserver -> loginserver
	struct stGameServerPing : stBaseCmd
	{
		stGameServerPing() : stBaseCmd(LOGIN_GAME_CMD, GAMESERVER_PING)
		{
		}
	};
	*/


	//////////////////////////////////////////////////////////////////////////
	//			��Ϸ�������͵�¼������֮�����Ϣ		����
	//////////////////////////////////////////////////////////////////////////




	//////////////////////////////////////////////////////////////////////////
	//			���ط������͵�¼������֮�����Ϣ		��ʼ
	//////////////////////////////////////////////////////////////////////////

/*
	// ���ط���������ע�ᵽ��½������
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

	// ��½������������֤���
	const byte REGISTER_GATEWAYSERVER_RESULT = 2;		// loginserver -> gatewayserver
	struct stRegisterGatewayServerResult : stBaseCmd
	{
		byte result;	// 0 �ɹ���1 ʧ��

		stRegisterGatewayServerResult() : stBaseCmd(LOGIN_GATEWAY_CMD, REGISTER_GATEWAYSERVER_RESULT)
		{
			result = 0;
		}
	};
	*/

/*
	// ��½������ͬ����ҵĵ�½��Կ�����ط�����
	const byte REG_USER_TO_GATEWAY = 3;		// loginserver -> gatewayserver, gatewayserver -> loginserver
	struct stRegUserToGateway : stBaseCmd
	{
		uint zoneId;						// ����ID
		uint gatewayId;						// ����
		char account[ACCNAME_LEN+1];	// �˺���
		char key[13];						// ��Կ
		byte result;						// ����ֵ 0 - �ɹ�, 1 - ʧ��

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
	// ����֪ͨ��½������ע���û�	gateway -> login
	const byte UNREG_USER = 4;
	struct stUnregUser : stBaseCmd 
	{
		char account[ACCNAME_LEN+1];	// �˺���
		char key[13];						// ��Կ

		stUnregUser() : stBaseCmd(LOGIN_GATEWAY_CMD, UNREG_USER)
		{
			bzero(account,sizeof(account));
			bzero(key,sizeof(key));
		}
	};


	// ��½������֪ͨ����ע���û�	login -> gateway
	const byte UNREG_GATEWAYUSER = 5;
	struct stUnregGatewayUser : stBaseCmd 
	{
		char account[ACCNAME_LEN+1];	// �˺���
		char key[13];						// ��Կ

		stUnregGatewayUser() : stBaseCmd(LOGIN_GATEWAY_CMD, UNREG_GATEWAYUSER)
		{
			bzero(account,sizeof(account));
			bzero(key,sizeof(key));
		}
	};


	// ��Ϸ������ping��¼������
	const byte GATEWAYSERVER_PING = 100;		// gatewayserver -> loginserver
	struct stGatewayServerPing : stBaseCmd
	{
		stGatewayServerPing() : stBaseCmd(LOGIN_GATEWAY_CMD, GATEWAYSERVER_PING)
		{
		}
	};
	*/

	//////////////////////////////////////////////////////////////////////////
	//			���ط������͵�¼������֮�����Ϣ		����
	//////////////////////////////////////////////////////////////////////////


};

#pragma pack()

#endif