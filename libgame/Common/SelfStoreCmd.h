#ifndef __SELFSTORE_CMD_H__
#define __SELFSTORE_CMD_H__
#include "BaseCmd.h"
#include "BaseStruct.h"
#include "Define.h"
#include "CharData.h"

namespace Protocol
{

#pragma pack(1)


	// �˹��ܿ�Ľṹ����
	struct stSelfStoreBaseCmd : public stBaseCmd
	{
		stSelfStoreBaseCmd(para_type para) : stBaseCmd(USERCMD_SELFSTORE, para)
		{
		}
	};


	//////////////////////////////////////////////////////////////////////////


	
	// ��̯��ʼ
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

	// ̯λ״̬
	const para_type GAME_CMD_SELF_STORE_STATE = 241;
	struct stSelfStoreState : stSelfStoreBaseCmd
	{
		byte state;
		stSelfStoreState() : stSelfStoreBaseCmd(GAME_CMD_SELF_STORE_STATE)
		{
			state = 0;
		}
	};

	// ��ȡ����
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

	// ������Ʒ
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