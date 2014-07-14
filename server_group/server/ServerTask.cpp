#include "ServerHeader.h"
#include "ServerTask.h"
#include "ProtoSvrLogin.pb.h"


//////////////////////////////////////////////////////////////////////////


void execServerTaskOnConnect(TCPTask* pTask, ScriptObject* pScript)
{
	pScript = ScriptManager::getInstance().createScript();
	printf("\n=============== ��ʼִ�нű� ================\n");
	
	// �ȳ�ʼ���ű�·������
	pScript->dofile("datas/scripts/package_path.lua");

	pScript->dofile("datas/scripts/server_task.lua");
	lua_tinker::call<void,TCPTask*>(pScript->getState(), "ServerTask_OnConnect", pTask);

	ScriptManager::getInstance().printInfo();
	printf("\n=============== �����ű�ִ�� ================\n");
	ScriptManager::getInstance().destroyScript(pScript);
}

void execServerTaskOnDisconnect(TCPTask* pTask, ScriptObject* pScript)
{
	pScript = ScriptManager::getInstance().createScript();
	printf("\n=============== ��ʼִ�нű� ================\n");

	// �ȳ�ʼ���ű�·������
	pScript->dofile("datas/scripts/package_path.lua");

	pScript->dofile("datas/scripts/server_task.lua");
	lua_tinker::call<void,TCPTask*>(pScript->getState(), "ServerTask_OnDisconnect", pTask);

	//pScript->dostring("ServerTask_OnDisconnect()");
	ScriptManager::getInstance().printInfo();
	printf("\n=============== �����ű�ִ�� ================\n");
	ScriptManager::getInstance().destroyScript(pScript);
}

void execServerTaskHandleProtoMsg(TCPTask* pTask, ScriptObject* pScript, const EventArgs& args)
{
	const NetEventArgs& netArgs = (const NetEventArgs&)args;
	if (netArgs.protoMsg == NULL)
		return;

	LoginCmd::RequestRegisterGameServer* loinMsg = (LoginCmd::RequestRegisterGameServer*)netArgs.protoMsg;

	pScript = ScriptManager::getInstance().createScript();
	printf("\n=============== ��ʼִ�нű� ================\n");

	// �ȳ�ʼ���ű�·������
	pScript->dofile("datas/scripts/package_path.lua");

	pScript->dofile("datas/scripts/server_task.lua");

	ProtoMessage* pMsg = netArgs.protoMsg;
	string strData;
	strData.reserve(netArgs.protoLen+1);
	strData.append((const char*)netArgs.protoData, netArgs.protoLen);
	lua_tinker::call<void,TCPTask*,const char*>(pScript->getState(), "ServerTask_handleProtoMsg", pTask, strData.c_str());

	//pScript->dostring("ServerTask_handleProtoMsg()");
	ScriptManager::getInstance().printInfo();
	printf("\n=============== �����ű�ִ�� ================\n");
	ScriptManager::getInstance().destroyScript(pScript);
}

//////////////////////////////////////////////////////////////////////////

ServerTaskAsync::ServerTaskAsync()
{
	mUniqueId = 0;
}

ServerTaskAsync::~ServerTaskAsync()
{

}

bool ServerTaskAsync::OnConnect()
{
	execServerTaskOnConnect(this, NULL);

	return true;
}

// ���ӶϿ�ʱ������
void ServerTaskAsync::OnDisconnect()
{
	execServerTaskOnDisconnect(this, NULL);
}

bool ServerTaskAsync::handleProtoMsg(const EventArgs& args)
{
	stBaseCmd* pCmd = Event2BaseCmd(args);
	if (pCmd->cmd != SVR_MSG_SUPER_SERVICE) {

		// ���Բ���
		//sendCmd(cmd, cmdLen);
		//return true;

		LOGD("�Ƿ��ķ�������Ϣ�� cmd=%d", pCmd->cmd);
		return false;
	}

	// ��Ϣ���ܼ�¼
	FUNC_PF_EXT(strformat("��Ϣִ��ʱ�����para=%d",pCmd->para), 50);

	execServerTaskHandleProtoMsg(this, NULL, args);

	return true;
}


//////////////////////////////////////////////////////////////////////////


ServerTaskSync::ServerTaskSync()
{
	mUniqueId = 0;
}

ServerTaskSync::~ServerTaskSync()
{

}

bool ServerTaskSync::OnConnect()
{
	execServerTaskOnConnect(this, NULL);

	return true;
}

// ���ӶϿ�ʱ������
void ServerTaskSync::OnDisconnect()
{
	execServerTaskOnDisconnect(this, NULL);
}

bool ServerTaskSync::handleProtoMsg(const EventArgs& args)
{
	stBaseCmd* pCmd = Event2BaseCmd(args);
	//if (pCmd->cmd != SVR_MSG_SUPER_SERVICE) {

	//	// ���Բ���
	//	//sendCmd(cmd, cmdLen);
	//	//return true;

	//	LOGD("�Ƿ��ķ�������Ϣ�� cmd=%d", pCmd->cmd);
	//	return false;
	//}

	// ��Ϣ���ܼ�¼
	FUNC_PF_EXT(strformat("��Ϣִ��ʱ�����para=%d",pCmd->para), 50);

	execServerTaskHandleProtoMsg(this, NULL, args);

	return true;
}
