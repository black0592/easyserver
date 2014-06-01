#ifndef __SVR_GAMEDB_CMD_H__
#define __SVR_GAMEDB_CMD_H__
#include "BaseCmd.h"
#include "BaseStruct.h"
#include "ServerDefine.h"

namespace GameDBCmd
{

#pragma pack(1)


	// �����������Ϣ�ṹ����
	struct stGameDBBaseCmd : public stBaseCmd
	{
		stGameDBBaseCmd(para_type para) 
			: stBaseCmd(GAMEDB_SERVICE, para)
		{

		}

	};


	//////////////////////////////////////////////////////////////////////////
	//             ���к�GameDB������ͨѶ����Ϣ		��ʼ
	//////////////////////////////////////////////////////////////////////////

	// ���ط�������GameDB������֮���ת��
	const para_type GATEWAY_FORWARD_CMD = 1;	// gateway -> gamedb, gamedb -> gateway
	struct stGatewayForwardCmd : public stGameDBBaseCmd
	{
		char account[ACCNAME_LEN+1];
		uint clientTaskId;				// task���
		uint charId;					// ��ɫΨһID��
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

	// ��Ϸ��������GameDB������֮���ת��
	const para_type GAME_FORWARD_CMD = 2;	// game -> gamedb, gamedb -> game
	struct stGameForwardCmd : public stGameDBBaseCmd
	{
		uint clientTaskId;				// task���
		uint charId;					// ��ɫΨһID��
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
	//			���к�Game������ͨѶ����Ϣ		����
	//////////////////////////////////////////////////////////////////////////



#pragma pack()

};	// namespace


#endif