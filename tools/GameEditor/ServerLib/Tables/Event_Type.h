
#ifndef __Event_Type_H__
#define __Event_Type_H__

#include "Event_Type.h"

enum EventType
{
	Event_Level                         = 1,	//���Ǵﵽ�ȼ�
	Event_ContinueLogin                 = 2,	//����������¼����
	Event_EquipUpgrade                  = 3,	//װ��ǿ����ָ���ȼ�
	Event_EquipCount                    = 4,	//ָ��Ʒ�ʵ�װ���ﵽָ������
	Event_StudyLevelSkill               = 5,	//ѧϰһ���ȼ��ļ���
	Event_Capacity                      = 6,	//�ﵽָ��ʵ��ֵ
	Event_KillEnemy                     = 7,	//����ж���Ӫ���������
	Event_VectoryBattle                 = 8,	//��������ս��ʤ���Ĵ���
	Event_CompleteDungeon               = 9,	//ͨ��ָ������ͨ����
	Event_FriendCount                   = 10,	//ӵ��һ�������ĺ���
	Event_GuildContribute               = 11,	//�����ۻ�����ﵽһ������
	Event_Recharge                      = 12,	//��Ϸ��ֵ
	Event_ChangeDay                     = 13,	//�ǵ�ǰ����ʱ���������߻����ߴ���
	Event_ChangeMap                     = 14,	//�л���ͼ
	Event_UserOnline                    = 15,	//�������
	Event_UserOffline                   = 16,	//�������
	Event_CompleteMission               = 17,	//���ָ��id������
	Event_PromptMail                    = 18,	//�ʼ�����
	Event_PromptActivity                = 19,	//�����
	Event_PromptAchievement             = 20,	//�ɾ�����
	Event_PromptGuildApplyFor           = 21,	//������������
	Event_PromptLevelupPoint            = 22,	//������������
	Event_FirstEnterGame                = 23,	//�½���ɫ���һ�ν���Ϸ
	Event_SecondTimer                   = 24,	//ÿ��ִ��һ��
	Event_JoinGuild                     = 25,	//���빫��
	Event_Marriage                      = 26,	//ӵ����ż
};

#endif