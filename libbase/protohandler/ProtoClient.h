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

	// ����protobuf����Ϣ
	virtual bool handleProtoMsg(const EventArgs& args) = 0;

	// ����protobuf��Ϣ
	virtual bool sendProtoMsg(const ProtoMessage& msg, uint cmdID = 0);

	// ���ܵ���Ϣ��Ĵ���
	virtual bool handleMessage(const void *cmd, int cmdLen);
};
