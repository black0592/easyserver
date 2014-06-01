#ifndef __SIEGEWAR_CMD_H__
#define __SIEGEWAR_CMD_H__
#include "BaseCmd.h"
#include "BaseStruct.h"
#include "Define.h"

namespace Protocol
{

#pragma pack(1)


	// �˹��ܿ�Ľṹ����
	struct stSiegeWarBaseCmd : public stBaseCmd
	{
		stSiegeWarBaseCmd(para_type para) : stBaseCmd(USERCMD_SIEGEWAR, para)
		{
		}
	};


	//////////////////////////////////////////////////////////////////////////


	// ��ʼ����ս
	const para_type GAME_CMD_BEGIN_SIEGEWAR = 170;			// s -> c
	struct stBeginSiegeWar : stSiegeWarBaseCmd
	{
		stBeginSiegeWar() : stSiegeWarBaseCmd(GAME_CMD_BEGIN_SIEGEWAR)
		{
		}
	};

	// ���빥��ս
	const para_type GAME_CMD_JOIN_SIEGEWAR = 171;			// c->s, s -> c
	struct stJoinSiegeWar : stSiegeWarBaseCmd
	{
		char guildName[CHARNAME_LEN+1];
		stJoinSiegeWar() : stSiegeWarBaseCmd(GAME_CMD_JOIN_SIEGEWAR)
		{
			bzero(guildName,sizeof(guildName));
		}
	};

	// ����սʣ��ʱ��
	const para_type GAME_CMD_SIEGEWAR_REMAINTIME = 173;			// s -> c
	struct stSiegeWarRemainTime : stSiegeWarBaseCmd
	{
		byte remainTime;
		stSiegeWarRemainTime() : stSiegeWarBaseCmd(GAME_CMD_SIEGEWAR_REMAINTIME)
		{
			remainTime = 0;
		}
	};

	// ��������ս
	const para_type GAME_CMD_END_SIEGEWAR = 174;			// s -> c
	struct stEndSiegeWar : stSiegeWarBaseCmd
	{
		byte winner;
		char guildName[CHARNAME_LEN+1];
		stEndSiegeWar() : stSiegeWarBaseCmd(GAME_CMD_END_SIEGEWAR)
		{
			winner = 0;
			bzero(guildName,sizeof(guildName));
		}
	};


	// ����Կ��
	const para_type GAME_CMD_PICKUP_KEY = 176;			// c -> s, s -> c
	struct stPickupKey : stSiegeWarBaseCmd
	{
		byte mapId;
		ushort x;
		ushort y;
		ushort itemId;
		byte result;

		stPickupKey() : stSiegeWarBaseCmd(GAME_CMD_PICKUP_KEY)
		{
			mapId = 0;
			x = 0;
			y = 0;
			itemId = 0;
			result = 0;
		}
	};


	// ����˰��
	const para_type GAME_CMD_SET_TAXRATE = 178;			// c -> s, s -> c
	struct stSetTaxRate : stSiegeWarBaseCmd
	{
		byte taxRate;
		byte result;

		stSetTaxRate() : stSiegeWarBaseCmd(GAME_CMD_SET_TAXRATE)
		{
			taxRate = 0;
			result = 0;
		}
	};

	// ��ѯ������
	const para_type GAME_CMD_GUILDSAFEMONEY = 180;			// c -> s, s -> c
	struct stGuildSafeMoney : stSiegeWarBaseCmd
	{
		uint money[3];	//���˹���,����˹���,����˹���
		byte result;

		stGuildSafeMoney() : stSiegeWarBaseCmd(GAME_CMD_GUILDSAFEMONEY)
		{
			bzero(money,sizeof(money));
			result = 0;
		}
	};

	// ���ҵ�����
	const para_type GAME_CMD_GUILDDEPOSITMONEY = 182;			// c -> s, s -> c
	struct stGuildDepositMoney : stSiegeWarBaseCmd
	{
		uint money;
		byte result;

		stGuildDepositMoney() : stSiegeWarBaseCmd(GAME_CMD_GUILDDEPOSITMONEY)
		{
			money = 0;
			result = 0;
		}
	};

	// �ӹ���ȡ���
	const para_type GAME_CMD_GUILDWIDTHDRAWMONEY = 186;			// c -> s, s -> c
	struct stGuildWidthdrawMoney : stSiegeWarBaseCmd
	{
		uint money;
		byte result;

		stGuildWidthdrawMoney() : stSiegeWarBaseCmd(GAME_CMD_GUILDWIDTHDRAWMONEY)
		{
			money = 0;
			result = 0;
		}
	};





#pragma pack()

};	// namespace


#endif