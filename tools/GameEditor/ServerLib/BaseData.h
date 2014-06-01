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
// 怪物基础属性表
struct stMonBase 
{
	uint id;			//  race编号
	char name[NAME_SIZE+1];	// 名字
	uint level;			// 等级
	uint type;			// 类型 0 - 普通，1 - 精英，2 - BOSS
	uint maxHP;			// 最大血量
	uint addHP;			// 每秒回复的血量
	uint phyAttack;		// 物理攻击力
	uint magAttack;		// 魔法攻击力
	uint phyDefence;	// 物理防御力
	uint magDefence;	// 魔法防御力
	uint getExp;		// 死亡后玩家能得到的经验值
	uint dropMoney;		// 掉落金钱数
	uint dropMoneyProb;	// 掉落金钱概率(万分比)
	uint magicClass;	// 魔法类型
	uint prop;			// 属性类型
	uint propValue;		// 属性值
	uint flight;		// 是否飞行
	uint sickness;		// 得病的类型
	uint sicknessProb;	// 得病的概率

	stMonBase()
	{
		bzero(this, sizeof(*this));
	}
};
*/



// 玩家经验表
struct stCharExpBase 
{
	uint id;	// 等级
	uint exp;	// 升级经验

	stCharExpBase()
	{
		bzero(this, sizeof(*this));
	}
};


// 职业经验表
struct stJobExpBase 
{
	uint id;	// 等级
	uint exp;	// 经验值

	stJobExpBase()
	{
		bzero(this, sizeof(*this));
	}
};



// 宠物信息表
struct stPetInfo
{
	uint id;	// 等级
	uint maxHP;	// 最大血量
	uint exp;	// 升级经验

	stPetInfo()
	{
		bzero(this, sizeof(*this));
	}
};


// 翅膀信息表
struct stWingInfo
{
	uint id;	// 等级
	uint exp;	// 升级经验

	stWingInfo()
	{
		bzero(this, sizeof(*this));
	}
};


/*
// 地图属性表
struct stMapInfo
{
	uint id;				// 编号
	char name[NAME_SIZE+1];	// 名字
	char mapfile[255];		// 地图的文件名
	uint kind;				// 地图类型
	uint safeFlag;			// 安全标志 1 - 不可PK，0 - 可PK
	char pkZone[255];		// PK区域
	uint checkAttrib;		// 检查属性
	uint mainMapId;			// 主地图编号
	uint countryId;			// 归属国家编号
	uint light;				// 是否有亮光
	uint weather;			// 是否有天气

	stMapInfo()
	{
		bzero(this, sizeof(*this));
	}
};
*/


//// 地图亮度表
//struct stLuminosityBase 
//{
//	uint id;			// 编号
//	uint luminosity;	// 亮度值
//
//	stLuminosityBase()
//	{
//		bzero(this, sizeof(*this));
//	}
//};


/*
// 物品基础表
struct stItemBase 
{
	uint id;				// 编号
	char name[NAME_SIZE+1];	// 名字
	uint type;				// 类型
	uint attack;			// 攻击力
	uint phyDefence;		// 物理防御力
	uint magDefence;		// 魔法防御力
	uint dur;				// 耐久值
	uint price;				// 价格
	uint canRepair;			// 可修理
	uint canSell;			// 可出售
	uint needLevel;			// 等级需求
	uint needStr;			// 力量需求
	uint needDex;			// 敏捷需求
	uint needWiz;			// 精神力需求
	uint needInt;			// 智力需求
	uint needCon;			// 体力需求
	uint needRace;			// 种族需求
	uint skillId;			// 技能ID
	uint res;				// 效果字段(变身道具，跑鞋...)
	uint accuracy;			// 命中率
	uint setType;			// 套装类型
	uint setNum;			// 套装编号
	uint maxHP;				// 最大生命值
	uint maxMP;				// 最大魔法值
	uint maxSP;				// 最大耐力之
	uint scriptId;			// 脚本编号
	uint params[4];			// 参数1-4

	stItemBase()
	{
		bzero(this, sizeof(*this));
	}
};
*/


// GM可以刷的物品基础表
struct stGMItemBase 
{
	uint id;				// 编号
	char name[NAME_SIZE+1];	// 名字

	stGMItemBase()
	{
		bzero(this, sizeof(*this));
	}
};



