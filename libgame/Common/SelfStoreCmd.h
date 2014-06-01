#ifndef __SELFSTORE_CMD_H__
#define __SELFSTORE_CMD_H__
#include "BaseCmd.h"
#include "BaseStruct.h"
#include "Define.h"
#include "CharData.h"

namespace Protocol
{

#pragma pack(1)


	// 此功能块的结构基类
	struct stSelfStoreBaseCmd : public stBaseCmd
	{
		stSelfStoreBaseCmd(para_type para) : stBaseCmd(USERCMD_SELFSTORE, para)
		{
		}
	};


	//////////////////////////////////////////////////////////////////////////


	
	// 摆摊开始
	const para_type MAX_STOREITEM_NUM = 8;
	const para_type GAME_CMD_SELF_STORE_OPEN = 240;
	struct stSelfStoreOpen : stSelfStoreBaseCmd
	{
		stItemEx item[MAX_STOREITEM_NUM];
		uint itemPrice[MAX_STOREITEM_NUM];
		byte result;

		stSelfStoreOpen() : stSelfStoreBaseCmd(GAME_CMD_SELF_STORE_OPEN)
		{
			result = 0;
		}
	};

	// 摊位状态
	const para_type GAME_CMD_SELF_STORE_STATE = 241;
	struct stSelfStoreState : stSelfStoreBaseCmd
	{
		byte state;
		stSelfStoreState() : stSelfStoreBaseCmd(GAME_CMD_SELF_STORE_STATE)
		{
			state = 0;
		}
	};

	// 获取分类
	const para_type GAME_CMD_SELF_STORE_REQUEST_CATAGORY = 242;
	struct stSelfStoreRequestCatagory : stSelfStoreBaseCmd
	{
		ushort targetId;
		byte result;
		stSelfStoreRequestCatagory() : stSelfStoreBaseCmd(GAME_CMD_SELF_STORE_REQUEST_CATAGORY)
		{
			targetId = 0;
			result = 0;
		}
	};

	// 购买物品
	const para_type GAME_CMD_SELF_STORE_BUYITEM = 243;
	struct stSelfStoreBuyItem : stSelfStoreBaseCmd
	{
		ushort targetId;
		byte index;
		stItemEx item;
		uint price;
		byte result;
		stSelfStoreBuyItem() : stSelfStoreBaseCmd(GAME_CMD_SELF_STORE_BUYITEM)
		{
			targetId = 0;
			index = 0;
			price = 0;
			result = 0;
		}
	};




#pragma pack()

};	// namespace


#endif