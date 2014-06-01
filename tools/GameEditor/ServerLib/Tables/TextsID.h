
#ifndef __TextsID_TABLE_READER_H__
#define __TextsID_TABLE_READER_H__

#include "TextTable.h"

enum TID_Type
{
	TID_ITEM_BAG_FULL                   = 1,	//您的背包已满
	TID_ITEM_PICKUP_BAG_FULL            = 2,	//您的背包已满，无法捡取
	TID_ITEM_DROP_ITEM_EXIST            = 3,	//此位置已存在道具，走几步换个地方扔吧
	TID_DATA_MONEY_NOT_ENOUGH           = 4,	//您的金币不够
	TID_DATA_GOLD_NOT_ENOUGH            = 5,	//您的钻石不够
	TID_DATA_GULD_CREDITS_NOT_ENOUGH    = 6,	//您的公会贡献不够
	TID_DATA_LEVEL_NOT_ENOUGH           = 7,	//您的等级不够
	TID_SYSTEM_NAME                     = 8,	//系统
	TID_AUCTION_NAME                    = 9,	//拍卖行
	TID_MAIL_NOT_EXIST_NAME             = 10,	//收件人不存在
	TID_MAIL_HAS_NEWMAIL                = 11,	//您有新的邮件，请注意查收
	TID_MAIL_NOT_WRITE_SELF             = 12,	//不能给自己发邮件
	TID_USER_NOT_ONLINE                 = 13,	//该玩家不在线
	TID_FRIEND_ALREADY_ADDED            = 14,	//该玩家已经是您的好友
	TID_TEAM_ALREADY_HAVE_TEAM          = 15,	//该玩家已经有队伍了
	TID_TEAM_INVITE_NOT_MASTER          = 16,	//只有队长才能邀请玩家入队
	TID_TEAM_INVITE_NOT_AGREE           = 17,	//对方拒绝了你的组队邀请
	TID_TEAM_JOIN_SUCCESS               = 18,	//您加入了 %s 的队伍
	TID_TEAM_JOIN_USER                  = 19,	//玩家 %s 加入了您的队伍
	TID_MARRY_NOT_ONLINE                = 20,	//请确认双方都在同一场景
	TID_UNMARRY_NOT_ONLINE              = 21,	//双方必须在同一场景，才能协议离婚
	TID_UNMARRY_NOT_MARRY               = 22,	//您尚未结婚，无法离婚
	TID_DUP_VOTE_NOT_MASTER             = 23,	//只有队长才能发起进入副本的投票
	TID_DUP_VOTE                        = 24,	//已发起进入副本的投票，请等待队伍回应
	TID_DUP_MEMBER_LEVEL_ERROR          = 25,	//队员%s等级不够，无法进入副本
	TID_DUP_VOTE_NOT_AGREE              = 26,	//队员%s拒绝进入副本
	TID_GUILD_CREATE_LEVEL_ERROR        = 27,	//等级不够，无法创建公会
	TID_GUILD_CREATE_NAME_ERROR         = 28,	//创建公会失败，无效的公会名
	TID_GUILD_JOIN_LEVEL_ERROR          = 29,	//等级不够，无法加入公会
	TID_GUILD_ALREADY_IN_THIS           = 30,	//已经在公会中了
	TID_GUILD_ALREADY_IN_OTHER          = 31,	//对方已经在其他公会中了
	TID_GUILD_MEMBER_FULL               = 32,	//公会人数已满
	TID_GUILD_INVITE_NOT_AGREE          = 33,	//对方拒绝了你的公会邀请
	TID_GUILD_ALREADY_APPLY             = 34,	//您已经申请过了，不能再申请
	TID_GUILD_MASTER_LEAVE_ERROR        = 35,	//请转交会长职务后，再离开行会
	TID_GUILD_INIT_NOTICE               = 36,	//欢迎加入【%s】公会
	TID_GUILD_MANAGER_FULL              = 37,	//管理员人数已满
	TID_AUCTION_NOT_EXIST               = 38,	//此寄售商品已不在
	TID_AUCTION_NOT_BUY_SELFT           = 39,	//不能购买自己寄售的商品
	TID_AUCTION_MONEY_EXPIRED           = 40,	//您寄售的%d金币未能售出，请及时领取附件中返还的金币
	TID_AUCTION_ITEM_EXPIRED            = 41,	//您寄售的物品【%s】未能售出，请及时领取附件中返还的物品
	TID_AUCTION_BACKOUT_MONEY           = 42,	//您寄售的%d金币从拍卖行下架，请及时领取附件中返还的金币
	TID_AUCTION_BACKOUT_ITEM            = 43,	//您寄售的物品【%s】从拍卖行下架，请及时领取附件中返还的物品
	TID_AUCTION_REQUIRE_GOLD_MIN        = 44,	//寄售价格不能小于%u个钻石
	TID_AUCTION_UNIMONEY_MIN            = 45,	//寄售单价不能小于%u金币
	TID_AUCTION_SELL_ITEM_MAIL          = 46,	//您寄售的物品【%s】已售出，扣除%u%%的交易税后您获得%u金币，请及时领取附件
	TID_AUCTION_SELL_MONEY_MAIL         = 47,	//您寄售的%d金币已售出，扣除%u%%的交易税后您获得%u钻石，请及时领取附件
	TID_CHATTING_ITEM_NOT_EXIST         = 48,	//此信息已不存在
	TID_UPGRADE_EQUIP_SUCCESS_INFO      = 49,	//太厉害了，%s 把 %s 强化到了 %d 级
	TID_ITEM_SAFE_FULL                  = 50,	//您的仓库已满，无法存放
	TID_EQUIP_NOT_NEED_REPAIR           = 51,	//您的装备没有损坏，不需要修理
	TID_UPGRADE_EQUIP_SAME_STUFF        = 52,	//一件装备只能镶嵌一颗同类型的宝石
	TID_EQUIP_EXPIRED_ITEM              = 53,	//无法装备已过期的时装，请您先续费
};

static const char* TID_Text( TID_Type eType )
{
	const Store::stTextInfo* pTextInfo = TextReader::Instance()->Item( eType );
	return pTextInfo == NULL ? NULL :pTextInfo->pText;
}

#endif