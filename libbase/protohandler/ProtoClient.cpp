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
		// ���û�У��ʹӻ�����ȡ
		const string& typeName = msg.GetTypeName();
		baseCmd = ::findCmdByMsgName(typeName);
		if (baseCmd.getCmdID() == 0) {
#if ENABLE_PROTO_REFLECT
			// ���δע�ᣬ�Ǿ������Զ��������
			baseCmd.setTypeName( msg.GetTypeName().c_str() );
#else
			BLOGE("unknow msg, plz call <regProtoMsg> first?\n");
			return false;
#endif
		}
	}
	
	byte rawData[MAX_USER_CMD_SIZE];
	uint filledSize = ProtoDataFill::fillProto2Buff(rawData, sizeof(rawData), msg, baseCmd);
	if (filledSize == 0)
		return false;

	// д�뷢�Ͷ���
	return sendCmd(rawData, filledSize);
}


bool ProtoClient::handleMessage(const void *cmd, int cmdLen)
{
#if 0
	// ����IDȡ����Ϣ������
	stBaseCmd* pCmd = (stBaseCmd*)cmd;
	ushort cmdID = pCmd->getCmdID();
	Subscriber* pSubscriber = getMsgHandler(cmdID);
	if (pSubscriber != NULL) {
		// ��Ϣ������ע���
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
	ProtoMessage* protoMsg = NULL;
#if ENABLE_PROTO_REFLECT
	if (pCmd->getCmdID() == 0) {
		protoMsg = createMessage(pCmd->name);
		assert(protoMsg);
	}
#endif
	NetEventArgs evt(pCmd, cmdLen, protoData, protoDataLen, protoMsg);
	handleProtoMsg( evt );
	return true;
#endif
}














