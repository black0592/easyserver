
#ifndef __Activity_Type_H__
#define __Activity_Type_H__

#include "Activity_Type.h"

enum class ActivityType
{
	LoginAward                          = 1,	//登录领奖
	FastDungeon                         = 2,	//急速副本
	ChargeRebate                        = 3,	//充值返利
	SoloFight                           = 4,	//单人竞技
	TeamFight                           = 5,	//队伍竞技
	GuildFight                          = 6,	//公会争霸
	CountryFight                        = 7,	//阵营战场
	GuildMineralDungeon                 = 8,	//公会矿石副本
	GuildCrystalDungeon                 = 9,	//公会晶体副本
	DailyUpgradeMission                 = 10,	//日常历练任务
	DailyGuildMission                   = 11,	//日常公会任务
	DailyPacifyMission                  = 12,	//日常平定任务
};

#endif