#include "BaseLib.h"
#include "ProtoTask.h"
#include "MsgHanderBase.h"


ProtoTaskBase::ProtoTaskBase()
{

}

ProtoTaskBase::~ProtoTaskBase()
{

}

// ����protobuf��Ϣ
bool ProtoTaskBase::fillProtoMsg(const ProtoMessage& msg, uint cmdID, byte* pBuff, int buffLen, int& nRetLen)
{
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

	uint filledSize = ProtoDataFill::fillProto2Buff(pBuff, buffLen, msg, baseCmd);
	if (filledSize == 0)
		return false;

	// д�뷢�Ͷ���
	//sendCmd((const char *)buffer, filledSize);

	return true;
}

// ������Ϣ
bool ProtoTaskBase::handleMessageImpl(const void *cmd, int cmdLen)
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
#else
	stBaseCmd* pCmd = (stBaseCmd*)cmd;

	if (pCmd->getCmdID() == MAKE_CMDID(CMD_PING,PARA_PING)) {
		// ping���ֱ�ӷ���
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
		// �����л���proto�ṹ��
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
	// д�뷢�Ͷ���
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
	// д�뷢�Ͷ���
	return sendCmd((const char *)buffer, retLen);
}

bool ProtoTaskSync::handleMessage(const void *cmd, int cmdLen)
{
	return handleMessageImpl(cmd, cmdLen);
}









