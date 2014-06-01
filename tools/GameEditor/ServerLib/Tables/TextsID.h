
#ifndef __TextsID_TABLE_READER_H__
#define __TextsID_TABLE_READER_H__

#include "TextTable.h"

enum TID_Type
{
	TID_ITEM_BAG_FULL                   = 1,	//���ı�������
	TID_ITEM_PICKUP_BAG_FULL            = 2,	//���ı����������޷���ȡ
	TID_ITEM_DROP_ITEM_EXIST            = 3,	//��λ���Ѵ��ڵ��ߣ��߼��������ط��Ӱ�
	TID_DATA_MONEY_NOT_ENOUGH           = 4,	//���Ľ�Ҳ���
	TID_DATA_GOLD_NOT_ENOUGH            = 5,	//������ʯ����
	TID_DATA_GULD_CREDITS_NOT_ENOUGH    = 6,	//���Ĺ��ṱ�ײ���
	TID_DATA_LEVEL_NOT_ENOUGH           = 7,	//���ĵȼ�����
	TID_SYSTEM_NAME                     = 8,	//ϵͳ
	TID_AUCTION_NAME                    = 9,	//������
	TID_MAIL_NOT_EXIST_NAME             = 10,	//�ռ��˲�����
	TID_MAIL_HAS_NEWMAIL                = 11,	//�����µ��ʼ�����ע�����
	TID_MAIL_NOT_WRITE_SELF             = 12,	//���ܸ��Լ����ʼ�
	TID_USER_NOT_ONLINE                 = 13,	//����Ҳ�����
	TID_FRIEND_ALREADY_ADDED            = 14,	//������Ѿ������ĺ���
	TID_TEAM_ALREADY_HAVE_TEAM          = 15,	//������Ѿ��ж�����
	TID_TEAM_INVITE_NOT_MASTER          = 16,	//ֻ�жӳ���������������
	TID_TEAM_INVITE_NOT_AGREE           = 17,	//�Է��ܾ�������������
	TID_TEAM_JOIN_SUCCESS               = 18,	//�������� %s �Ķ���
	TID_TEAM_JOIN_USER                  = 19,	//��� %s ���������Ķ���
	TID_MARRY_NOT_ONLINE                = 20,	//��ȷ��˫������ͬһ����
	TID_UNMARRY_NOT_ONLINE              = 21,	//˫��������ͬһ����������Э�����
	TID_UNMARRY_NOT_MARRY               = 22,	//����δ��飬�޷����
	TID_DUP_VOTE_NOT_MASTER             = 23,	//ֻ�жӳ����ܷ�����븱����ͶƱ
	TID_DUP_VOTE                        = 24,	//�ѷ�����븱����ͶƱ����ȴ������Ӧ
	TID_DUP_MEMBER_LEVEL_ERROR          = 25,	//��Ա%s�ȼ��������޷����븱��
	TID_DUP_VOTE_NOT_AGREE              = 26,	//��Ա%s�ܾ����븱��
	TID_GUILD_CREATE_LEVEL_ERROR        = 27,	//�ȼ��������޷���������
	TID_GUILD_CREATE_NAME_ERROR         = 28,	//��������ʧ�ܣ���Ч�Ĺ�����
	TID_GUILD_JOIN_LEVEL_ERROR          = 29,	//�ȼ��������޷����빫��
	TID_GUILD_ALREADY_IN_THIS           = 30,	//�Ѿ��ڹ�������
	TID_GUILD_ALREADY_IN_OTHER          = 31,	//�Է��Ѿ���������������
	TID_GUILD_MEMBER_FULL               = 32,	//������������
	TID_GUILD_INVITE_NOT_AGREE          = 33,	//�Է��ܾ�����Ĺ�������
	TID_GUILD_ALREADY_APPLY             = 34,	//���Ѿ�������ˣ�����������
	TID_GUILD_MASTER_LEAVE_ERROR        = 35,	//��ת���᳤ְ������뿪�л�
	TID_GUILD_INIT_NOTICE               = 36,	//��ӭ���롾%s������
	TID_GUILD_MANAGER_FULL              = 37,	//����Ա��������
	TID_AUCTION_NOT_EXIST               = 38,	//�˼�����Ʒ�Ѳ���
	TID_AUCTION_NOT_BUY_SELFT           = 39,	//���ܹ����Լ����۵���Ʒ
	TID_AUCTION_MONEY_EXPIRED           = 40,	//�����۵�%d���δ���۳����뼰ʱ��ȡ�����з����Ľ��
	TID_AUCTION_ITEM_EXPIRED            = 41,	//�����۵���Ʒ��%s��δ���۳����뼰ʱ��ȡ�����з�������Ʒ
	TID_AUCTION_BACKOUT_MONEY           = 42,	//�����۵�%d��Ҵ��������¼ܣ��뼰ʱ��ȡ�����з����Ľ��
	TID_AUCTION_BACKOUT_ITEM            = 43,	//�����۵���Ʒ��%s�����������¼ܣ��뼰ʱ��ȡ�����з�������Ʒ
	TID_AUCTION_REQUIRE_GOLD_MIN        = 44,	//���ۼ۸���С��%u����ʯ
	TID_AUCTION_UNIMONEY_MIN            = 45,	//���۵��۲���С��%u���
	TID_AUCTION_SELL_ITEM_MAIL          = 46,	//�����۵���Ʒ��%s�����۳����۳�%u%%�Ľ���˰�������%u��ң��뼰ʱ��ȡ����
	TID_AUCTION_SELL_MONEY_MAIL         = 47,	//�����۵�%d������۳����۳�%u%%�Ľ���˰�������%u��ʯ���뼰ʱ��ȡ����
	TID_CHATTING_ITEM_NOT_EXIST         = 48,	//����Ϣ�Ѳ�����
	TID_UPGRADE_EQUIP_SUCCESS_INFO      = 49,	//̫�����ˣ�%s �� %s ǿ������ %d ��
	TID_ITEM_SAFE_FULL                  = 50,	//���Ĳֿ��������޷����
	TID_EQUIP_NOT_NEED_REPAIR           = 51,	//����װ��û���𻵣�����Ҫ����
	TID_UPGRADE_EQUIP_SAME_STUFF        = 52,	//һ��װ��ֻ����Ƕһ��ͬ���͵ı�ʯ
	TID_EQUIP_EXPIRED_ITEM              = 53,	//�޷�װ���ѹ��ڵ�ʱװ������������
};

static const char* TID_Text( TID_Type eType )
{
	const Store::stTextInfo* pTextInfo = TextReader::Instance()->Item( eType );
	return pTextInfo == NULL ? NULL :pTextInfo->pText;
}

#endif