#include "BaseLib.h"
#include "ProtoTask.h"
#include "MsgHanderBase.h"

ProtoTaskAsync::ProtoTaskAsync()
{


}

ProtoTaskAsync::~ProtoTaskAsync()
{

}

bool ProtoTaskAsync::sendProtoMsg(const ProtoMessage& msg, uint cmdID)
{
	stBaseCmd baseCmd(cmdID);
	if (baseCmd.getCmdID() == 0) {
		// ���û�У��ʹӻ�����ȡ
		const string& typeName = msg.GetTypeName();
		baseCmd = ::findCmdByMsgName(typeName);
		if (baseCmd.getCmdID() == 0) {
			BLOGE("unknow msg, plz call <regProtoMsg> first?\n");
			return false;
		}
	}

	byte buffer[MAX_USER_CMD_SIZE];
	uint filledSize = ProtoDataFill::fillProto2Buff(buffer, sizeof(buffer), msg, baseCmd.getCmdID());
	if (filledSize == 0)
		return false;

	// д�뷢�Ͷ���
	sendCmd((const char *)buffer, filledSize);

	return true;
}


bool ProtoTaskAsync::handleMessage(const void *cmd, int cmdLen)
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

	if (pCmd->getCmdID() == MAKE_CMDID(CMD_NULL,PARA_NULL)) {
		// ping���ֱ�ӷ���
		return true;
	}

	byte* protoData = ((byte*)cmd + (sizeof(stBaseCmd)));
	int protoDataLen = (int)cmdLen - (sizeof(stBaseCmd));
	NetEventArgs evt(pCmd, cmdLen, protoData, protoDataLen);
	handleProtoMsg( evt );
	return true;
#endif

	return true;
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
	stBaseCmd baseCmd(cmdID);
	if (baseCmd.getCmdID() == 0) {
		// ���û�У��ʹӻ�����ȡ
		const string& typeName = msg.GetTypeName();
		baseCmd = ::findCmdByMsgName(typeName);
		if (baseCmd.getCmdID() == 0) {
			BLOGE("unknow msg, plz call <regProtoMsg> first?\n");
			return false;
		}
	}

	byte buffer[MAX_USER_CMD_SIZE];
	uint filledSize = ProtoDataFill::fillProto2Buff(buffer, sizeof(buffer), msg, baseCmd.getCmdID());
	if (filledSize == 0)
		return false;

	// д�뷢�Ͷ���
	sendCmd((const char *)buffer, filledSize);

	return true;
}


bool ProtoTaskSync::handleMessage(const void *cmd, int cmdLen)
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

	if (pCmd->getCmdID() == MAKE_CMDID(CMD_NULL,PARA_NULL)) {
		// ping���ֱ�ӷ���
		return true;
	}

	byte* protoData = ((byte*)cmd + (sizeof(stBaseCmd)));
	int protoDataLen = (int)cmdLen - (sizeof(stBaseCmd));
	NetEventArgs evt(pCmd, cmdLen, protoData, protoDataLen);
	handleProtoMsg( evt );
	return true;
#endif

	return true;
}









