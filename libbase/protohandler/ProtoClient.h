#pragma once
#include "BaseCmd.h"
#include "ProtobufDefine.h"
#include "NetEventArgs.h"
using namespace easygame;

class ProtoClient : public easygame::SocketClient
{
public:
	ProtoClient(const string& name);
	virtual ~ProtoClient(void);

	// 处理protobuf的消息
	virtual bool handleProtoMsg(const EventArgs& args) = 0;

	// 发送protobuf消息
	virtual bool sendProtoMsg(const ProtoMessage& msg, uint cmdID = 0);

	// 接受到消息后的处理
	virtual bool handleMessage(const void *cmd, int cmdLen);
};
