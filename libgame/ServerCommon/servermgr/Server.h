#ifndef __SERVER_H__
#define __SERVER_H__

#include "BaseLib.h"
using namespace easygame;

#include "ServerInfo.h"
#include "ProtoSvrServerData.pb.h"


class Server : public Entity
{
public:
	Server(void);
	virtual ~Server(void);

	// ·¢ËÍÏûÏ¢
	bool sendCmd(const void* cmd, ushort cmdLen);

	bool sendProtoMsg(const ProtoMessage& msg, uint cmdID = 0);

	void setServerInfo(const ServerInfo& info);
	void getServerInfo(ServerInfo& info);

	void setTCPTask(TCPTask* task);
	TCPTask* getTCPTask();

	void setType(ServerType type);
	ServerType getType();

	void setState(ServerState state);
	ServerState getState();

	void setName(const char* name);
	const char* getName();

	void setIP(int ip);
	int getIP();

	void setPort(ushort port);
	ushort getPort();

	void setOnlineNum(uint num);
	uint getOnlineNum();

	void setMaxNum(uint num);
	uint getMaxNum();

private:
	std::string mName;
	ServerType mType;
	ServerState mState;
	int mIP;
	ushort mPort;
	uint mOnlineNum;
	uint mMaxNum;
	TCPTask* mTCPTask;
};




#endif