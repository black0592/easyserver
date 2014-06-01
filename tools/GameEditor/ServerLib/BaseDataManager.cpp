#include "stdafx.h"
#include "BasedataManager.h"

#include "ItemTable.h"
#include "MapTable.h"

#define TBL_PATH	"old_tables"

//DataManager<stMonBase> monstBaseManager;
DataManager<stCharExpBase> charExpBaseManager;
DataManager<stJobExpBase> jobExpBaseManager;
//DataManager<stMapInfo> mapInfoManager;
//DataManager<stItemBase> itemBaseManager;
DataManager<stGMItemBase> gmItemBaseManager;
DataManager<stPetInfo> petInfoManager;
DataManager<stWingInfo> wingInfoManager;
//DataManager<stLuminosityBase> luminosityBaseManager;
//DataManager<stSkillBase> skillBaseManager;
DataManager<stMissionBase> missionBaseManager;
DataManager<stCashItemBase> cashItemBaseManager;
DataManager<stChristmasItemBase> christmasItemBaseManager;
//DataManager<stNpcBase> npcBaseManager;
DataManager<stChatFilter> chatFilterManager;
DataManager<stMonsterInbreak> monsterInbreakManager;



//DataManager<stMonBase>& getMonBaseMgr()
//{
//	return monstBaseManager;
//}

DataManager<stCharExpBase>& getCharExpBaseMgr()
{
	return charExpBaseManager;
}

DataManager<stJobExpBase>& getJobExpBaseMgr()
{
	return jobExpBaseManager;
}

//DataManager<stMapInfo>& getMapInfoMgr()
//{
//	return mapInfoManager;
//}
//
//DataManager<stItemBase>& getItemBaseMgr()
//{
//	return itemBaseManager;
//}

DataManager<stGMItemBase>& getGMItemBaseMgr()
{
	return gmItemBaseManager;
}

DataManager<stPetInfo>& getPetInfoMgr()
{
	return petInfoManager;
}

DataManager<stWingInfo>& getWingInfoMgr()
{
	return wingInfoManager;
}

//DataManager<stLuminosityBase>& getLuminosityBaseMgr()
//{
//	return luminosityBaseManager;
//}

//DataManager<stSkillBase>& getSkillBaseMgr()
//{
//	return skillBaseManager;
//}

DataManager<stMissionBase>& getMissionBaseMgr()
{
	return missionBaseManager;
}

//DataManager<stCashItemBase>& getCashItemBaseMgr()
//{
//	return cashItemBaseManager;
//}

DataManager<stChristmasItemBase>& getChristmasItemBaseMgr()
{
	return christmasItemBaseManager;
}

//DataManager<stNpcBase>& getNpcBaseMgr()
//{
//	return npcBaseManager;
//}

DataManager<stChatFilter>& getChatFilterBaseMgr()
{
	return chatFilterManager;
}

DataManager<stMonsterInbreak>& getMonsterInbreakBaseMgr()
{
	return monsterInbreakManager;
}




template <class BaseMgr>
bool loadBaseFile(BaseMgr& baseMgr, string& filename)
{
	File file;
	if (!file.open(filename.c_str()))
		return false;

	if (!baseMgr.load(file.getBufferPtr(), file.getLength())) {
		return false;
	}

	return true;
}