/*
// 类型
//201 - 法师魔法
//202 - 游侠魔法
//203 - 战士技能
//204 - 游侠技能
//101 - 铁匠技能
//...

// 目标类型
// 1 - 玩家
// 2 - 自己
// 4 - 其他
// 8 - 怪物
// 16 - 无(扩展)
// 32 - 玩家(扩展)
// 64 - 怪物(扩展)
// 128 - 事件(扩展)

// 技能基础表
struct stSkillBase
{
	uint id;		// 编号
	char name[SKL_NAME_SIZE+1];	// 名字
	uint isAttack;		// 是否攻击流程
	uint skillKind;		// 技能类型
	uint attackKind;	// 攻击类型
	float factor1;	// 攻击系数
	float factor2;	// 属性攻击系数
	uint addState;	// 附加状态ID号
	uint stateTime;	// 状态持续时间(秒)
	uint targetNum;	// 目标数量
	uint coolDown;	// 冷却时间
	uint needHP;	// 需要生命值
	uint needMP;	// 需要魔法值
	uint needSP;	// 需要技能值
	uint needHG;	// 需要饥饿值
	uint target;	// 目标类型
	uint attrib;	// 属性

	stSkillBase()
	{
		bzero(this, sizeof(*this));
	}
};
*/


#define MISS_ITEM_NUM	8

// 任务配置表格
struct stMissionBase 
{
	uint id;			// 任务编号
	char name[NAME_SIZE+1];	// 名字
	uint type;			// 任务类型
	uint loop;			// 是否循环任务
	uint needLevel;		// 需求等级
	uint needMonId;		// 需求怪物ID
	uint needMonNum;	// 需求怪物数量
	uint needItem[MISS_ITEM_NUM]; // 需求物品
	uint eType;			// 奖励类型
	uint eExp;			// 奖励经验
	uint eMoney;		// 奖励金币
	uint eItem[MISS_ITEM_NUM];	// 奖励物品

	stMissionBase()
	{
		bzero(this, sizeof(*this));
	}
};

// 商城物品表
struct stCashItemBase 
{
	uint id;			// 产品编号
	uint group;			// 分页(暂时1-5页)
	uint itemId;		// 物品编号
	uint originalPrice;	// 原价
	uint discountPrice;	// 折扣价
	uint recommend;		// 推荐状态(1-推荐)
	uint itemNum;		// 显示数量(大于0才显示)
	uint sellable;		// 出售状态(0-停止销售,1-开始销售)
	uint isgreen;		// 是否绿装

	stCashItemBase()
	{
		bzero(this, sizeof(*this));
	}
};


// 圣诞活动物品表
struct stChristmasItemBase
{
	uint id;				// 序号
	uint itemId;			// 物品编号
	char name[NAME_SIZE+1];	// 物品名称
	uint type;				// 类型
	uint star;				// 星数
	uint itemNum;			// 物品数量

	stChristmasItemBase()
	{
		bzero(this, sizeof(*this));
	}
};

/*
// NPC属性表
struct stNpcBase
{
	uint id;				// 编号
	char name[NAME_SIZE+1];	// npc名字
	uint imgId;				// 图片编号

	stNpcBase()
	{
		bzero(this, sizeof(*this));
	}
};
*/

// 聊天过滤字
struct stChatFilter
{
	uint id;				// 编号
	char str[NAME_SIZE+1];	// 过滤字

	stChatFilter()
	{
		bzero(this, sizeof(*this));
	}
};

// 怪物入侵活动
#define MON_DROP_NUM	2
struct MonDropInfo
{
	int dropId;		// 掉落编号
	int dropProb;	// 掉落概率(万分比)
	int dropType;	// 掉落类型(0 - 多选一, 1 - 全掉落，单个80%概率)
};

struct stMonsterInbreak
{
	uint id;					// 编号
	uint classId;				// 分类编号
	uint monsterRace;			// 怪物编号
	uint monsterNum;			// 怪物数量
	char comment[NAME_SIZE+1];	// 备注
	float hpRate;				// 生命值倍数
	float phyAtkRate;			// 物理攻击倍数
	float magAtkRate;			// 魔法攻击倍数
	float phyDefRate;			// 物理防御倍数
	float magDefRate;			// 魔法防御倍数
	uint moveSpeed;				// 移动速度
	uint givenExp;				// 经验值
	uint dropMoney;				// 掉落金币
	uint dropMoneyProb;			// 掉落金币概率
	MonDropInfo dropItems[MON_DROP_NUM];// 物品掉落
};


#pragma pack()






