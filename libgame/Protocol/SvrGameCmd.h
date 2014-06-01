#ifndef __SVR_GAME_CMD_H__
#define __SVR_GAME_CMD_H__
#include "BaseCmd.h"
#include "BaseStruct.h"
#include "ServerDefine.h"

namespace GameCmd
{

#pragma pack(1)


	// �����������Ϣ�ṹ����
	struct stGameBaseCmd : public stBaseCmd
	{
		stGameBaseCmd(para_type para) 
			: stBaseCmd(GAME_SERVICE, para)
		{

		}

	};


	//////////////////////////////////////////////////////////////////////////
	//             ���к�Game������ͨѶ����Ϣ		��ʼ
	//////////////////////////////////////////////////////////////////////////

	// ���غ���Ϸ������֮���ת��
	const para_type GATEWAY_FORWARD_CMD = 1;	// gateway -> game, game -> gateway
	struct stGatewayForwardCmd : public stGameBaseCmd
	{
		uint clientTaskId;				// task���
		uint charId;					// ��ɫΨһID��
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


	// ֪ͨ���ؽ��й㲥����Ϣ
	struct stTaskInfo 
	{
		uint taskId;
		uint charId;
	};

	const para_type GATEWAY_BROADCAST_CMD = 2;	// game -> gateway
	struct stGatewayBroadcastCmd : public stGameBaseCmd
	{
		uint taskCount;	// �ͻ���task����
		uint protoSize;	// ת����Ϣ�ĳ���
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
	//			���к�Game������ͨѶ����Ϣ		����
	//////////////////////////////////////////////////////////////////////////



#pragma pack()

};	// namespace


#endif