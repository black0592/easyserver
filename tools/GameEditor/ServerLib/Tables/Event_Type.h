
#ifndef __Event_Type_H__
#define __Event_Type_H__

#include "Event_Type.h"

enum EventType
{
	Event_Level                         = 1,	//主角达到等级
	Event_ContinueLogin                 = 2,	//主角连续登录天数
	Event_EquipUpgrade                  = 3,	//装备强化到指定等级
	Event_EquipCount                    = 4,	//指定品质的装备达到指定数量
	Event_StudyLevelSkill               = 5,	//学习一定等级的技能
	Event_Capacity                      = 6,	//达到指定实力值
	Event_KillEnemy                     = 7,	//消灭敌对阵营的玩家数量
	Event_VectoryBattle                 = 8,	//竞技场或战场胜利的次数
	Event_CompleteDungeon               = 9,	//通过指定的普通副本
	Event_FriendCount                   = 10,	//拥有一定数量的好友
	Event_GuildContribute               = 11,	//公会累积共享达到一定数量
	Event_Recharge                      = 12,	//游戏充值
	Event_ChangeDay                     = 13,	//非当前天数时触发，在线或上线触发
	Event_ChangeMap                     = 14,	//切换地图
	Event_UserOnline                    = 15,	//玩家上线
	Event_UserOffline                   = 16,	//玩家下线
	Event_CompleteMission               = 17,	//完成指定id的任务
	Event_PromptMail                    = 18,	//邮件提醒
	Event_PromptActivity                = 19,	//活动提醒
	Event_PromptAchievement             = 20,	//成就提醒
	Event_PromptGuildApplyFor           = 21,	//公会申请提醒
	Event_PromptLevelupPoint            = 22,	//升级点数提醒
	Event_FirstEnterGame                = 23,	//新建角色后第一次进游戏
	Event_SecondTimer                   = 24,	//每秒执行一次
	Event_JoinGuild                     = 25,	//加入公会
	Event_Marriage                      = 26,	//拥有配偶
};

#endif