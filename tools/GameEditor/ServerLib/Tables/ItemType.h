
#ifndef __Item_Type_H__
#define __Item_Type_H__

enum EItemType
{
	InvalidItemType                     = 0,	//��Ч
	EquipBegin                          = 1,	//װ����ʼ
	Sword                               = 2,	//��
	Bow                                 = 3,	//��
	Pole                                = 4,	//ǹ
	Armor                               = 5,	//�·�
	Trousers                            = 6,	//����
	Axe                                 = 7,	//��
	Staff                               = 8,	//��
	Shield                              = 9,	//��
	Helmet                              = 10,	//ͷ��
	Glove                               = 11,	//����
	Shoes                               = 12,	//Ь��
	Wig                                 = 13,	//�ٷ�
	Cloak                               = 14,	//����(����)
	Hat                                 = 15,	//ñ��
	Bracelet                            = 16,	//����
	Necklace                            = 17,	//����
	Ring                                = 18,	//��ָ
	Earring                             = 19,	//����
	Belt                                = 20,	//����
	Arrow                               = 21,	//��֧
	Wing                                = 22,	//���
	EquipEnd                            = 23,	//װ������
	Job                                 = 24,	//ְҵ��Ʒ(����)
	Book                                = 25,	//�鱾
	VirtualMission                      = 26,	//�����������
	Potions_Hp                          = 27,	//��ɫҩˮ
	Potions_Mp                          = 28,	//��ɫҩˮ
	Bag                                 = 29,	//����
	Special                             = 30,	//������Ʒ(����)
	StarStuff                           = 31,	//ǿ������
	GrooveStuff                         = 32,	//��ײ���
	GenModel                            = 33,	//��ʯģ��
	Revive                              = 34,	//�������
	ExtendCard                          = 35,	//���俨
	AttribBall                          = 36,	//���Ե���(ˮ���𡢷�����)
	Citizenship                         = 37,	//����Ȩ֤��
	AttribPaper                         = 38,	//���Ծ���(ˮ���𡢷����)
	DressMon                            = 39,	//�������
	Crown                               = 40,	//�ʹ�(����)
	Key                                 = 41,	//Կ��
	Mix                                 = 42,	//�����Ʒ
	PetEgg                              = 43,	//���ﵰ
	Pet                                 = 44,	//����
	Mission                             = 45,	//�������
	Rune                                = 46,	//���ĵ���
	Know                                = 47,	//Know
	Checkup                             = 48,	//������ʯ
	ToGreen                             = 49,	//�̻���ʯ
	CreateItem                          = 50,	//��Ʒת��������Ʒ
	PetExp                              = 51,	//���ﾭ��ҩˮ
	RepairStone                         = 52,	//�޲�ʯ
	ScriptItem                          = 53,	//�ű���Ʒ
	WarriorSkillBook                    = 54,	//սʿ������
	MagicSkillBook                      = 55,	//��ʦ������
	BowmanSkillBook                     = 56,	//����������
	DropMoney                           = 57,	//������
	FillStuffBegin                      = 58,	//��Ƕ��ʯ��ʼ
	FillStuff_HP                        = 59,	//������ʯ
	FillStuff_MP                        = 60,	//ħ����ʯ
	FillStuff_ATTACK                    = 61,	//������ʯ
	FillStuff_PHY_DEF                   = 62,	//�����ʯ
	FillStuff_MAG_DEF                   = 63,	//ħ����ʯ
	FillStuff_Dodge                     = 64,	//���ܱ�ʯ
	FillStuff_Hit                       = 65,	//���б�ʯ
	FillStuff_Critical                  = 66,	//������ʯ
	FillStuff_Toughness                 = 67,	//������ʯ
	FillStuffEnd                        = 68,	//��Ƕ��ʯ����
	GuildItem                           = 69,	//�������
};

#endif