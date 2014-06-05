#ifndef __ZONE_MANAGER_H__
#define __ZONE_MANAGER_H__

class ZoneManager : public Singleton<ZoneManager>
{
	typedef vector<ServerInfo> ServerInfoList;
	typedef ServerInfoList::iterator ServerInfoIter;

public:
	ZoneManager();
	virtual ~ZoneManager();

	bool addZoneServer(Server* server);
	void removeZoneServer(Server* server);

	// 刷新区服务器信息到登陆服务器
	void refreshZoneInfoToLogin();

private:
	ServerInfoList mZoneList;
};


#endif