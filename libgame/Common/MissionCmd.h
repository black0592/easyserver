#ifndef __MISSION_CMD_H__
#define __MISSION_CMD_H__
#include "BaseCmd.h"
#include "BaseStruct.h"
#include "Define.h"

namespace Protocol
{

#pragma pack(1)


	// 此功能块的结构基类
	struct stMissionBaseCmd : public stBaseCmd
	{
		stMissionBaseCmd(para_type para) : stBaseCmd(22, para)
		{
		}
	};


	//////////////////////////////////////////////////////////////////////////



	// 洗职业技能任务
	const para_type GAME_CMD_REMOVE_JOB = 205;
	struct stRemoveJob : stMissionBaseCmd
	{
		stPkgPos pos;
		ushort itemId;
		byte result;

		stRemoveJob() : stMissionBaseCmd(GAME_CMD_REMOVE_JOB)
		{
			itemId = 0;
			result = 0;
		}
	};


	// npc菜单
	const para_type GAME_CMD_GET_NPCMENU = 106;
	struct stGetNpcMenu : stMissionBaseCmd
	{
		ushort npcid;
		ushort scriptId;

		stGetNpcMenu() : stMissionBaseCmd(GAME_CMD_GET_NPCMENU)
		{
			npcid = 0;
			scriptId = 0;
		}
	};


#define NPCMSGLINE 12

	struct NPCMSG
	{
		byte LineNum;
		char strmsg[NPCMSGLINE][64];

		NPCMSG()
		{
			LineNum = 0;
			bzero(strmsg,sizeof(strmsg));
		}
	};

	// npc菜单返回值
	const para_type GAME_CMD_GET_NPCMENU_RESULT = 120;
	struct stGetNpcMenuResult : stMissionBaseCmd
	{
		ushort npcid;
		byte dlgType;	// 0 - 文字框, 1 - 菜单框
		byte btnType;	// 按钮类型 0 - 确定，1 - 确定/取消
		NPCMSG npcMsg;	// 消息内容

		stGetNpcMenuResult() : stMissionBaseCmd(GAME_CMD_GET_NPCMENU_RESULT)
		{
			npcid = 0;
			dlgType = 0;
			btnType = 0;
		}
	};


	// 点击npc菜单
	const para_type GAME_CMD_CLICK_NPCMENU = 132;
	struct stClickNpcMenu : stMissionBaseCmd
	{
		ushort npcid;
		byte line;	// 选中的行号(244 - 确定, 255 - 取消)

		stClickNpcMenu() : stMissionBaseCmd(GAME_CMD_CLICK_NPCMENU)
		{
			npcid = 0;
			line = 0;
		}
	};





#pragma pack()

};	// namespace


#endif