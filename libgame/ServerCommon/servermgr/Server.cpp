#include "Server.h"

Server::Server(void)
{
	mTCPTask = NULL;
	mState = SS_Free;
	mType = AllServer;
	mIP = 0;
	mPort = 0;
	mOnlineNum = 0;
	mMaxNum = 1000;
}

Server::~Server(void)
{

}

bool Server::sendCmd(const void* cmd, ushort cmdLen)
{
	if (mTCPTask)
		return mTCPTask->sendCmd(cmd, cmdLen);

	return false;
}

bool Server::sendProtoMsg(const ProtoMessage& msg, uint cmdID)
{
	if (mTCPTask == NULL)
		return false;

	ProtoTaskSync* taskSync = dynamic_cast<ProtoTaskSync*>(mTCPTask);
	if (taskSync)
		taskSync->sendProtoMsg(msg, cmdID);

	ProtoTaskAsync* taskAsync = dynamic_cast<ProtoTaskAsync*>(mTCPTask);
	if (taskAsync)
		taskAsync->sendProtoMsg(msg, cmdID);

	return false;
}

void Server::setTCPTask(TCPTask* task)
{
	mTCPTask = task;
}

TCPTask* Server::getTCPTask()
{
	return mTCPTask;
}

void Server::setServerInfo(const ServerInfo& info)
{
	setID(info.serverid());
	setName(info.name().c_str());
	setLongID( server_hash_id(info.ip(),info.port()) );
	setType((ServerType)info.type());
	setIP(info.ip());
	setPort(info.port());
	setOnlineNum(info.onlinenum());
	setMaxNum(info.maxnum());

	setState(SS_Normal);
}

void Server::getServerInfo(ServerInfo& info)
{
	info.set_serverid(getID());
	info.set_name( getName() );
	info.set_uniqueid( getLongID() );
	info.set_type( getType()) ;
	info.set_ip( getIP() );
	info.set_port( getPort() );
	info.set_onlinenum( getOnlineNum() );
	info.set_maxnum( getMaxNum() );
}

void Server::setType(ServerType type)
{
	mType = type;
}

ServerType Server::getType()
{
	return mType;
}

void Server::setState(ServerState state)
{
	mState = state;
}

ServerState Server::getState()
{
	return mState;
}

void Server::setIP(int ip)
{
	mIP = ip;
}

int Server::getIP()
{
	return mIP;
}

void Server::setName(const char* name)
{
	mName = name;
}

const char* Server::getName()
{
	return mName.c_str();
}

void Server::setPort(ushort port)
{
	mPort = port;
}

ushort Server::getPort()
{
	return mPort;
}

void Server::setOnlineNum(uint num)
{
	mOnlineNum = num;
}

uint Server::getOnlineNum()
{
	return mOnlineNum;
}

void Server::setMaxNum(uint num)
{
	mMaxNum = num;
}

uint Server::getMaxNum()
{
	return mMaxNum;
}
