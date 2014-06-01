#ifndef __COUNTRY_MANAGER_H__
#define __COUNTRY_MANAGER_H__

#include "Country.h"

class CountryManager : public Singleton<CountryManager>, public EntityManager<EntityManagerID,EntityManagerName>
{
public:
	struct stMapInfo
	{
		uint id;
		string name;
		string logicMap;	// 逻辑地图文件
		string blockMap;	// 阻挡地图文件
		uint backtoMapId;	// 回城地图信息
		uint backtoMapX;
		uint backtoMapY;
		uint flag;			// 地图标志
		uint enterLevelMin;	// 进入等级限制下限
		uint enterLevelMax;	// 进入等级限制上限
	};

	// 服务器下的地图信息
	struct stServerMapInfo
	{
		uint serverId;		// 地图服务器编号
		uint countryId;		// 国家编号
		uint mapId;			// 地图编号
	};

public:

	CountryManager(void);
	virtual ~CountryManager(void);

	bool loadConfig();

	// 通过临时ID来获取国家
	Country* getCountry(const char* name);

	// 通过服务器ID获取国家
	Country* getCountry(uint id);

	// 添加国家
	bool addCountry(Country* pCountry);

	// 删除国家
	bool removeCountry(Country* pCountry);

	// -------------- 地图信息 -------------
	const CountryManager::stMapInfo* getMapInfo(uint mapId);

	// -------------- 服务器分布的地图信息 -------------
	const CountryManager::stServerMapInfo* getServerMapInfo(uint mapId);


private:
	easygame::Mutex mMutex;
	map<uint,stMapInfo> mMapInfoHashMap;
	map<uint,stServerMapInfo> mServerMapInfo;
};



#endif