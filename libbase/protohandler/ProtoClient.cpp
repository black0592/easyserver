#include "BaseLib.h"
#include "ProtoClient.h"
#include "MsgHanderBase.h"

ProtoClient::ProtoClient(const string& name)
	: SocketClient(name)
{
}

ProtoClient::~ProtoClient()
{

}


bool ProtoClient::sendProtoMsg(const ProtoMessage& msg, uint cmdID)
{
	if ( !isConnected() )
		return false;

	stBaseCmd baseCmd(cmdID);
	if (baseCmd.getCmdID() == 0) {
		// 如果没有，就从缓存中取
		const string& typeName = msg.GetTypeName();
		baseCmd = ::findCmdByMsgName(typeName);
		if (baseCmd.getCmdID() == 0) {
			BLOGE("unknow msg, plz call <regProtoMsg> first?\n");
			return false;
		}
	}
	
	byte rawData[MAX_USER_CMD_SIZE];
	uint filledSize = ProtoDataFill::fillProto2Buff(rawData, sizeof(rawData), msg, baseCmd.getCmdID());
	if (filledSize == 0)
		return false;

	// 写入发送队列
	return sendCmd(rawData, filledSize);
}


bool ProtoClient::handleMessage(const void *cmd, int cmdLen)
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

	return true;
#else
	stBaseCmd* pCmd = (stBaseCmd*)cmd;
	byte* protoData = ((byte*)cmd + (sizeof(stBaseCmd)));
	int protoDataLen = (int)cmdLen - (sizeof(stBaseCmd));
	NetEventArgs evt(pCmd, cmdLen, protoData, protoDataLen);
	handleProtoMsg( evt );
	return true;
#endif
}














