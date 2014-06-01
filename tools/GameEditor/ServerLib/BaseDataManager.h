#pragma once
#include "BaseData.h"

//DataManager<stMonBase>& getMonBaseMgr();
DataManager<stCharExpBase>& getCharExpBaseMgr();
DataManager<stJobExpBase>& getJobExpBaseMgr();
//DataManager<stMapInfo>& getMapInfoMgr();
//DataManager<stItemBase>& getItemBaseMgr();
DataManager<stGMItemBase>& getGMItemBaseMgr();
DataManager<stPetInfo>& getPetInfoMgr();
DataManager<stWingInfo>& getWingInfoMgr();
//DataManager<stLuminosityBase>& getLuminosityBaseMgr();
//DataManager<stSkillBase>& getSkillBaseMgr();
DataManager<stMissionBase>& getMissionBaseMgr();
//DataManager<stCashItemBase>& getCashItemBaseMgr();
DataManager<stChristmasItemBase>& getChristmasItemBaseMgr();
//DataManager<stNpcBase>& getNpcBaseMgr();
DataManager<stChatFilter>& getChatFilterBaseMgr();
DataManager<stMonsterInbreak>& getMonsterInbreakBaseMgr();

//////////////////////////////////////////////////////////////////////////

namespace GameTable
{
	const stItemBase* getItemBase(uint id);
	void getAllItemBase(vector<stItemBase*>& list);

	const stEquipBase* getEquipBase(uint id);
	void getAllEquipBase(vector<stEquipBase*>& list);

	const stMapInfo* getMapInfo(uint id);
	void getAllMapInfo(vector<stMapInfo*>& list);

	const stNpcBase* getNpcBase(uint id);
	void getAllNpcBase(vector<stNpcBase*>& list);

	const stMonBase* getMonBase(uint id);
	void getAllMonBase(vector<stMonBase*>& list);

	const stCashItemBase* getCashItemBase(uint id);
	void getAllCashItemBase(vector<stCashItemBase*>& list);

	const stSkillBase* getSkillBase(uint skillId, uint skillLevel = 1);
	void getAllSkillBase(vector<stSkillBase*>& list);

	const ShopInfo* getShopInfo(uint id);
	void getAllShopInfo(vector<ShopInfo*>& list);

	const DropItem* getDropItem(uint id);
	void getAllDropItem(vector<DropItem*>& list);
}	// namespace



//////////////////////////////////////////////////////////////////////////

extern bool loadAllBaseData();