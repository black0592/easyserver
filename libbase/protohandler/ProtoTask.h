#pragma once
#include "BaseCmd.h"
#include "ProtobufDefine.h"
#include "NetEventArgs.h"
using namespace easygame;

// --- 异步处理消息的ProtoTaskAsync -----
class ProtoTaskAsync : public TCPTaskAsync
{
public:
	ProtoTaskAsync();
	virtual ~ProtoTaskAsync(void);

	// 处理protobuf的消息
	virtual bool handleProtoMsg(const EventArgs& args) = 0;

	// 发送protobuf消息
	virtual bool sendProtoMsg(const ProtoMessage& msg, uint cmdID = 0);

	// 处理消息
	virtual bool handleMessage(const void *cmd, int cmdLen);
};



//////////////////////////////////////////////////////////////////////////


// --- 采用消息队列方式来处理消息 -----
class ProtoTaskSync : public TCPTaskSync
{
public:
	ProtoTaskSync();
	virtual ~ProtoTaskSync(void);

	// 处理protobuf的消息
	virtual bool handleProtoMsg(const EventArgs& args) = 0;

	// 发送protobuf消息
	virtual bool sendProtoMsg(const ProtoMessage& msg, uint cmdID = 0);

	// 处理消息
	virtual bool handleMessage(const void *cmd, int cmdLen);
};







