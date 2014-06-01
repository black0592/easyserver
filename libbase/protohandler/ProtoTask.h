#pragma once
#include "BaseCmd.h"
#include "ProtobufDefine.h"
#include "NetEventArgs.h"
using namespace easygame;

// --- �첽������Ϣ��ProtoTaskAsync -----
class ProtoTaskAsync : public TCPTaskAsync
{
public:
	ProtoTaskAsync();
	virtual ~ProtoTaskAsync(void);

	// ����protobuf����Ϣ
	virtual bool handleProtoMsg(const EventArgs& args) = 0;

	// ����protobuf��Ϣ
	virtual bool sendProtoMsg(const ProtoMessage& msg, uint cmdID = 0);

	// ������Ϣ
	virtual bool handleMessage(const void *cmd, int cmdLen);
};



//////////////////////////////////////////////////////////////////////////


// --- ������Ϣ���з�ʽ��������Ϣ -----
class ProtoTaskSync : public TCPTaskSync
{
public:
	ProtoTaskSync();
	virtual ~ProtoTaskSync(void);

	// ����protobuf����Ϣ
	virtual bool handleProtoMsg(const EventArgs& args) = 0;

	// ����protobuf��Ϣ
	virtual bool sendProtoMsg(const ProtoMessage& msg, uint cmdID = 0);

	// ������Ϣ
	virtual bool handleMessage(const void *cmd, int cmdLen);
};







