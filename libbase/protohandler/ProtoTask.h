#pragma once
#include "BaseCmd.h"
#include "ProtobufDefine.h"
#include "NetEventArgs.h"
using namespace easygame;


class ProtoTask : public TCPTaskImpl
{
public:
	ProtoTask(TCPTaskType type);
	virtual ~ProtoTask();

	bool sendProtoMsg(const ProtoMessage& msg, uint cmdID = 0);
	bool sendProtoMsg2(const char* msg, int msgLen, const char* typeName, uint cmdID = 0);
	bool sendProtoMsg3(int a, int b);

protected:
	// ���protobuf��Ϣ
	bool fillProtoMsg(const ProtoMessage& msg, uint cmdID, byte* pBuff, int buffLen, int& nRetLen);

	// ������Ϣ
	virtual bool handleMessage(const void *cmd, int cmdLen);

	// ����protobuf����Ϣ
	virtual bool handleProtoMsg(const EventArgs& args) { return true; }

protected:
private:
};

//////////////////////////////////////////////////////////////////////////

/*

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


*/




