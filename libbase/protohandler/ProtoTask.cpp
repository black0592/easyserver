#include "BaseLib.h"
#include "ProtoTask.h"
#include "MsgHanderBase.h"


ProtoTaskBase::ProtoTaskBase()
{

}

ProtoTaskBase::~ProtoTaskBase()
{

}

// 发送protobuf消息
bool ProtoTaskBase::fillProtoMsg(const ProtoMessage& msg, uint cmdID, byte* pBuff, int buffLen, int& nRetLen)
{
	stBaseCmd baseCmd(cmdID);
	if (baseCmd.getCmdID() == 0) {
		// 如果没有，就从缓存中取
		const string& typeName = msg.GetTypeName();
		baseCmd = ::findCmdByMsgName(typeName);
		if (baseCmd.getCmdID() == 0) {
#if ENABLE_PROTO_REFLECT
			// 如果未注册，那就启用自动反射机制
			baseCmd.setTypeName( msg.GetTypeName().c_str() );
#else
			BLOGE("unknow msg, plz call <regProtoMsg> first?\n");
			return false;
#endif
		}
	}

	uint filledSize = ProtoDataFill::fillProto2Buff(pBuff, buffLen, msg, baseCmd);
	if (filledSize == 0)
		return false;

	// 写入发送队列
	//sendCmd((const char *)buffer, filledSize);

	return true;
}

// 处理消息
bool ProtoTaskBase::handleMessageImpl(const void *cmd, int cmdLen)
{
#if 0
	// 根据ID取出消息处理器
	stBaseCmd* pCmd = (stBaseCmd*)cmd;
	ushort cmdID = pCmd->getCmdID();
	Subscriber* pSubscriber = getMsgHandler(cmdID);
	if (pSubscriber != NULL) {
		// 消息处理函数注册过
		byte* protoData = ((byte*)cmd + (sizeof(stBaseCmd)));
		int protoDataLen = (int)cmdLen - (sizeof(stBaseCmd));
		NetEventArgs evt(protoData, protoDataLen);
		(*pSubscriber)(evt);
	} else {
		BLOGE("handlerSocketPacket fail! hander(%u) is NULL", cmdID);
		return false;
	}
#else
	stBaseCmd* pCmd = (stBaseCmd*)cmd;

	if (pCmd->getCmdID() == MAKE_CMDID(CMD_PING,PARA_PING)) {
		// ping命令，直接返回
		return true;
	}

	byte* protoData = ((byte*)cmd + (sizeof(stBaseCmd)));
	int protoDataLen = (int)cmdLen - (sizeof(stBaseCmd));
	ProtoMessage* protoMsg = NULL;
#if ENABLE_PROTO_REFLECT
	if (pCmd->getCmdID() == 0) {
		protoMsg = createMessage(pCmd->name);
		assert(protoMsg);
		if (protoMsg == NULL) {
			return true;
		}
		// 反序列化成proto结构体
		protoMsg->ParseFromArray(protoData, protoDataLen);
	}
#endif
	NetEventArgs evt(pCmd, cmdLen, protoData, protoDataLen, protoMsg);
	handleProtoMsg( evt );
	return true;
#endif

	return true;
}

//////////////////////////////////////////////////////////////////////////


ProtoTaskAsync::ProtoTaskAsync()
{


}

ProtoTaskAsync::~ProtoTaskAsync()
{

}

bool ProtoTaskAsync::sendProtoMsg(const ProtoMessage& msg, uint cmdID)
{
	byte buffer[MAX_USER_CMD_SIZE];
	int retLen = 0;
	fillProtoMsg(msg, cmdID, buffer, sizeof(buffer), retLen);
	// 写入发送队列
	return sendCmd((const char *)buffer, retLen);
}

bool ProtoTaskAsync::handleMessage(const void *cmd, int cmdLen)
{
	return handleMessageImpl(cmd, cmdLen);
}




//////////////////////////////////////////////////////////////////////////



ProtoTaskSync::ProtoTaskSync()
{


}

ProtoTaskSync::~ProtoTaskSync()
{

}

bool ProtoTaskSync::sendProtoMsg(const ProtoMessage& msg, uint cmdID)
{
	byte buffer[MAX_USER_CMD_SIZE];
	int retLen = 0;
	fillProtoMsg(msg, cmdID, buffer, sizeof(buffer), retLen);
	// 写入发送队列
	return sendCmd((const char *)buffer, retLen);
}

bool ProtoTaskSync::handleMessage(const void *cmd, int cmdLen)
{
	return handleMessageImpl(cmd, cmdLen);
}









