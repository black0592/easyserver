#include "BaseLib.h"
#include "ProtoDataFill.h"


uint ProtoDataFill::fillProto2Buff(void* buff, uint buffLen, const ProtoMessage& msg, const stBaseCmd& cmd)
{
	int protoSize = msg.ByteSize();
	uint cmdLen = sizeof(stBaseCmd) + protoSize;

	if (cmdLen > MAX_USER_CMD_SIZE || cmdLen > buffLen) {
		BLOGE("[ProtoDataFill::fillProto2Buff] msg too long!");
		return 0;
	}

	byte* pBufferPtr = (byte*)buff;

	// 写入消息内容
	memcpy(pBufferPtr, &cmd, sizeof(stBaseCmd));
	pBufferPtr += sizeof(stBaseCmd);

	// 写入protobuf二进制流
	msg.SerializeToArray(pBufferPtr, protoSize);

	return cmdLen;
}







