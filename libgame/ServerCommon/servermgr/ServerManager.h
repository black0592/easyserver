#ifndef __SERVER_MANAGER_H__
#define __SERVER_MANAGER_H__

#include "Server.h"

// 服务器列表
class ServerSortList
{
public:
	ServerSortList();
	virtual ~ServerSortList();

	bool empty();

	Server* top();

	void push(Server* pServer);

	void pop();

	bool remove(Server* pServer);

	void sort();

protected:
private:
	vector<Server*> mList;
};

//////////////////////////////////////////////////////////////////////////

class ServerManager : public Singleton<ServerManager>, public EntityManager<EntityManagerLongID>
{
public:

	ServerManager(void);
	virtual ~ServerManager(void);

	// 通过临时ID来获取服务器
	Server* getServerByUniqueId(uint64 longId);

	// 删除服务器
	bool removeServer(Server* pServer);

	// 添加服务器
	bool addServer(Server* pServer);

	// 更新服务器状态
	void updateServerInfo(uint64 serverUID, uint onlineNum, ServerState state);

	// 通过类型获得服务器
	void getListByType(ServerType type, vector<Server*>& vecServer);

	// 广播消息给指定类型的服务器
	void broadcastCmdByType(ServerType type, const void* cmd, ushort cmdLen);

	// 广播消息给指定类型的服务器
	void broadcastProtoMsgByType(ServerType type, ProtoMessage& msg, uint cmdID = 0);

	// 查找一个指定类型的空闲的服务器
	Server* findFreeServerByType(ServerType type);

	/*
	// 查找一个空闲的网关服务器
	Server* findFreeGatewayServer() { return findFreeServerByType(GatewayServer); }

	// 查找一个空闲的登陆服务器
	Server* findFreeLoginServer() { return findFreeServerByType(LoginServer); }

	// 查找一个空闲的游戏服务器
	Server* findFreeGameServer() { return findFreeServerByType(GameServer); }

	// 查找一个空闲的Session服务器
	Server* findFreeSessionServer() { return findFreeServerByType(SessionServer); }

	// 查找一个空闲的LoginDB服务器
	Server* findFreeLoginDBServer() { return findFreeServerByType(LoginDBServer); }

	// 查找一个空闲的GameDB服务器
	Server* findFreeGameDBServer() { return findFreeServerByType(GameDBServer); }

	// 查找一个空闲的SessionDB服务器
	//Server* findFreeSessionDBServer() { return findFreeServerByType(SessionDBServer); }
	*/

private:
	// 一个类型对应一个ServerSortList，一个ServerSortList中包含很多个同类型的服务器
	vector<ServerSortList> mServerList;
	easygame::Mutex mMutex;
};



#endif