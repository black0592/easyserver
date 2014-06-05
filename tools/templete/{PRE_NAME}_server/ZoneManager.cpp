#include "SuperHeader.h"
#include "ZoneManager.h"

ZoneManager::ZoneManager()
{

}

ZoneManager::~ZoneManager()
{

}

bool ZoneManager::addZoneServer(Server* server)
{
	if (server->getType() != ZoneServer)
		return false;

	ServerInfo info;
	server->getServerInfo(info);
	mZoneList.push_back(info);

	// 同步到登陆服务器
	refreshZoneInfoToLogin();

	return true;
}

void ZoneManager::removeZoneServer(Server* server)
{
	if (server->getType() != ZoneServer)
		return;

	bool removeResult = false;
	for (ServerInfoIter it=mZoneList.begin(); it != mZoneList.end(); it++) {
		ServerInfo& info = *it;
		if (info.uniqueid() == server->getLongID()) {
			mZoneList.erase( it );
			removeResult = true;
			break;
		}
	}

	// 同步到登陆服务器
	if (removeResult)
		refreshZoneInfoToLogin();
}

void ZoneManager::refreshZoneInfoToLogin()
{
	using namespace SuperCmd;

	NotifyRefreshZoneInfo cmd;
	for (size_t i=0; i<mZoneList.size(); i++) {
		ServerInfo& info = mZoneList[i];
		ServerInfo* pServerInfo = cmd.add_server_list();
		*pServerInfo = info;
	}

	ServerManager::getInstance().broadcastProtoMsgByType(LoginServer, cmd, CMD_SUPER_SERVICE(SUPER_OP_REFRESH_ZONE_INFO));
}