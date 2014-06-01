
#ifndef __BuffType_H__
#define __BuffType_H__

enum BuffType
{
	BuffType_HPFix                      = 1,	//�����̶�ֵ
	BuffType_HPPercent                  = 2,	//�����ٷֱ�
	BuffType_MaxHPFix                   = 3,	//�������̶�ֵ
	BuffType_MaxHPPercent               = 4,	//�������ٷֱ�
	BuffType_ExtraDamageFix             = 5,	//�����˺��̶�ֵ
	BuffType_DamagePercent              = 6,	//�����˺��ٷֱ�
	BuffType_MoveSpeedFix               = 7,	//�ƶ��ٶȹ̶�ֵ
	BuffType_MoveSpeedPercent           = 8,	//�ƶ��ٶȰٷֱ�
	BuffType_DodgeFix                   = 9,	//���̶ܹ�ֵ
	BuffType_DodgePercent               = 10,	//���ܰٷֱ�
	BuffType_MagDefFix                  = 11,	//ħ�������̶�ֵ
	BuffType_MagDefPercent              = 12,	//ħ�������ٷֱ�
	BuffType_PhyDefFix                  = 13,	//��������̶�ֵ
	BuffType_PhyDefPercent              = 14,	//��������ٷֱ�
	BuffType_HitFix                     = 15,	//���й̶�ֵ
	BuffType_HitPercent                 = 16,	//���аٷֱ�
};

#endif