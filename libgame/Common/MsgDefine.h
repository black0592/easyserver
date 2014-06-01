#ifndef __MSG_DEFINE_H__
#define __MSG_DEFINE_H__
#include "ProtoMsgType.pb.h"
using namespace Protocol;

// 登录消息
#define CMD_LOGIN(op)			MSG(MSG_LOGIN, op)

// 角色自己的消息
#define CMD_SELECTCHAR(op)		MSG(MSG_SELECTCHAR, op)

// 角色自己的消息
#define CMD_CHARDATA(op)		MSG(MSG_CHARDATA, op)

// 场景其他玩家有关联的消息
#define CMD_SCENEDATA(op)		MSG(MSG_GAMEDATA, op)

// 物品相关
#define CMD_ITEM(op)			MSG(MSG_ITEM, op)

// 交易
#define CMD_TRADE(op)			MSG(MSG_TRADE, op)

// 组队
#define CMD_TEAM(op)			MSG(MSG_TEAM, op)

// 攻城
#define CMD_SIEGEWAR(op)		MSG(MSG_SIEGEWAR, op)

// 摆摊
#define CMD_SELFSTORE(op)		MSG(MSG_SELFSTORE, op)

// 任务
#define CMD_MISSION(op)			MSG(MSG_MISSION, op)

// 公会
#define CMD_GUILD(op)			MSG(MSG_GUILD, op)

// 战斗
#define CMD_FIGHT(op)			MSG(MSG_FIGHT, op)

// 邮件相关
#define CMD_MAIL(op)			MSG(MSG_MAIL, op)

// 技能相关
#define CMD_SKILL(op)			MSG(MSG_SKILL, op)

// 社会关系
#define CMD_RELATIONS(op)		MSG(MSG_RELATIONS, op)

// 脚本相关
#define CMD_SCRIPT(op)			MSG(MSG_SCRIPT, op)

// 排行榜
#define CMD_TOPLIST(op)			MSG(MSG_TOPLIST, op)

// 装备强化
#define CMD_EQUIPUPGRADE(op)	MSG(MSG_EQUIPUPGRADE, op)

// 拍卖行
#define CMD_AUCTION(op)			MSG(MSG_AUCTION, op)

// 聊天
#define CMD_CHATTING(op)		MSG(MSG_CHATTING, op)

// 成就系统
#define CMD_ACHIEVEMENT(op)		MSG(MSG_ACHIEVEMENT, op)

// 活动系统
#define CMD_ACTIVITY(op)		MSG(MSG_ACTIVITY, op)

// 在线时长奖励
#define CMD_ONLINEREWARDS(op)	MSG(MSG_ONLINE_REWARDS, op)


// 其他通用(错误通知、游戏参数)
#define CMD_GENERAL(op)			MSG(MSG_GENERAL, op)


#endif