bool loadAllBaseData()
{
	/*
	string strFilename = "";

	strFilename = EditorConfig::Instance()->makeServerResPath(TBL_PATH);
	strFilename += "/charexp.tbl";
	if ( !loadBaseFile(charExpBaseManager, strFilename) ) {
		LOGE("加载角色经验基础表格失败");
		return false;
	}

	strFilename = EditorConfig::Instance()->makeServerResPath(TBL_PATH);
	strFilename += "/petinfo.tbl";
	if ( !loadBaseFile(petInfoManager, strFilename) ) {
		LOGE("加载宠物信息表失败");
		return false;
	}

	strFilename = EditorConfig::Instance()->makeServerResPath(TBL_PATH);
	strFilename += "/winginfo.tbl";
	if ( !loadBaseFile(wingInfoManager, strFilename) ) {
		LOGE("加载翅膀信息表失败");
		return false;
	}


	strFilename = EditorConfig::Instance()->makeServerResPath(TBL_PATH);
	strFilename += "/cashitem.tbl";
	if ( !loadBaseFile(cashItemBaseManager, strFilename) ) {
		LOGE("加载商城物品表失败");
		return false;
	}

	strFilename = EditorConfig::Instance()->makeServerResPath(TBL_PATH);
	strFilename += "/christmas.tbl";
	if ( !loadBaseFile(christmasItemBaseManager, strFilename) ) {
		LOGE("加载圣诞活动物品表失败");
		return false;
	}

	strFilename = EditorConfig::Instance()->makeServerResPath(TBL_PATH);
	strFilename += "/chatfilter.tbl";
	if ( !loadBaseFile(chatFilterManager, strFilename) ) {
		LOGE("加载过滤字表失败");
		return false;
	}
	*/

	return true;
}


namespace GameTable
{

	//////////////////////////////////////////////////////////////////////////

	const stItemBase* getItemBase(uint id)
	{
		return ItemReader::Instance()->Item(id);
	}

	void getAllItemBase(vector<stItemBase*>& list)
	{
		ItemReader::Instance()->GetAllItem( list );
	}

	//////////////////////////////////////////////////////////////////////////

	const stEquipBase* getEquipBase(uint id)
	{
		return EquipmentReader::Instance()->Item(id);
	}

	void getAllEquipBase(vector<stEquipBase*>& list)
	{
		EquipmentReader::Instance()->GetAllItem( list );
	}

	//////////////////////////////////////////////////////////////////////////

	const stMapInfo* getMapInfo(uint id)
	{
		return MapReader::Instance()->Item(id);
	}

	void getAllMapInfo(vector<stMapInfo*>& list)
	{
		MapReader::Instance()->GetAllItem( list );
	}

	//////////////////////////////////////////////////////////////////////////

	const stNpcBase* getNpcBase(uint id)
	{
		return NpcReader::Instance()->Item(id);
	}

	void getAllNpcBase(vector<stNpcBase*>& list)
	{
		NpcReader::Instance()->GetAllItem( list );
	}


	//////////////////////////////////////////////////////////////////////////

	const stMonBase* getMonBase(uint id)
	{
		return MonsterReader::Instance()->Item(id);
	}

	void getAllMonBase(vector<stMonBase*>& list)
	{
		MonsterReader::Instance()->GetAllItem( list );
	}

	//////////////////////////////////////////////////////////////////////////

	const stCashItemBase* getCashItemBase(uint id)
	{
		return cashItemBaseManager.getData(id);
	}

	void getAllCashItemBase(vector<stCashItemBase*>& list)
	{
		cashItemBaseManager.getAllDataPtr(list);
	}

	//////////////////////////////////////////////////////////////////////////

	const stSkillBase* getSkillBase(uint skillId, uint skillLevel)
	{
		return SkillReader::Instance()->Item( SkillKey(skillId, skillLevel) );
	}

	void getAllSkillBase(vector<stSkillBase*>& list)
	{
		SkillReader::Instance()->GetAllItem( list );
	}

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	const ShopInfo* getShopInfo(uint id)
	{
		return ShopReader::Instance()->Item( id );
	}

	void getAllShopInfo(vector<ShopInfo*>& list)
	{
		ShopReader::Instance()->GetAllItem(list);
	}


	//////////////////////////////////////////////////////////////////////////

	const DropItem* getDropItem(uint id)
	{
		return DropItemReader::Instance()->Item( id );
	}

	void getAllDropItem(vector<DropItem*>& list)
	{
		DropItemReader::Instance()->GetAllItem(list);
	}

	//////////////////////////////////////////////////////////////////////////


}	// namespace