#ifndef __TRADE_CMD_H__
#define __TRADE_CMD_H__
#include "BaseCmd.h"
#include "BaseStruct.h"
#include "Define.h"

namespace Protocol
{

#pragma pack(1)


	// 此功能块的结构基类
	struct stTradeBaseCmd : public stBaseCmd
	{
		stTradeBaseCmd(para_type para) : stBaseCmd(USERCMD_TRADE, para)
		{
		}
	};


	//////////////////////////////////////////////////////////////////////////


	// 玩家请求交易(C->S, S->C)
	const para_type GAME_CMD_REQUEST_TRADE = 66;
	struct stRequestTrade : stTradeBaseCmd 
	{
		uint id;
		ushort targetId;
		byte result;

		stRequestTrade() : stTradeBaseCmd(GAME_CMD_REQUEST_TRADE)
		{
			id = 0;
			targetId = 0;
			result = 0;
		}
	};

	const para_type GAME_CMD_REQUEST_TRADE_RESULT = 67;
	struct stRequestTradeResult : stTradeBaseCmd 
	{
		uint id;
		ushort targetId;
		byte result;

		stRequestTradeResult() : stTradeBaseCmd(GAME_CMD_REQUEST_TRADE_RESULT)
		{
			id = 0;
			targetId = 0;
			result = 0;
		}
	};

	// 取消交易(C->S, S->C)
	const para_type GAME_CMD_CANCEL_TRADE = 68;
	struct stCancelTrade : stTradeBaseCmd 
	{
		ushort targetId;
		byte result;

		stCancelTrade() : stTradeBaseCmd(GAME_CMD_CANCEL_TRADE)
		{
			targetId = 0;
			result = 0;
		}
	};

	const para_type GAME_CMD_CANCEL_TRADE_RESULT = 69;
	struct stCancelTradeResult : stTradeBaseCmd 
	{
		ushort targetId;
		byte result;

		stCancelTradeResult() : stTradeBaseCmd(GAME_CMD_CANCEL_TRADE_RESULT)
		{
			targetId = 0;
			result = 0;
		}
	};

	// 交易确认(C->S, S->C)
	const para_type GAME_CMD_TRADE_OK = 70;
	struct stTradeOK : stTradeBaseCmd 
	{
		uint id;
		ushort targetId;
		byte result;

		stTradeOK() : stTradeBaseCmd(GAME_CMD_TRADE_OK)
		{
			id = 0;
			targetId = 0;
			result = 0;
		}
	};

	const para_type GAME_CMD_TRADE_OK_RESULT = 71;
	struct stTradeOKResult : stTradeBaseCmd 
	{
		uint id;
		ushort targetId;
		byte result;

		stTradeOKResult() : stTradeBaseCmd(GAME_CMD_TRADE_OK_RESULT)
		{
			id = 0;
			targetId = 0;
			result = 0;
		}
	};


	// 放入物品
	const para_type GAME_CMD_TRADE_PUT_ITEM = 72;
	struct stTradePutItem : stTradeBaseCmd 
	{
		uint id;
		ushort targetId;
		stPkgPos pos;
		ItemData itemData;
		ushort index;
		byte result;

		stTradePutItem() : stTradeBaseCmd(GAME_CMD_TRADE_PUT_ITEM)
		{
			id = 0;
			targetId = 0;
			index = 0;
			result = 0;
		}
	};

	const para_type GAME_CMD_TRADE_PUT_ITEM_RESULT = 73;
	struct stTradePutItemResult : stTradeBaseCmd 
	{
		uint id;
		ushort targetId;
		stPkgPos pos;
		ItemData itemData;
		ushort index;
		byte result;

		stTradePutItemResult() : stTradeBaseCmd(GAME_CMD_TRADE_PUT_ITEM_RESULT)
		{
			id = 0;
			targetId = 0;
			index = 0;
			result = 0;
		}
	};


	// 取消放入物品
	const para_type GAME_CMD_TRADE_PUT_CANCEL = 74;
	struct stTradePutCancel : stTradeBaseCmd 
	{
		ushort targetId;
		stPkgPos pos;
		ItemData itemData;
		ushort index;
		byte result;

		stTradePutCancel() : stTradeBaseCmd(GAME_CMD_TRADE_PUT_CANCEL)
		{
			targetId = 0;
			index = 0;
			result = 0;
		}
	};

	const para_type GAME_CMD_TRADE_PUT_CANCEL_RESULT = 75;
	struct stTradePutCancelResult : stTradeBaseCmd 
	{
		ushort targetId;
		stPkgPos pos;
		ItemData itemData;
		ushort index;
		byte result;

		stTradePutCancelResult() : stTradeBaseCmd(GAME_CMD_TRADE_PUT_CANCEL_RESULT)
		{
			targetId = 0;
			index = 0;
			result = 0;
		}
	};


	// 交易栏放入金币
	const para_type GAME_CMD_TRADE_PUT_MONEY = 76;
	struct stTradePutMoney : stTradeBaseCmd 
	{
		uint id;
		ushort targetId;
		uint money;
		byte result;

		stTradePutMoney() : stTradeBaseCmd(GAME_CMD_TRADE_PUT_MONEY)
		{
			id = 0;
			targetId = 0;
			money = 0;
			result = 0;
		}
	};
	const para_type GAME_CMD_TRADE_PUT_MONEY_RESULT = 77;
	struct stTradePutMoneyResult : stTradeBaseCmd 
	{
		uint id;
		ushort targetId;
		uint money;
		byte result;

		stTradePutMoneyResult() : stTradeBaseCmd(GAME_CMD_TRADE_PUT_MONEY_RESULT)
		{
			id = 0;
			targetId = 0;
			money = 0;
			result = 0;
		}
	};


	// 交易完成
	const para_type GAME_CMD_TRADE_END = 146;
	struct stTradeEnd : stTradeBaseCmd 
	{
		ushort targetId;
		uint money1;
		uint money2;
		stAddItemData addItem1[3];
		stAddItemData addItem2[3];
		stRemoveItemData removeItem1[3];
		stRemoveItemData removeItem2[3];
		byte result;

		stTradeEnd() : stTradeBaseCmd(GAME_CMD_TRADE_END)
		{
			targetId = 0;
			money1 = money2 = 0;
			result = 0;
		}
	};

	const para_type GAME_CMD_TRADE_END_RESULT = 147;
	struct stTradeEndResult : stTradeBaseCmd 
	{
		ushort targetId;
		uint money1;
		uint money2;
		stAddItemData addItem1[3];
		stAddItemData addItem2[3];
		stRemoveItemData removeItem1[3];
		stRemoveItemData removeItem2[3];
		byte result;

		stTradeEndResult() : stTradeBaseCmd(GAME_CMD_TRADE_END_RESULT)
		{
			targetId = 0;
			money1 = money2 = 0;
			result = 0;
		}
	};





#pragma pack()

};	// namespace


#endif