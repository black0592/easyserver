#include "CountryManager.h"

using namespace easygame;


CountryManager::CountryManager(void)
{
}

CountryManager::~CountryManager(void)
{
}

bool CountryManager::loadConfig()
{
	pugi::xml_node node;
	pugi::xml_document doc;
	if (!doc.load_file("./config/countryinfo.xml")) {
		BLOGE("加载文件失败");
		return false;
	}

	release();

	// 先加载国家信息
	node = doc.child("Root").child("CountryInfo").child("Country");
	while (node) {
		string strID;
		string strName;
		string strBirthMap;
		string strBackToMap;
		strID = node.attribute("id").value();
		strName = node.attribute("name").value();
		strBirthMap = node.attribute("birthmap").value();
		strBackToMap = node.attribute("backto").value();
		
		Country* country = new Country;
		country->init(atoi(strID.c_str()), strName, strBirthMap, strBackToMap);
		addCountry(country);

		node = node.next_sibling();
	}


	// 加载地图信息
	vector<string> vecTemp;
	node = doc.child("Root").child("MapInfo").child("Map");
	while (node) {
		stMapInfo info;
		string strBackto;
		string strEnterLevel;
		info.id = atoi(node.attribute("id").value());
		info.name = node.attribute("name").value();
		info.logicMap = node.attribute("logicMap").value();
		info.blockMap = node.attribute("blockMap").value();
		info.flag = atoi(node.attribute("flag").value());
		
		strBackto = node.attribute("backto").value();
		vecTemp.clear();
		splitString(vecTemp, strBackto, ",");
		if (vecTemp.size() >= 3) {
			info.backtoMapId = atoi(vecTemp[0].c_str());
			info.backtoMapX = atoi(vecTemp[1].c_str());
			info.backtoMapY = atoi(vecTemp[2].c_str());
		}

		strEnterLevel = node.attribute("enterlevel").value();
		vecTemp.clear();
		splitString(vecTemp, strEnterLevel, "-");
		if (vecTemp.size() >= 2) {
			info.enterLevelMin = atoi(vecTemp[0].c_str());
			info.enterLevelMax = atoi(vecTemp[1].c_str());
		}

		mMapInfoHashMap[info.id] = info;

		node = node.next_sibling();
	}


	// 加载服务器的地图分布信息
	pugi::xml_node node2, node3;
	node = doc.child("Root").child("ServerInfo").child("Server");
	while (node) {
		stServerMapInfo info;

		info.serverId = atoi(node.attribute("id").value());
		node2 = node.child("Country");
		while (node2) {
			info.countryId = atoi(node2.attribute("id").value());

			node3 = node2.child("Map");
			while (node3) {
				info.mapId = atoi(node3.attribute("id").value());

				mServerMapInfo[info.mapId] = info;

				node3 = node3.next_sibling();
			}

			node2 = node2.next_sibling();
		}

		node = node.next_sibling();
	}

	return true;
}

// 通过临时ID来获取国家
Country* CountryManager::getCountry(const char* name)
{
	MutexScopeLock scopeLock(mMutex);

	return (Country*)getEntityByName(name);
}

// 通过服务器ID获取国家
Country* CountryManager::getCountry(uint id)
{
	MutexScopeLock scopeLock(mMutex);

	return (Country*)getEntityByID(id);
}

// 添加国家
bool CountryManager::addCountry(Country* pCountry)
{
	MutexScopeLock scopeLock(mMutex);

	if ( !this->addEntity(pCountry) ) {
		BLOGE("CountryManager::addCountry,加入管理器失败,id=%s", pCountry->getName());
		return false;
	}

	return true;
}

// 删除国家
bool CountryManager::removeCountry(Country* pCountry)
{
	MutexScopeLock scopeLock(mMutex);

	if ( !this->removeEntity(pCountry) ) {
		BLOGE("CountryManager::removeCountry,从管理器删除国家失败,id=%s", pCountry->getName());
		return false;
	}

	return true;
}

const CountryManager::stMapInfo* CountryManager::getMapInfo(uint mapId)
{
	map<uint,stMapInfo>::iterator itFind = mMapInfoHashMap.find(mapId);
	if (itFind == mMapInfoHashMap.end())
		return NULL;

	return &itFind->second;
}

const CountryManager::stServerMapInfo* CountryManager::getServerMapInfo(uint mapId)
{
	map<uint,stServerMapInfo>::iterator itFind = mServerMapInfo.find(mapId);
	if (itFind == mServerMapInfo.end())
		return NULL;

	return &itFind->second;
}
