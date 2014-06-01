
#ifndef __BuffType_H__
#define __BuffType_H__

enum BuffType
{
	BuffType_HPFix                      = 1,	//生命固定值
	BuffType_HPPercent                  = 2,	//生命百分比
	BuffType_MaxHPFix                   = 3,	//生命最大固定值
	BuffType_MaxHPPercent               = 4,	//生命最大百分比
	BuffType_ExtraDamageFix             = 5,	//额外伤害固定值
	BuffType_DamagePercent              = 6,	//额外伤害百分比
	BuffType_MoveSpeedFix               = 7,	//移动速度固定值
	BuffType_MoveSpeedPercent           = 8,	//移动速度百分比
	BuffType_DodgeFix                   = 9,	//闪避固定值
	BuffType_DodgePercent               = 10,	//闪避百分比
	BuffType_MagDefFix                  = 11,	//魔法防御固定值
	BuffType_MagDefPercent              = 12,	//魔法防御百分比
	BuffType_PhyDefFix                  = 13,	//物理防御固定值
	BuffType_PhyDefPercent              = 14,	//物理防御百分比
	BuffType_HitFix                     = 15,	//命中固定值
	BuffType_HitPercent                 = 16,	//命中百分比
};

#endif