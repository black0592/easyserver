#include "BaseLib.h"
#include "MsgHanderBase.h"

#define MAX_MSG_PROC_FUNC	0xFFFF

Subscriber* sMsgHandlerFunction[MAX_MSG_PROC_FUNC] = {0};


void releaseMsgHandler()
{
	for (int i=0; i<MAX_MSG_PROC_FUNC; i++) {
		delete sMsgHandlerFunction[i];
		sMsgHandlerFunction[i] = NULL;
	}
}

// ���ָ��ID����Ϣ������
Subscriber* getMsgHandler(ushort cmdID)
{
	if (cmdID >= MAX_MSG_PROC_FUNC) {
		BLOGE("��Ч����ϢID(%u)������δע��", cmdID);
		return NULL;
	}

	return sMsgHandlerFunction[cmdID];
}

bool bindMsgHandler(cmd_type cmd, para_type para, ProtoMessage& msg, const Subscriber* subscriber)
{
	uint cmdID = MAKE_CMDID(cmd, para);

	while(true) {
		if (!subscriber->connected()) {
			BLOGE("ע����ϢID(%u)ʧ�ܣ���Ϣ��Ӧ�Ĵ�����δָ��", cmdID);
			break;
		}

		if (cmdID >= MAX_MSG_PROC_FUNC) {
			BLOGE("��Ч����ϢID(%u)��������Ϣ�ŵ��������(%u)", cmdID, MAX_MSG_PROC_FUNC);
			break;
		}

		const Subscriber* pMsgHander = getMsgHandler(cmdID);
		if ( pMsgHander && pMsgHander->connected() ) {
			BLOGE("��ϢID(%u)��ע�ᣬ�벻Ҫ�ظ�ע��", cmdID);
			break;
		}

		// ע��ɹ�����
		sMsgHandlerFunction[cmdID] = (Subscriber*)subscriber;

		return true;
	}

	delete subscriber;
	return false;
}

/*
void handlerSocketPacket()
{
	NET::stNetPacket* packet = getSocket().popPacket();
	if (packet == NULL)
		return;

	// ����IDȡ����Ϣ������
	stBaseCmd* pCmd = (stBaseCmd*)packet->cmd;
	ushort cmdID = pCmd->getCmdID();
	Subscriber* pSubscriber = getMsgHandler(cmdID);
	if (pSubscriber != NULL) {
		// ��Ϣ������ע���
		byte* protoData = ((byte*)packet->cmd + (sizeof(stBaseCmd)));
		uint protoDataLen = (uint)packet->size - (sizeof(stBaseCmd));
		NetEventArgs evt(protoData, protoDataLen);
		(*pSubscriber)(evt);
	} else {
		CCLOG("handlerSocketPacket fail! hander(%u) is NULL", cmdID);
	}

	// ɾ����
	getSocket().deletePacket(&packet);
}
*/


//////////////////////////////////////////////////////////////////////////

// ������Ϣ��proto��Ӧ��ϵ

map<string,uint> cmdMap;

// ע��һ����Ϣ
void regProtoMsg(cmd_type cmd, para_type para, ProtoMessage& msg)
{
	stBaseCmd baseCmd(cmd, para);
	const string& typeName = msg.GetTypeName();
	cmdMap[typeName] = baseCmd.getCmdID();
}

stBaseCmd findCmdByMsgName(const string& typeName)
{
	stBaseCmd cmd(0, 0);

	if (cmdMap.empty() || cmdMap.find(typeName) == cmdMap.end()) {
		BLOGE("can not find protomsg <%s>", typeName.c_str());
		return cmd;
	}

	uint command = cmdMap[typeName];
	cmd.cmd = GET_CMD(command);
	cmd.para = GET_PARA(command);
	return cmd;
}
