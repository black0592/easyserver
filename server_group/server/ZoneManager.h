#pragma once

class ZoneManager : public Singleton<ZoneManager>
{
	typedef vector<ServerInfo> ServerInfoList;
	typedef ServerInfoList::iterator ServerInfoIter;

public:
	ZoneManager();
	virtual ~ZoneManager();

	bool addZoneServer(Server* server);
	void removeZoneServer(Server* server);

	// ˢ������������Ϣ����½������
	void refreshZoneInfoToLogin();

private:
	ServerInfoList mZoneList;
};

