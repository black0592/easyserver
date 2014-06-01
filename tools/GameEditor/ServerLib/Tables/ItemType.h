
#ifndef __Item_Type_H__
#define __Item_Type_H__

enum EItemType
{
	InvalidItemType                     = 0,	//无效
	EquipBegin                          = 1,	//装备开始
	Sword                               = 2,	//剑
	Bow                                 = 3,	//弓
	Pole                                = 4,	//枪
	Armor                               = 5,	//衣服
	Trousers                            = 6,	//裤子
	Axe                                 = 7,	//斧
	Staff                               = 8,	//杖
	Shield                              = 9,	//盾
	Helmet                              = 10,	//头盔
	Glove                               = 11,	//手套
	Shoes                               = 12,	//鞋子
	Wig                                 = 13,	//假发
	Cloak                               = 14,	//斗篷(暂无)
	Hat                                 = 15,	//帽子
	Bracelet                            = 16,	//手镯
	Necklace                            = 17,	//项链
	Ring                                = 18,	//戒指
	Earring                             = 19,	//护符
	Belt                                = 20,	//腰带
	Arrow                               = 21,	//箭支
	Wing                                = 22,	//翅膀
	EquipEnd                            = 23,	//装备结束
	Job                                 = 24,	//职业物品(暂无)
	Book                                = 25,	//书本
	VirtualMission                      = 26,	//虚拟任务道具
	Potions_Hp                          = 27,	//红色药水
	Potions_Mp                          = 28,	//蓝色药水
	Bag                                 = 29,	//背包
	Special                             = 30,	//特殊物品(很杂)
	StarStuff                           = 31,	//强化材料
	GrooveStuff                         = 32,	//打孔材料
	GenModel                            = 33,	//宝石模具
	Revive                              = 34,	//复活道具
	ExtendCard                          = 35,	//扩充卡
	AttribBall                          = 36,	//属性道具(水、火、风龙球)
	Citizenship                         = 37,	//公民权证书
	AttribPaper                         = 38,	//属性卷轴(水、火、风卷轴)
	DressMon                            = 39,	//变身道具
	Crown                               = 40,	//皇冠(暂无)
	Key                                 = 41,	//钥匙
	Mix                                 = 42,	//混合物品
	PetEgg                              = 43,	//宠物蛋
	Pet                                 = 44,	//宠物
	Mission                             = 45,	//任务道具
	Rune                                = 46,	//符文道具
	Know                                = 47,	//Know
	Checkup                             = 48,	//鉴定宝石
	ToGreen                             = 49,	//绿化宝石
	CreateItem                          = 50,	//物品转化出新物品
	PetExp                              = 51,	//宠物经验药水
	RepairStone                         = 52,	//修补石
	ScriptItem                          = 53,	//脚本物品
	WarriorSkillBook                    = 54,	//战士技能书
	MagicSkillBook                      = 55,	//法师技能书
	BowmanSkillBook                     = 56,	//游侠技能书
	DropMoney                           = 57,	//掉落金币
	FillStuffBegin                      = 58,	//镶嵌宝石开始
	FillStuff_HP                        = 59,	//生命宝石
	FillStuff_MP                        = 60,	//魔法宝石
	FillStuff_ATTACK                    = 61,	//攻击宝石
	FillStuff_PHY_DEF                   = 62,	//物防宝石
	FillStuff_MAG_DEF                   = 63,	//魔防宝石
	FillStuff_Dodge                     = 64,	//闪避宝石
	FillStuff_Hit                       = 65,	//命中宝石
	FillStuff_Critical                  = 66,	//暴击宝石
	FillStuff_Toughness                 = 67,	//防暴宝石
	FillStuffEnd                        = 68,	//镶嵌宝石结束
	GuildItem                           = 69,	//公会道具
};

#endif