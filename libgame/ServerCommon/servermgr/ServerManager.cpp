#include "ServerManager.h"

using namespace easygame;



ServerSortList::ServerSortList()
{

}

ServerSortList::~ServerSortList()
{

}

bool ServerSortList::empty()
{
	return mList.empty();
}

void ServerSortList::push(Server* pServer)
{
	mList.push_back(pServer);
	sort();
}

Server* ServerSortList::top()
{
	if (mList.empty())
		return NULL;

	return mList[mList.size()-1];
}

void ServerSortList::pop()
{
	mList.pop_back();
}

bool ServerSortList::remove(Server* pServer)
{
	for (vector<Server*>::iterator it=mList.begin(); it!=mList.end(); it++) {
		if (*it == pServer) {
			mList.erase(it);
			sort();
			return true;
		}
	}

	return false;
}

bool sortServer(Server* a, Server* b)
{
	//if (a->getOnlineNum() != b->getOnlineNum())
	//	return a->getOnlineNum() < b->getOnlineNum();
	return a->getOnlineNum() > b->getOnlineNum();
}

void ServerSortList::sort()
{
	std::sort( mList.begin(), mList.end(), sortServer );
}

//////////////////////////////////////////////////////////////////////////

ServerManager::ServerManager(void)
{
	mServerList.resize(MaxServerType);
}

ServerManager::~ServerManager(void)
{
}

// 通过临时ID来获取服务器
Server* ServerManager::getServerByUniqueId(uint64 longId)
{
	MutexScopeLock scopeLock(mMutex);

	return (Server*)getEntityByLongID(longId);
}


// 删除服务器
bool ServerManager::removeServer(Server* pServer)
{
	MutexScopeLock scopeLock(mMutex);

	if (pServer == NULL)
		return false;

	ServerType svrType = pServer->getType();
	if (!isValidServerType(svrType)) {
		BLOGE("ServerManager::removeServer,无效的服务器类型%u", pServer->getType());
		return false;
	}

	ServerSortList* pSortList = &mServerList[svrType];
	pSortList->remove(pServer);

	if ( !this->removeEntity(pServer) )
		return false;
	
	return true;
}

// 添加服务器
bool ServerManager::addServer(Server* pServer)
{
	MutexScopeLock scopeLock(mMutex);

	if (pServer == NULL)
		return false;

	Server* pTempServer = (Server*)getEntityByLongID(pServer->getLongID());
	if (pTempServer != NULL) {
		BLOGE("ServerManager::addServer,此服务器已注册，无法重复注册%u", pServer->getType());
		return false;
	}

	ServerType svrType = pServer->getType();
	if (!isValidServerType(svrType)) {
		BLOGE("ServerManager::addServer,无效的服务器类型%u", pServer->getType());
		return false;
	}

	mServerList[svrType].push(pServer);

	if ( !this->addEntity(pServer) ) {
		BLOGE("ServerManager::addServer,加入管理器失败,类型%u", pServer->getType());
		return false;
	}

	return true;
}

void ServerManager::updateServerInfo(uint64 serverUID, uint onlineNum, ServerState state)
{
	Server* pServer = getServerByUniqueId(serverUID);
	if (pServer == NULL)
		return;

	if (pServer->getOnlineNum() == onlineNum)
		return;

	pServer->setOnlineNum(onlineNum);
	mServerList[pServer->getType()].sort();
}

void ServerManager::getListByType(ServerType type, vector<Server*>& vecServer)
{
	MutexScopeLock scopeLock(mMutex);

	Server* server = NULL;
	EntityManagerLongID::EntityIter it;
	for (it = EntityManagerLongID::m_entityMap.begin(); it!=EntityManagerLongID::m_entityMap.end(); it++) {
		server = (Server*)it->second;
		if (server->getType() == type) {
			vecServer.push_back(server);
		}
	}
}

Server* ServerManager::findFreeServerByType(ServerType type)
{
	MutexScopeLock scopeLock(mMutex);

	if (!isValidServerType(type)) {
		BLOGE("ServerManager::findFreeServerByType,无效的服务器类型%u", type);
		return NULL;
	}

	if (mServerList[type].empty())
		return NULL;

	return mServerList[type].top();
}

void ServerManager::broadcastProtoMsgByType(ServerType type, ProtoMessage& msg, uint cmdID)
{
	MutexScopeLock scopeLock(mMutex);
	
	if (!isValidServerType(type)) {
		BLOGE("ServerManager::broadcastProtoMsgByType,无效的服务器类型%u", type);
		return;
	}

	Server* server = NULL;
	EntityManagerLongID::EntityIter it;
	for (it = EntityManagerLongID::m_entityMap.begin(); it!=EntityManagerLongID::m_entityMap.end(); it++) {
		server = (Server*)it->second;
		if (server->getType() == type) {
			server->sendProtoMsg(msg, cmdID);
		}
	}
}


void ServerManager::broadcastCmdByType(ServerType type, const void* cmd, ushort cmdLen)
{
	MutexScopeLock scopeLock(mMutex);

	if (!isValidServerType(type)) {
		BLOGE("ServerManager::broadcastCmdByType,无效的服务器类型%u", type);
		return;
	}

	Server* server = NULL;
	EntityManagerLongID::EntityIter it;
	for (it = EntityManagerLongID::m_entityMap.begin(); it!=EntityManagerLongID::m_entityMap.end(); it++) {
		server = (Server*)it->second;
		if (server->getType() == type) {
			server->sendCmd(cmd, cmdLen);
		}
	}
}








