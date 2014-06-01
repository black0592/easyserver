#ifndef __DEFINE_H__
#define __DEFINE_H__

#ifdef WIN32
#ifndef bzero
#define bzero(data,size) memset(data, 0, size);
#endif
#endif


/* ����Ϣ�� */
const byte USERCMD_CHARDATA			= 4;
const byte USERCMD_ITEM				= 5;
const byte USERCMD_TRADE			= 6;
const byte USERCMD_SIEGEWAR			= 8;
const byte USERCMD_SELFSTORE		= 9;

//////////////////////////////////////////////////////////////////////////

// �˺�������
#define ACCNAME_LEN	50

// ��ɫ������
#define CHARNAME_LEN	20

// ��ȷ�ķ���ֵ
#define RESULT_OK	0


#define INVALID_ID			0xFFFFFFFF

// ְҵ
enum ECharRace
{
	warrior,				// ��սʿ		
	fwarrior,				// Ůսʿ
	mage,					// �з�ʦ
	fmage,					// Ů��ʦ
	bowman,					// ������
	fbowman,				// Ů����
	InvalidCharRace,		// ��Ч������
};

// ����
enum ECountry
{
	Neutral = 0,	// ������
	Country1 = 1,	// ����1
	Country2,		// ����2
	Country3,		// ����3
	MaxCountry,		// ��Ч�Ĺ���
};


//////////////////////////////////////////////////////////////////////////

// �������ͣ�����ID��
enum PackageType
{
	EquipPack = 0,	// ְҵװ��
	AvatarPack,		// ʱװװ��
	BagPack,		// ����
	SafePack,		// �ֿ�
	QuickPack,		// �����
	JobPack,		// ְҵ��
	SkillPack,		// ������
	BuybackPack,	// �ع�����
	InvalidPack,	// ��Ч�İ���
};


// װ����λ
enum EEquipIndex
{
	E_Armor = 0,	// �·�
	E_Trousers = 1,	// ����
	E_Weapon = 2,	// ����
	E_Helmet = 3,	// ͷ��
	E_Belt,			// ����
	E_Shoes,		// Ь��
	E_Amulet,		// ����
	E_Bracelet,		// ����
	E_GLove,		// ����
	E_Necklace,		// ����
	E_Ring,			// ��ָ
	//////////////////////////////////////////////////////////////////////////
	E_Wing,			// ���
	E_Cloak,		// ͷ��(����������)
	E_Bag,			// ����
	E_Shield,		// ��
	E_MaxIndex,		// �����±�
};


// ��������
enum ESkillType
{
	SkillType_Active = 0,	// ��������
	SkillType_Passive,		// ��������(���ñ���)
};

// Ŀ������
enum ETargetType
{
	TargetType_Self = 1,		// �Լ�
	TargetType_Monster = 2,		// ����
	TargetType_Friend = 4,		// �ѷ�(�������Լ�)
	TargetType_Enemy = 8,		// �з�
	TargetType_Neutral = 16,	// ����(��ʱ��֧��)
};

// ��������
enum EElement
{
	Element_None = 0,	// ������
	Element_Water,		// ˮ
	Element_Wind,		// ��
	Element_Fire,		// ��
};


// ְҵ����
enum ERaceMask
{
	RaceMask_Warrior		= 1,		// ��սʿ		
	RaceMask_FemaleWarrior	= 2,		// Ůսʿ
	RaceMask_Mage			= 4,		// �з�ʦ
	RaceMask_FemaleMage		= 8,		// Ů��ʦ
	RaceMask_Bowman			= 16,		// ������
	RaceMask_FemaleBowman	= 32,		// Ů����
};



// ְҵתְҵ����
__inline ERaceMask RaceToRaceMask(ECharRace race)
{
	ERaceMask mask = RaceMask_Warrior;
	switch(race)
	{
	case warrior:
		mask = RaceMask_Warrior;
		break;
	case fwarrior:
		mask = RaceMask_FemaleWarrior;
		break;
	case mage:
		mask = RaceMask_Mage;
		break;
	case fmage:
		mask = RaceMask_FemaleMage;
		break;
	case bowman:
		mask = RaceMask_Bowman;
		break;
	case fbowman:
		mask = RaceMask_FemaleBowman;
		break;
	default:
		break;
	}

	return mask;
}

__inline bool checkRaceMask(uint race, uint needRace)
{
	ERaceMask useLimit = RaceToRaceMask((ECharRace)race);
	if ( !(useLimit & needRace) ) {
		return false;
	}

	return true;
}


// ����һ��װ��������
// equipGarde - װ������
// equipStarRate - װ��ǿ��ϵ��(����100�Ժ��ϵ��)
// jewelGarde - ��ʯ����
__inline int calcItemGrade(int equipGrade, float equipStarRate, int jewelGrade)
{
	// ǿ�����ֲ�������ȡ��
	int gardeStar = (int)(equipGrade * equipStarRate);
	// �ۼ�
	int totalGarde = equipGrade + gardeStar + jewelGrade;
	return totalGarde;
}

enum ItemRarity
{
	ItemRarity_Crude,		//�ֲ�
	ItemRarity_Normal,		//��ͨ
	ItemRarity_Good,		//����
	ItemRarity_Rare,		//ϡ��
	ItemRarity_Epic,		//ʷʫ
	ItemRarity_Inherit,		//����
};

enum class MissionRarity
{
	Normal,		//��ͨ
	Good,		//����
	Rare,		//ϡ��
	Epic,		//ʷʫ
	Inherit,	//����
	Count,		//������
};

enum class MissionType
{
	Main,			//"����"
	Branch,			//"֧��"
	UpgradeDialy,	//"�����ճ�"
	GuildDialy,		//"�����ճ�"
	PacifyDialy,	//"ƽ���ճ�"
	Count,			//����������
};


// ���ʶ
enum class EActivityStateFlag
{
	Opened = 0,		// ����
	Cloed = 1,		// �ر�
	Maintain = 2,	// ά��
};

// ��ͼPK��ʶ
enum class EMapPKFlag
{
	Safe = 0,		// ���԰�ȫ(��Һ����֮���޷�PK)
	Neutral = 1,	// ��������(ֻ�в�ͬ���ҵ���Ҳ���PK)
	Match = 2,		// ����(���䳡-����PK��û�н����ͳͷ�)
};



#endif		// __DEFINE_H__