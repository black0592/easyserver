#ifndef __COUNTRY_H__
#define __COUNTRY_H__

#include "BaseLib.h"
using namespace easygame;


class Country : public Entity
{
public:



public:
	Country(void);
	virtual ~Country(void);

	bool init(uint id, const string& name, const string& birthmap, const string& backtomap);

	uint getBirthMapId() { return mBirthMapId; }
	uint getBirthMapX() { return mBirthMapX; }
	uint getBirthMapY() { return mBirthMapY; }

	uint getBackToMapId() { return mBackToMapId; }
	uint getBackToMapX() { return mBackToMapX; }
	uint getBackToMapY() { return mBackToMapY; }


	bool isExistMap(uint mapId);

private:
	// ������Ϣ
	uint mBirthMapId;
	uint mBirthMapX;
	uint mBirthMapY;

	// ��ȫ�س���Ϣ
	uint mBackToMapId;
	uint mBackToMapX;
	uint mBackToMapY;


	map<uint,uint> mMapIdList;
};




#endif