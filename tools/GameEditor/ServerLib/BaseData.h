#pragma once

#ifdef WIN32
#ifndef bzero
#define bzero(data,size) memset(data, 0, size);
#endif
#endif

#include "ItemTable.h"
#include "equipmentTable.h"
#include "MapTable.h"
#include "NpcTable.h"
#include "MonsterTable.h"
#include "SkillTable.h"
#include "DropItemTable.h"
#include "ShopTable.h"
using namespace Store;

#pragma pack(1)

#define NAME_SIZE	49
#define SKL_NAME_SIZE	63

/*
// ����������Ա�
struct stMonBase 
{
	uint id;			//  race���
	char name[NAME_SIZE+1];	// ����
	uint level;			// �ȼ�
	uint type;			// ���� 0 - ��ͨ��1 - ��Ӣ��2 - BOSS
	uint maxHP;			// ���Ѫ��
	uint addHP;			// ÿ��ظ���Ѫ��
	uint phyAttack;		// ��������
	uint magAttack;		// ħ��������
	uint phyDefence;	// ���������
	uint magDefence;	// ħ��������
	uint getExp;		// ����������ܵõ��ľ���ֵ
	uint dropMoney;		// �����Ǯ��
	uint dropMoneyProb;	// �����Ǯ����(��ֱ�)
	uint magicClass;	// ħ������
	uint prop;			// ��������
	uint propValue;		// ����ֵ
	uint flight;		// �Ƿ����
	uint sickness;		// �ò�������
	uint sicknessProb;	// �ò��ĸ���

	stMonBase()
	{
		bzero(this, sizeof(*this));
	}
};
*/



// ��Ҿ����
struct stCharExpBase 
{
	uint id;	// �ȼ�
	uint exp;	// ��������

	stCharExpBase()
	{
		bzero(this, sizeof(*this));
	}
};


// ְҵ�����
struct stJobExpBase 
{
	uint id;	// �ȼ�
	uint exp;	// ����ֵ

	stJobExpBase()
	{
		bzero(this, sizeof(*this));
	}
};



// ������Ϣ��
struct stPetInfo
{
	uint id;	// �ȼ�
	uint maxHP;	// ���Ѫ��
	uint exp;	// ��������

	stPetInfo()
	{
		bzero(this, sizeof(*this));
	}
};


// �����Ϣ��
struct stWingInfo
{
	uint id;	// �ȼ�
	uint exp;	// ��������

	stWingInfo()
	{
		bzero(this, sizeof(*this));
	}
};


/*
// ��ͼ���Ա�
struct stMapInfo
{
	uint id;				// ���
	char name[NAME_SIZE+1];	// ����
	char mapfile[255];		// ��ͼ���ļ���
	uint kind;				// ��ͼ����
	uint safeFlag;			// ��ȫ��־ 1 - ����PK��0 - ��PK
	char pkZone[255];		// PK����
	uint checkAttrib;		// �������
	uint mainMapId;			// ����ͼ���
	uint countryId;			// �������ұ��
	uint light;				// �Ƿ�������
	uint weather;			// �Ƿ�������

	stMapInfo()
	{
		bzero(this, sizeof(*this));
	}
};
*/


//// ��ͼ���ȱ�
//struct stLuminosityBase 
//{
//	uint id;			// ���
//	uint luminosity;	// ����ֵ
//
//	stLuminosityBase()
//	{
//		bzero(this, sizeof(*this));
//	}
//};


/*
// ��Ʒ������
struct stItemBase 
{
	uint id;				// ���
	char name[NAME_SIZE+1];	// ����
	uint type;				// ����
	uint attack;			// ������
	uint phyDefence;		// ���������
	uint magDefence;		// ħ��������
	uint dur;				// �;�ֵ
	uint price;				// �۸�
	uint canRepair;			// ������
	uint canSell;			// �ɳ���
	uint needLevel;			// �ȼ�����
	uint needStr;			// ��������
	uint needDex;			// ��������
	uint needWiz;			// ����������
	uint needInt;			// ��������
	uint needCon;			// ��������
	uint needRace;			// ��������
	uint skillId;			// ����ID
	uint res;				// Ч���ֶ�(������ߣ���Ь...)
	uint accuracy;			// ������
	uint setType;			// ��װ����
	uint setNum;			// ��װ���
	uint maxHP;				// �������ֵ
	uint maxMP;				// ���ħ��ֵ
	uint maxSP;				// �������֮
	uint scriptId;			// �ű����
	uint params[4];			// ����1-4

	stItemBase()
	{
		bzero(this, sizeof(*this));
	}
};
*/


// GM����ˢ����Ʒ������
struct stGMItemBase 
{
	uint id;				// ���
	char name[NAME_SIZE+1];	// ����

