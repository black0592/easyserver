#pragma once
#include "BaseCmd.h"
#include "ProtobufDefine.h"
#include "NetEventArgs.h"
using namespace easygame;

class ProtoDataFill
{
public:
	// ���msg��cmdID��buff�У����������ֽ��������ڷ���
	static uint fillProto2Buff(void* buff, uint buffLen, const ProtoMessage& msg, uint cmdID);
};







