
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
	TID_DATA_LEVEL_NOT_ENOUGH           = 6,	//���ĵȼ�����
	TID_SYSTEM_NAME                     = 7,	//ϵͳ
	TID_AUCTION_NAME                    = 8,	//������
	TID_MAIL_NOT_EXIST_NAME             = 9,	//�ռ��˲�����
	TID_MAIL_HAS_NEWMAIL                = 10,	//�����µ��ʼ�����ע�����
	TID_MAIL_NOT_WRITE_SELF             = 11,	//���ܸ��Լ����ʼ�
	TID_USER_NOT_ONLINE                 = 12,	//����Ҳ�����
	TID_FRIEND_ALREADY_ADDED            = 13,	//������Ѿ������ĺ���
	TID_TEAM_ALREADY_HAVE_TEAM          = 14,	//������Ѿ��ж�����
	TID_TEAM_INVITE_NOT_MASTER          = 15,	//ֻ�жӳ���������������
	TID_TEAM_INVITE_NOT_AGREE           = 16,	//�Է��ܾ�������������
	TID_TEAM_JOIN_SUCCESS               = 17,	//�������� %s �Ķ���
	TID_TEAM_JOIN_USER                  = 18,	//��� %s ���������Ķ���
	TID_MARRY_NOT_ONLINE                = 19,	//��ȷ��˫������ͬһ����
	TID_UNMARRY_NOT_ONLINE              = 20,	//˫��������ͬһ����������Э�����
	TID_UNMARRY_NOT_MARRY               = 21,	//����δ��飬�޷����
	TID_DUP_VOTE_NOT_MASTER             = 22,	//ֻ�жӳ����ܷ�����븱����ͶƱ
	TID_DUP_VOTE                        = 23,	//�ѷ�����븱����ͶƱ����ȴ������Ӧ
	TID_DUP_MEMBER_LEVEL_ERROR          = 24,	//��Ա%s�ȼ��������޷����븱��
	TID_DUP_VOTE_NOT_AGREE              = 25,	//��Ա%s�ܾ����븱��
	TID_GUILD_CREATE_LEVEL_ERROR        = 26,	//�ȼ��������޷���������
	TID_GUILD_CREATE_NAME_ERROR         = 27,	//��������ʧ�ܣ���Ч�Ĺ�����
	TID_GUILD_JOIN_LEVEL_ERROR          = 28,	//�ȼ��������޷����빫��
	TID_GUILD_ALREADY_IN_THIS           = 29,	//�Ѿ��ڹ�������
	TID_GUILD_ALREADY_IN_OTHER          = 30,	//�Է��Ѿ���������������
	TID_GUILD_MEMBER_FULL               = 31,	//������������
	TID_GUILD_INVITE_NOT_AGREE          = 32,	//�Է��ܾ�����Ĺ�������
	TID_GUILD_ALREADY_APPLY             = 33,	//���Ѿ�������ˣ�����������
	TID_GUILD_MASTER_LEAVE_ERROR        = 34,	//��ת���᳤ְ������뿪�л�
	TID_GUILD_INIT_NOTICE               = 35,	//��ӭ���롾%s������
	TID_GUILD_MANAGER_FULL              = 36,	//����Ա��������
	TID_AUCTION_NOT_EXIST               = 37,	//�˼�����Ʒ�Ѳ���
	TID_AUCTION_NOT_BUY_SELFT           = 38,	//���ܹ����Լ����۵���Ʒ
	TID_AUCTION_MONEY_EXPIRED           = 39,	//�����۵�%d���δ���۳����뼰ʱ��ȡ�����з����Ľ��
	TID_AUCTION_ITEM_EXPIRED            = 40,	//�����۵���Ʒ��%s��δ���۳����뼰ʱ��ȡ�����з�������Ʒ
	TID_AUCTION_BACKOUT_MONEY           = 41,	//�����۵�%d��Ҵ��������¼ܣ��뼰ʱ��ȡ�����з����Ľ��
	TID_AUCTION_BACKOUT_ITEM            = 42,	//�����۵���Ʒ��%s�����������¼ܣ��뼰ʱ��ȡ�����з�������Ʒ
	TID_AUCTION_REQUIRE_GOLD_MIN        = 43,	//���ۼ۸���С��%u����ʯ
	TID_AUCTION_UNIMONEY_MIN            = 44,	//���۵��۲���С��%u���
	TID_AUCTION_SELL_ITEM_MAIL          = 45,	//�����۵���Ʒ��%s�����۳����۳�%u%%�Ľ���˰�������%u��ң��뼰ʱ��ȡ����
	TID_AUCTION_SELL_MONEY_MAIL         = 46,	//�����۵�%d������۳����۳�%u%%�Ľ���˰�������%u��ʯ���뼰ʱ��ȡ����
	TID_CHATTING_ITEM_NOT_EXIST         = 47,	//����Ϣ�Ѳ�����
};

static const char* TID_Text( TID_Type eType )
{
	const Store::stTextInfo* pTextInfo = TextReader::Instance()->Item( eType );
	return pTextInfo == NULL ? NULL :pTextInfo->pText;
}

#endif