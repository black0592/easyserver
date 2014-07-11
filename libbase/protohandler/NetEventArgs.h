#ifndef __NETEVENTARGS_H__
#define __NETEVENTARGS_H__

#include "EventTemplate.h"
#include "ProtobufDefine.h"


// �������Ϣ�����ṹ
class NetEventArgs : public EventArgs
{
public:
	NetEventArgs(stBaseCmd* _cmd, ushort _cmdLen, byte* _protoData, ushort _protoLen, ProtoMessage* _protoMsg)
		: pCmd(_cmd)
		, cmdLen(_cmdLen)
		, protoData(_protoData)
		, protoLen(_protoLen)
		, protoMsg(_protoMsg)
	{

	}

	virtual ~NetEventArgs()
	{
		SAFE_DELETE(protoMsg);
	}

public:
	stBaseCmd* pCmd;		// ������Ϣ����
	ushort cmdLen;			// ������Ϣ����
	byte* protoData;		// protobuf����������
	ushort protoLen;		// protobuf���ݴ�С
	ProtoMessage* protoMsg;	// proto�����������Ϣ�ṹ��
};



__inline void Event2Proto(const EventArgs& args, ProtoMessage& msg)
{
	const NetEventArgs& netargs = (const NetEventArgs&)args;
	msg.ParseFromArray(netargs.protoData, netargs.protoLen);
}

__inline stBaseCmd* Event2BaseCmd(const EventArgs& args)
{
	const NetEventArgs& netargs = (const NetEventArgs&)args;
	return netargs.pCmd;
}



#endif