#ifndef __SERVER_MANAGER_H__
#define __SERVER_MANAGER_H__

#include "Server.h"

// �������б�
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

	// ͨ����ʱID����ȡ������
	Server* getServerByUniqueId(uint64 longId);

	// ɾ��������
	bool removeServer(Server* pServer);

	// ��ӷ�����
	bool addServer(Server* pServer);

	// ���·�����״̬
	void updateServerInfo(uint64 serverUID, uint onlineNum, ServerState state);

	// ͨ�����ͻ�÷�����
	void getListByType(ServerType type, vector<Server*>& vecServer);

	// �㲥��Ϣ��ָ�����͵ķ�����
	void broadcastCmdByType(ServerType type, const void* cmd, ushort cmdLen);

	// �㲥��Ϣ��ָ�����͵ķ�����
	void broadcastProtoMsgByType(ServerType type, ProtoMessage& msg, uint cmdID = 0);

	// ����һ��ָ�����͵Ŀ��еķ�����
	Server* findFreeServerByType(ServerType type);

	/*
	// ����һ�����е����ط�����
	Server* findFreeGatewayServer() { return findFreeServerByType(GatewayServer); }

	// ����һ�����еĵ�½������
	Server* findFreeLoginServer() { return findFreeServerByType(LoginServer); }

	// ����һ�����е���Ϸ������
	Server* findFreeGameServer() { return findFreeServerByType(GameServer); }

	// ����һ�����е�Session������
	Server* findFreeSessionServer() { return findFreeServerByType(SessionServer); }

	// ����һ�����е�LoginDB������
	Server* findFreeLoginDBServer() { return findFreeServerByType(LoginDBServer); }

	// ����һ�����е�GameDB������
	Server* findFreeGameDBServer() { return findFreeServerByType(GameDBServer); }

	// ����һ�����е�SessionDB������
	//Server* findFreeSessionDBServer() { return findFreeServerByType(SessionDBServer); }
	*/

private:
	// һ�����Ͷ�Ӧһ��ServerSortList��һ��ServerSortList�а����ܶ��ͬ���͵ķ�����
	vector<ServerSortList> mServerList;
	easygame::Mutex mMutex;
};



#endif