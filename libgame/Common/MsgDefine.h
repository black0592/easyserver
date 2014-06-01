#ifndef __MSG_DEFINE_H__
#define __MSG_DEFINE_H__
#include "ProtoMsgType.pb.h"
using namespace Protocol;

// ��¼��Ϣ
#define CMD_LOGIN(op)			MSG(MSG_LOGIN, op)

// ��ɫ�Լ�����Ϣ
#define CMD_SELECTCHAR(op)		MSG(MSG_SELECTCHAR, op)

// ��ɫ�Լ�����Ϣ
#define CMD_CHARDATA(op)		MSG(MSG_CHARDATA, op)

// ������������й�������Ϣ
#define CMD_SCENEDATA(op)		MSG(MSG_GAMEDATA, op)

// ��Ʒ���
#define CMD_ITEM(op)			MSG(MSG_ITEM, op)

// ����
#define CMD_TRADE(op)			MSG(MSG_TRADE, op)

// ���
#define CMD_TEAM(op)			MSG(MSG_TEAM, op)

// ����
#define CMD_SIEGEWAR(op)		MSG(MSG_SIEGEWAR, op)

// ��̯
#define CMD_SELFSTORE(op)		MSG(MSG_SELFSTORE, op)

// ����
#define CMD_MISSION(op)			MSG(MSG_MISSION, op)

// ����
#define CMD_GUILD(op)			MSG(MSG_GUILD, op)

// ս��
#define CMD_FIGHT(op)			MSG(MSG_FIGHT, op)

// �ʼ����
#define CMD_MAIL(op)			MSG(MSG_MAIL, op)

// �������
#define CMD_SKILL(op)			MSG(MSG_SKILL, op)

// ����ϵ
#define CMD_RELATIONS(op)		MSG(MSG_RELATIONS, op)

// �ű����
#define CMD_SCRIPT(op)			MSG(MSG_SCRIPT, op)

// ���а�
#define CMD_TOPLIST(op)			MSG(MSG_TOPLIST, op)

// װ��ǿ��
#define CMD_EQUIPUPGRADE(op)	MSG(MSG_EQUIPUPGRADE, op)

// ������
#define CMD_AUCTION(op)			MSG(MSG_AUCTION, op)

// ����
#define CMD_CHATTING(op)		MSG(MSG_CHATTING, op)

// �ɾ�ϵͳ
#define CMD_ACHIEVEMENT(op)		MSG(MSG_ACHIEVEMENT, op)

// �ϵͳ
#define CMD_ACTIVITY(op)		MSG(MSG_ACTIVITY, op)

// ����ʱ������
#define CMD_ONLINEREWARDS(op)	MSG(MSG_ONLINE_REWARDS, op)


// ����ͨ��(����֪ͨ����Ϸ����)
#define CMD_GENERAL(op)			MSG(MSG_GENERAL, op)


#endif



