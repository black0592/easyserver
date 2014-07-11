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

	// ����protobuf����Ϣ
	virtual bool handleProtoMsg(const EventArgs& args) = 0;

protected:
	// ���protobuf��Ϣ
	bool fillProtoMsg(const ProtoMessage& msg, uint cmdID, byte* pBuff, int buffLen, int& nRetLen);

	// ������Ϣ
	bool handleMessageImpl(const void *cmd, int cmdLen);

protected:
private:
};

//////////////////////////////////////////////////////////////////////////


// --- �첽������Ϣ��ProtoTaskAsync -----
class ProtoTaskAsync : public TCPTaskAsync, ProtoTaskBase
{
public:
	ProtoTaskAsync();
	virtual ~ProtoTaskAsync(void);

	bool sendProtoMsg(const ProtoMessage& msg, uint cmdID = 0);

protected:
	// ������Ϣ
	virtual bool handleMessage(const void *cmd, int cmdLen);
};



//////////////////////////////////////////////////////////////////////////


// --- ������Ϣ���з�ʽ��������Ϣ -----
class ProtoTaskSync : public TCPTaskSync, ProtoTaskBase
{
public:
	ProtoTaskSync();
	virtual ~ProtoTaskSync(void);

	bool sendProtoMsg(const ProtoMessage& msg, uint cmdID = 0);

protected:
	// ������Ϣ
	virtual bool handleMessage(const void *cmd, int cmdLen);
};







