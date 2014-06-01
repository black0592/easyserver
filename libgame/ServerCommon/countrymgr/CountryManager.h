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
		string logicMap;	// �߼���ͼ�ļ�
		string blockMap;	// �赲��ͼ�ļ�
		uint backtoMapId;	// �سǵ�ͼ��Ϣ
		uint backtoMapX;
		uint backtoMapY;
		uint flag;			// ��ͼ��־
		uint enterLevelMin;	// ����ȼ���������
		uint enterLevelMax;	// ����ȼ���������
	};

	// �������µĵ�ͼ��Ϣ
	struct stServerMapInfo
	{
		uint serverId;		// ��ͼ���������
		uint countryId;		// ���ұ��
		uint mapId;			// ��ͼ���
	};

public:

	CountryManager(void);
	virtual ~CountryManager(void);

	bool loadConfig();

	// ͨ����ʱID����ȡ����
	Country* getCountry(const char* name);

	// ͨ��������ID��ȡ����
	Country* getCountry(uint id);

	// ��ӹ���
	bool addCountry(Country* pCountry);

	// ɾ������
	bool removeCountry(Country* pCountry);

	// -------------- ��ͼ��Ϣ -------------
	const CountryManager::stMapInfo* getMapInfo(uint mapId);

	// -------------- �������ֲ��ĵ�ͼ��Ϣ -------------
	const CountryManager::stServerMapInfo* getServerMapInfo(uint mapId);


private:
	easygame::Mutex mMutex;
	map<uint,stMapInfo> mMapInfoHashMap;
	map<uint,stServerMapInfo> mServerMapInfo;
};



#endif