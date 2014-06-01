#pragma once
#include "BaseCmd.h"
#include "ProtobufDefine.h"
#include "NetEventArgs.h"
using namespace easygame;

class ProtoDataFill
{
public:
	// 填充msg和cmdID到buff中，返回填充的字节数，用于发送
	static uint fillProto2Buff(void* buff, uint buffLen, const ProtoMessage& msg, uint cmdID);
};







