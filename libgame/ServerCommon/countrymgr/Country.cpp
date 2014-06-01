#include "Country.h"

Country::Country(void)
{
	mBirthMapId = 0;
	mBirthMapX = 0;
	mBirthMapY = 0;

	mBackToMapId = 0;
	mBackToMapX = 0;
	mBackToMapY = 0;
}

Country::~Country(void)
{

}

bool Country::init(uint id, const string& name, const string& birthmap, const string& backtomap)
{
	setID(id);
	setName(name.c_str());

	vector<string> vecBirthMap;
	splitString(vecBirthMap, birthmap, ",");
	if (vecBirthMap.size() < 3) return false;
	mBirthMapId = atoi(vecBirthMap[0].c_str());
	mBirthMapX = atoi(vecBirthMap[1].c_str());
	mBirthMapY = atoi(vecBirthMap[2].c_str());

	vector<string> vecBackToMap;
	splitString(vecBackToMap, backtomap, ",");
	if (vecBackToMap.size() < 3) return false;
	mBackToMapId = atoi(vecBackToMap[0].c_str());
	mBackToMapX = atoi(vecBackToMap[1].c_str());
	mBackToMapY = atoi(vecBackToMap[2].c_str());

	return true;
}

bool Country::isExistMap(uint mapId)
{
	return mMapIdList.find(mapId) != mMapIdList.end();
}

