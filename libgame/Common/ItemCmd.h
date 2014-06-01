#ifndef __ITEM_CMD_H__
#define __ITEM_CMD_H__
#include "BaseCmd.h"
#include "BaseStruct.h"
#include "Define.h"

namespace Protocol
{

#pragma pack(1)


	// �˹��ܿ�Ľṹ����
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


	// ����(�����ӡ)
	const para_type GAME_CMD_CHECKUP_ITEM = 11;
	struct stCheckupItem : stItemBaseCmd 
	{
		stPkgPos destPos;	// װ������
		stPkgPos srcPos;	// ��ʯ����
		ItemData data;	// ���������
		ushort paperNum;		// (������)������ʯ������
		byte result;		// ����ֵ

		stCheckupItem() : stItemBaseCmd(GAME_CMD_CHECKUP_ITEM)
		{

		}
	};

	// ֪ͨ�ͻ��˸�����Ʒ�;�
	const para_type GAME_CMD_SET_ITEMDUR = 107;		// s -> c
	struct stSetItemDur : stItemBaseCmd
	{
		uint id;
		ushort index;	// װ�������ϵ�λ��
		ushort dur;	// �;�ֵ
		stSetItemDur() : stItemBaseCmd(GAME_CMD_SET_ITEMDUR)
		{
			index = 0;
			dur = 0;
		}
	};

	// ������Ʒ�Ļ���ֶ�
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


	// ֪ͨ�ͻ��˴���һ����Ʒ
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
		uint petMaxHP;		// ���ڱ���������Ѫ��
		uint petNextExp;	// ���ڱ�������������

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

	// �ı���Ʒ������ֵ
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


	// ʳ��֮Դ(��ħ������ʳ��)
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

	// ��װ����װ
	const para_type GAME_CMD_TO_GREEN_TIEM = 134;
	struct stToGreenItem : stItemBaseCmd
	{
		stPkgPos destPos;	// װ������
		stPkgPos srcPos;	// ��ʯ����
		ushort paperNum;	// (������)������ʯ������
		byte result;		// ����ֵ  0 - �ɹ��� 1 - ����ǿ����װ��

		stToGreenItem() : stItemBaseCmd(GAME_CMD_TO_GREEN_TIEM)
		{
		}
	};

	// �ý�Ҷ�װ��
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

	
	// ������װ�����ı�����
	const para_type GAME_CMD_CHANGE_TIEM_CRITICAL = 138;
	struct stChangeItemCritical : stItemBaseCmd
	{
		stPkgPos destPos;	// װ������
		stPkgPos srcPos;	// ��ʯ����
		byte critical;		// ������ı�����
		byte result;		// ����ֵ  0 - �ɹ��� 1 - ����ǿ����װ��

		stChangeItemCritical() : stItemBaseCmd(GAME_CMD_CHANGE_TIEM_CRITICAL)
		{
		}
	};

	// ��װ������ռ��ʯ
	const para_type GAME_CMD_ADD_RUNE_TO_ITEM = 237;
	struct stAddRuneToItem : stItemBaseCmd
	{
		stPkgPos runePos[3];	// 3��ռ��ʯ
		stPkgPos itemPos[2];	// ������������
		stPkgPos equipPos;		// װ��λ��
		ItemData equipData;	// װ������
		byte result;			// ����ֵ  0 - �ɹ��� 1 - ����ǿ����װ��

		stAddRuneToItem() : stItemBaseCmd(GAME_CMD_ADD_RUNE_TO_ITEM)
		{
		}
	};


#pragma pack()

};	// namespace


#endif