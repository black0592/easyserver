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

// 获得指定ID的消息处理函数
Subscriber* getMsgHandler(ushort cmdID)
{
	if (cmdID >= MAX_MSG_PROC_FUNC) {
		BLOGE("无效的消息ID(%u)，可能未注册", cmdID);
		return NULL;
	}

	return sMsgHandlerFunction[cmdID];
}

bool bindMsgHandler(cmd_type cmd, para_type para, ProtoMessage& msg, const Subscriber* subscriber)
{
	uint cmdID = MAKE_CMDID(cmd, para);

	while(true) {
		if (!subscriber->connected()) {
			BLOGE("注册消息ID(%u)失败，消息对应的处理函数未指定", cmdID);
			break;
		}

		if (cmdID >= MAX_MSG_PROC_FUNC) {
			BLOGE("无效的消息ID(%u)，超过消息号的最大限制(%u)", cmdID, MAX_MSG_PROC_FUNC);
			break;
		}

		const Subscriber* pMsgHander = getMsgHandler(cmdID);
		if ( pMsgHander && pMsgHander->connected() ) {
			BLOGE("消息ID(%u)已注册，请不要重复注册", cmdID);
			break;
		}

		// 注册成功！！
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

	// 根据ID取出消息处理器
	stBaseCmd* pCmd = (stBaseCmd*)packet->cmd;
	ushort cmdID = pCmd->getCmdID();
	Subscriber* pSubscriber = getMsgHandler(cmdID);
	if (pSubscriber != NULL) {
		// 消息处理函数注册过
		byte* protoData = ((byte*)packet->cmd + (sizeof(stBaseCmd)));
		uint protoDataLen = (uint)packet->size - (sizeof(stBaseCmd));
		NetEventArgs evt(protoData, protoDataLen);
		(*pSubscriber)(evt);
	} else {
		CCLOG("handlerSocketPacket fail! hander(%u) is NULL", cmdID);
	}

	// 删除包
	getSocket().deletePacket(&packet);
}
*/


//////////////////////////////////////////////////////////////////////////

// 保存消息和proto对应关系

map<string,uint> cmdMap;

// 注册一条消息
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