	stGMItemBase()
	{
		bzero(this, sizeof(*this));
	}
};



/*
// ����
//201 - ��ʦħ��
//202 - ����ħ��
//203 - սʿ����
//204 - ��������
//101 - ��������
//...

// Ŀ������
// 1 - ���
// 2 - �Լ�
// 4 - ����
// 8 - ����
// 16 - ��(��չ)
// 32 - ���(��չ)
// 64 - ����(��չ)
// 128 - �¼�(��չ)

// ���ܻ�����
struct stSkillBase
{
	uint id;		// ���
	char name[SKL_NAME_SIZE+1];	// ����
	uint isAttack;		// �Ƿ񹥻�����
	uint skillKind;		// ��������
	uint attackKind;	// ��������
	float factor1;	// ����ϵ��
	float factor2;	// ���Թ���ϵ��
	uint addState;	// ����״̬ID��
	uint stateTime;	// ״̬����ʱ��(��)
	uint targetNum;	// Ŀ������
	uint coolDown;	// ��ȴʱ��
	uint needHP;	// ��Ҫ����ֵ
	uint needMP;	// ��Ҫħ��ֵ
	uint needSP;	// ��Ҫ����ֵ
	uint needHG;	// ��Ҫ����ֵ
	uint target;	// Ŀ������
	uint attrib;	// ����

	stSkillBase()
	{
		bzero(this, sizeof(*this));
	}
};
*/


#define MISS_ITEM_NUM	8

// �������ñ��
struct stMissionBase 
{
	uint id;			// ������
	char name[NAME_SIZE+1];	// ����
	uint type;			// ��������
	uint loop;			// �Ƿ�ѭ������
	uint needLevel;		// ����ȼ�
	uint needMonId;		// �������ID
	uint needMonNum;	// �����������
	uint needItem[MISS_ITEM_NUM]; // ������Ʒ
	uint eType;			// ��������
	uint eExp;			// ��������
	uint eMoney;		// �������
	uint eItem[MISS_ITEM_NUM];	// ������Ʒ

	stMissionBase()
	{
		bzero(this, sizeof(*this));
	}
};

// �̳���Ʒ��
struct stCashItemBase 
{
	uint id;			// ��Ʒ���
	uint group;			// ��ҳ(��ʱ1-5ҳ)
	uint itemId;		// ��Ʒ���
	uint originalPrice;	// ԭ��
	uint discountPrice;	// �ۿۼ�
	uint recommend;		// �Ƽ�״̬(1-�Ƽ�)
	uint itemNum;		// ��ʾ����(����0����ʾ)
	uint sellable;		// ����״̬(0-ֹͣ����,1-��ʼ����)
	uint isgreen;		// �Ƿ���װ

	stCashItemBase()
	{
		bzero(this, sizeof(*this));
	}
};


// ʥ�����Ʒ��
struct stChristmasItemBase
{
	uint id;				// ���
	uint itemId;			// ��Ʒ���
	char name[NAME_SIZE+1];	// ��Ʒ����
	uint type;				// ����
	uint star;				// ����
	uint itemNum;			// ��Ʒ����

	stChristmasItemBase()
	{
		bzero(this, sizeof(*this));
	}
};

/*
// NPC���Ա�
struct stNpcBase
{
	uint id;				// ���
	char name[NAME_SIZE+1];	// npc����
	uint imgId;				// ͼƬ���

	stNpcBase()
	{
		bzero(this, sizeof(*this));
	}
};
*/

// ���������
struct stChatFilter
{
	uint id;				// ���
	char str[NAME_SIZE+1];	// ������

	stChatFilter()
	{
		bzero(this, sizeof(*this));
	}
};

// �������ֻ
#define MON_DROP_NUM	2
struct MonDropInfo
{
	int dropId;		// ������
	int dropProb;	// �������(��ֱ�)
	int dropType;	// ��������(0 - ��ѡһ, 1 - ȫ���䣬����80%����)
};

struct stMonsterInbreak
{
	uint id;					// ���
	uint classId;				// ������
	uint monsterRace;			// ������
	uint monsterNum;			// ��������
	char comment[NAME_SIZE+1];	// ��ע
	float hpRate;				// ����ֵ����
	float phyAtkRate;			// ����������
	float magAtkRate;			// ħ����������
	float phyDefRate;			// �����������
	float magDefRate;			// ħ����������
	uint moveSpeed;				// �ƶ��ٶ�
	uint givenExp;				// ����ֵ
	uint dropMoney;				// ������
	uint dropMoneyProb;			// �����Ҹ���
	MonDropInfo dropItems[MON_DROP_NUM];// ��Ʒ����
};


#pragma pack()






