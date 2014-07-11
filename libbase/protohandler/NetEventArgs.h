#ifndef __NETEVENTARGS_H__
#define __NETEVENTARGS_H__

#include "EventTemplate.h"
#include "ProtobufDefine.h"


// 网络层消息参数结构
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
	stBaseCmd* pCmd;		// 基础消息类型
	ushort cmdLen;			// 整个消息长度
	byte* protoData;		// protobuf二进制数据
	ushort protoLen;		// protobuf数据大小
	ProtoMessage* protoMsg;	// proto反射出来的消息结构体
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