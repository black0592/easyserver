#ifndef __DEFINE_H__
#define __DEFINE_H__

#ifdef WIN32
#ifndef bzero
#define bzero(data,size) memset(data, 0, size);
#endif
#endif


/* 主消息号 */
const byte USERCMD_CHARDATA			= 4;
const byte USERCMD_ITEM				= 5;
const byte USERCMD_TRADE			= 6;
const byte USERCMD_SIEGEWAR			= 8;
const byte USERCMD_SELFSTORE		= 9;

//////////////////////////////////////////////////////////////////////////

// 账号名长度
#define ACCNAME_LEN	50

// 角色名长度
#define CHARNAME_LEN	20

// 正确的返回值
#define RESULT_OK	0


#define INVALID_ID			0xFFFFFFFF

// 职业
enum ECharRace
{
	warrior,				// 男战士		
	fwarrior,				// 女战士
	mage,					// 男法师
	fmage,					// 女法师
	bowman,					// 男游侠
	fbowman,				// 女游侠
	InvalidCharRace,		// 无效的种族
};

// 国家
enum ECountry
{
	Neutral = 0,	// 中立的
	Country1 = 1,	// 国家1
	Country2,		// 国家2
	Country3,		// 国家3
	MaxCountry,		// 无效的国家
};


//////////////////////////////////////////////////////////////////////////

// 包裹类型（包裹ID）
enum PackageType
{
	EquipPack = 0,	// 职业装备
	AvatarPack,		// 时装装备
	BagPack,		// 背包
	SafePack,		// 仓库
	QuickPack,		// 快捷栏
	JobPack,		// 职业栏
	SkillPack,		// 技能栏
	BuybackPack,	// 回购包裹
	InvalidPack,	// 无效的包裹
};


// 装备孔位
enum EEquipIndex
{
	E_Armor = 0,	// 衣服
	E_Trousers = 1,	// 裤子
	E_Weapon = 2,	// 武器
	E_Helmet = 3,	// 头盔
	E_Belt,			// 腰带
	E_Shoes,		// 鞋子
	E_Amulet,		// 护符
	E_Bracelet,		// 手镯
	E_GLove,		// 手套
	E_Necklace,		// 项链
	E_Ring,			// 戒指
	//////////////////////////////////////////////////////////////////////////
	E_Wing,			// 翅膀
	E_Cloak,		// 头巾(宠物、变身道具)
	E_Bag,			// 背包
	E_Shield,		// 盾
	E_MaxIndex,		// 最大的下标
};


// 技能类型
enum ESkillType
{
	SkillType_Active = 0,	// 主动技能
	SkillType_Passive,		// 被动技能(永久被动)
};

// 目标类型
enum ETargetType
{
	TargetType_Self = 1,		// 自己
	TargetType_Monster = 2,		// 怪物
	TargetType_Friend = 4,		// 友方(不包含自己)
	TargetType_Enemy = 8,		// 敌方
	TargetType_Neutral = 16,	// 中立(暂时不支持)
};

// 五行属性
enum EElement
{
	Element_None = 0,	// 无属性
	Element_Water,		// 水
	Element_Wind,		// 风
	Element_Fire,		// 火
};


// 职业掩码
enum ERaceMask
{
	RaceMask_Warrior		= 1,		// 男战士		
	RaceMask_FemaleWarrior	= 2,		// 女战士
	RaceMask_Mage			= 4,		// 男法师
	RaceMask_FemaleMage		= 8,		// 女法师
	RaceMask_Bowman			= 16,		// 男游侠
	RaceMask_FemaleBowman	= 32,		// 女游侠
};



// 职业转职业掩码
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


// 计算一件装备的评分
// equipGarde - 装备评分
// equipStarRate - 装备强化系数(除以100以后的系数)
// jewelGarde - 宝石评分
__inline int calcItemGrade(int equipGrade, float equipStarRate, int jewelGrade)
{
	// 强化评分采用向下取整
	int gardeStar = (int)(equipGrade * equipStarRate);
	// 累加
	int totalGarde = equipGrade + gardeStar + jewelGrade;
	return totalGarde;
}

enum ItemRarity
{
	ItemRarity_Crude,		//粗糙
	ItemRarity_Normal,		//普通
	ItemRarity_Good,		//优秀
	ItemRarity_Rare,		//稀有
	ItemRarity_Epic,		//史诗
	ItemRarity_Inherit,		//传承
};

enum class MissionRarity
{
	Normal,		//普通
	Good,		//优秀
	Rare,		//稀有
	Epic,		//史诗
	Inherit,	//传承
	Count,		//类型数
};

enum class MissionType
{
	Main,			//"主线"
	Branch,			//"支线"
	UpgradeDialy,	//"历练日常"
	GuildDialy,		//"公会日常"
	PacifyDialy,	//"平定日常"
	Count,			//任务类型数
};


// 活动标识
enum class EActivityStateFlag
{
	Opened = 0,		// 开启
	Cloed = 1,		// 关闭
	Maintain = 2,	// 维护
};

// 地图PK标识
enum class EMapPKFlag
{
	Safe = 0,		// 绝对安全(玩家和玩家之间无法PK)
	Neutral = 1,	// 中立类型(只有不同国家的玩家才能PK)
	Match = 2,		// 竞技(比武场-任意PK，没有奖励和惩罚)
};



#endif		// __DEFINE_H__