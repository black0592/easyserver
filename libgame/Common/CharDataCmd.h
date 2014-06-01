#ifndef __CHARDATA_CMD_H__
#define __CHARDATA_CMD_H__
#include "BaseCmd.h"
#include "BaseStruct.h"
#include "Define.h"
#include "CharData.h"

namespace Protocol
{

#pragma pack(1)


	// �˹��ܿ�Ľṹ����
	struct stCharDataBaseCmd : public stBaseCmd
	{
		stCharDataBaseCmd(para_type para) : stBaseCmd(USERCMD_CHARDATA, para)
		{
		}
	};


	//////////////////////////////////////////////////////////////////////////



	// �ͻ��˷��ͳ����ƶ���Ϣ
	const para_type GAME_CMD_PETMOVE = 6;	// c -> s, s -> c
	struct stPetMove : stCharDataBaseCmd
	{
		uint id;
		ushort x;
		ushort y;
		stPetMove() : stCharDataBaseCmd(GAME_CMD_PETMOVE)
		{
			id = 0;
			x = y = 0;
		}
	};


	// �ͻ��˷��ͳ����ƶ�ֹͣ����Ϣ
	const para_type GAME_CMD_PETMOVE_EX = 7;	// c -> s, s -> c
	struct stPetMoveEx : stCharDataBaseCmd
	{
		uint id;
		ushort x;
		ushort y;
		stPetMoveEx() : stCharDataBaseCmd(GAME_CMD_PETMOVE_EX)
		{
			id = 0;
			x = y = 0;
		}
	};


	// ע����ҹ���Ȩ
	const para_type GAME_CMD_REGIST_CONTRY = 141;	// c -> s, s -> c
	struct stRegistCountry : stCharDataBaseCmd
	{
		stPkgPos srcPos;
		ushort itemId;
		ushort country;
		char charName[13];
		byte result;

		stRegistCountry() : stCharDataBaseCmd(GAME_CMD_REGIST_CONTRY)
		{

		}
	};


	// ��ʼְҵ����
	const para_type GAME_CMD_START_JOB_SKILL = 99;
	struct stStartJobSkill : stCharDataBaseCmd
	{
		byte level;
		byte jobProb;

		ushort targetId;
		ushort job;
		ushort state;
		byte result;

		stStartJobSkill() : stCharDataBaseCmd(GAME_CMD_START_JOB_SKILL)
		{

		}
	};


	// �ƶ���Ʒ��ְҵ������
	const para_type GAME_CMD_MOVEITEM_TO_JOBPACK = 101;
	struct stMoveItemToJobPack : stCharDataBaseCmd
	{
		ushort targetId;
		ushort index;
		stPkgPos srcPos;
		ItemData itemData;
		byte result;

		stMoveItemToJobPack() : stCharDataBaseCmd(GAME_CMD_MOVEITEM_TO_JOBPACK)
		{

		}
	};

	// ʹ��ְҵ����
	const para_type GAME_CMD_USE_JOB_SKILL = 103;
	struct stUseJobSkill : stCharDataBaseCmd
	{
		ushort targetId;
		uint money;
		ushort skillId[3];
		stItemEx ItemEx[2];
		ushort curJP;
		byte result;  // 0 - XX , 1 - XX , 2 - JP����, 3 - XX, 4 - 2+3

		stUseJobSkill() : stCharDataBaseCmd(GAME_CMD_USE_JOB_SKILL)
		{

		}

		uint getSize() { return sizeof(stUseJobSkill); }
	};

	// �ر�ְҵ���ܽ���
	const para_type GAME_CMD_CLOSE_JOBPACK = 105;
	struct stCloseJobPack : stCharDataBaseCmd
	{
		ushort TargetId;
		byte result;

		stCloseJobPack() : stCharDataBaseCmd(GAME_CMD_CLOSE_JOBPACK)
		{

		}
	};
	
	// �����ǳƺ��ǳ���ɫ
	const para_type GAME_CMD_NICK_SETTING = 144;
	struct stNickSetting : stCharDataBaseCmd
	{
		ushort targetID;
		char nickName[CHARNAME_LEN+1];
		byte nickColor;
		byte result;

		stNickSetting() : stCharDataBaseCmd(GAME_CMD_NICK_SETTING)
		{
			bzero(nickName,sizeof(nickName));
			targetID = 0;
			nickColor = 0;
			result = 0;
		}
	};


	/*
	// ���а��µ����
	const para_type GAME_CMD_CALL_MYDEAR = 100;
	struct stCallMyDear : stCharDataBaseCmd
	{
		byte type;					// 0 - ���봫��, 1 - ������
		char name[CHARNAME_LEN+1];	// �����ߵ�����
		//byte agree;				// ֻ��type==1ʱ��Ч, agree==0��ʾ��ͬ�⣬agree==1��ʾͬ��
		stCallMyDear() : stCharDataBaseCmd(GAME_CMD_CALL_MYDEAR)
		{
			type = 0;
			bzero(name,sizeof(name));
			//agree = 0;
		}
	};


	// ������
	const para_type GAME_CMD_MARRIAGE = 109;
	struct stMarriage : stCharDataBaseCmd
	{
		ushort targetId;
		stPkgPos pos[2];
		byte result;

		stMarriage() : stCharDataBaseCmd(GAME_CMD_MARRIAGE)
		{
			targetId = 0;
			result = 0;
		}
	};

	// ���Ľ��
	const para_type GAME_CMD_MARRIAGE_RESULT = 110;
	struct stMarriageResult : stCharDataBaseCmd
	{
		ushort targetId;
		stPkgPos pos[2];
		byte result;

		stMarriageResult() : stCharDataBaseCmd(GAME_CMD_MARRIAGE_RESULT)
		{
			targetId = 0;
			result = 0;
		}
	};


	// �������
	const para_type GAME_CMD_UNMARRY	 = 111;
	struct stUnmarry : stCharDataBaseCmd
	{
		ushort targetId;
		stPkgPos pos[2];
		byte result;

		stUnmarry() : stCharDataBaseCmd(GAME_CMD_UNMARRY)
		{
			targetId = 0;
			result = 0;
		}
	};

	// ���Ľ��
	const para_type GAME_CMD_UNMARRY_RESULT = 112;
	struct stUnmarryResult : stCharDataBaseCmd
	{
		uint id;
		ushort targetId;
		stPkgPos pos[2];
		byte result;

		stUnmarryResult() : stCharDataBaseCmd(GAME_CMD_UNMARRY_RESULT)
		{
			id = 0;
			targetId = 0;
			result = 0;
		}
	};
	*/


#pragma pack()

};	// namespace


#endif