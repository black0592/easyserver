#ifndef __ITEM_CMD_H__
#define __ITEM_CMD_H__
#include "BaseCmd.h"
#include "BaseStruct.h"
#include "Define.h"

namespace Protocol
{

#pragma pack(1)


	// 此功能块的结构基类
	struct stItemBaseCmd : public stBaseCmd
	{
		stItemBaseCmd(para_type para) : stBaseCmd(USERCMD_ITEM, para)
		{
		}
	};


	//////////////////////////////////////////////////////////////////////////


	struct stAddItemData 
	{
		stPkgPos loc;
		ItemData itemData;

		stAddItemData()
		{
		}
	};
	typedef stAddItemData stRemoveItemData;


	// 鉴定(解除封印)
	const para_type GAME_CMD_CHECKUP_ITEM = 11;
	struct stCheckupItem : stItemBaseCmd 
	{
		stPkgPos destPos;	// 装备坐标
		stPkgPos srcPos;	// 宝石坐标
		ItemData data;	// 解封后的数据
		ushort paperNum;		// (鉴定后)鉴定宝石的数量
		byte result;		// 返回值

		stCheckupItem() : stItemBaseCmd(GAME_CMD_CHECKUP_ITEM)
		{

		}
	};

	// 通知客户端更新物品耐久
	const para_type GAME_CMD_SET_ITEMDUR = 107;		// s -> c
	struct stSetItemDur : stItemBaseCmd
	{
		uint id;
		ushort index;	// 装备界面上的位置
		ushort dur;	// 耐久值
		stSetItemDur() : stItemBaseCmd(GAME_CMD_SET_ITEMDUR)
		{
			index = 0;
			dur = 0;
		}
	};

	// 更新物品的混合字段
	const para_type GAME_CMD_ITEM_MIXDATA = 214;
	struct stItemMixData : stItemBaseCmd 
	{
		uint id;
		ushort index;
		uint mixData;
		uint petMaxHP;
		uint petNextExp;

		stItemMixData() : stItemBaseCmd(GAME_CMD_ITEM_MIXDATA)
		{
			index = 0;
			mixData = 0;
			petMaxHP = petNextExp = 0;
		}
	};


	// 通知客户端创建一个物品
	const para_type GAME_CMD_CREATE_ITEM_RESULT = 216;		// s -> c
	struct stCreateItemResult : stItemBaseCmd
	{
		uint id;
		ushort pkg;
		ushort x;
		ushort y;
		ushort itemId;
		ushort itemCurDur;
		uint mixData;
		byte result;
		uint petMaxHP;		// 用于保存宠物最大血量
		uint petNextExp;	// 用于保存宠物最大生命

		stCreateItemResult() : stItemBaseCmd(GAME_CMD_CREATE_ITEM_RESULT)
		{
			id = 0;
			pkg = x = y = 0;
			itemId = 0;
			itemCurDur = 0;
			mixData = 0;
			result = 0;
			petMaxHP = petNextExp = 0;
		}
	};

	// 改变物品的属性值
	const para_type GAME_CMD_CHANGE_ITEM_PROP = 166;
	struct stChangeItemProp : stItemBaseCmd
	{
		stPkgPos pos[2];
		byte prop;
		byte result;

		stChangeItemProp() : stItemBaseCmd(GAME_CMD_CHANGE_ITEM_PROP)
		{
			prop = 0;
			result = 0;
		}
	};


	// 食物之源(用魔法创建食物)
	const para_type GAME_CMD_CREATE_FOOD = 119;
	struct stCreateFood : stItemBaseCmd
	{
		stPkgPos pos;
		ushort itemId;
		byte result;

		stCreateFood() : stItemBaseCmd(GAME_CMD_CREATE_FOOD)
		{
			itemId = 0;
			result = 0;
		}
	};

	// 白装变绿装
	const para_type GAME_CMD_TO_GREEN_TIEM = 134;
	struct stToGreenItem : stItemBaseCmd
	{
		stPkgPos destPos;	// 装备坐标
		stPkgPos srcPos;	// 宝石坐标
		ushort paperNum;	// (鉴定后)鉴定宝石的数量
		byte result;		// 返回值  0 - 成功， 1 - 不能强化的装备

		stToGreenItem() : stItemBaseCmd(GAME_CMD_TO_GREEN_TIEM)
		{
		}
	};

	// 用金币赌装备
	const para_type GAME_CMD_GAMBLE = 246;
	struct stGambleItem : stItemBaseCmd 
	{
		byte GambleType;
		byte GambleItemClass;

		stPkgPos destPos;

		ItemData ResultItem;
		byte result;				//0: success , 1: fail, 2: invalid gambleitem class, 3:not enough money

		stGambleItem() : stItemBaseCmd(GAME_CMD_GAMBLE)
		{

		}
	};

	
	// 增加绿装武器的暴击率
	const para_type GAME_CMD_CHANGE_TIEM_CRITICAL = 138;
	struct stChangeItemCritical : stItemBaseCmd
	{
		stPkgPos destPos;	// 装备坐标
		stPkgPos srcPos;	// 宝石坐标
		byte critical;		// 升级后的暴击率
		byte result;		// 返回值  0 - 成功， 1 - 不能强化的装备

		stChangeItemCritical() : stItemBaseCmd(GAME_CMD_CHANGE_TIEM_CRITICAL)
		{
		}
	};

	// 给装备增加占卜石
	const para_type GAME_CMD_ADD_RUNE_TO_ITEM = 237;
	struct stAddRuneToItem : stItemBaseCmd
	{
		stPkgPos runePos[3];	// 3个占卜石
		stPkgPos itemPos[2];	// 两个辅助道具
		stPkgPos equipPos;		// 装备位置
		ItemData equipData;	// 装备数据
		byte result;			// 返回值  0 - 成功， 1 - 不能强化的装备

		stAddRuneToItem() : stItemBaseCmd(GAME_CMD_ADD_RUNE_TO_ITEM)
		{
		}
	};


#pragma pack()

};	// namespace


#endif