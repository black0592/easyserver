#pragma once
#include "BaseCmd.h"
#include "ProtobufDefine.h"
#include "NetEventArgs.h"
using namespace easygame;


class ProtoTaskBase
{
public:
	ProtoTaskBase();
	virtual ~ProtoTaskBase();

	// 处理protobuf的消息
	virtual bool handleProtoMsg(const EventArgs& args) = 0;

protected:
	// 填充protobuf消息
	bool fillProtoMsg(const ProtoMessage& msg, uint cmdID, byte* pBuff, int buffLen, int& nRetLen);

	// 处理消息
	bool handleMessageImpl(const void *cmd, int cmdLen);

protected:
private:
};

//////////////////////////////////////////////////////////////////////////


// --- 异步处理消息的ProtoTaskAsync -----
class ProtoTaskAsync : public TCPTaskAsync, ProtoTaskBase
{
public:
	ProtoTaskAsync();
	virtual ~ProtoTaskAsync(void);

	bool sendProtoMsg(const ProtoMessage& msg, uint cmdID = 0);

protected:
	// 处理消息
	virtual bool handleMessage(const void *cmd, int cmdLen);
};



//////////////////////////////////////////////////////////////////////////


// --- 采用消息队列方式来处理消息 -----
class ProtoTaskSync : public TCPTaskSync, ProtoTaskBase
{
public:
	ProtoTaskSync();
	virtual ~ProtoTaskSync(void);

	bool sendProtoMsg(const ProtoMessage& msg, uint cmdID = 0);

protected:
	// 处理消息
	virtual bool handleMessage(const void *cmd, int cmdLen);
};







