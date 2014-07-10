#include "ServerHeader.h"
#include "ServerTask.h"


//////////////////////////////////////////////////////////////////////////


void execServerTaskOnConnect(ScriptObject* pScript)
{
	pScript = ScriptManager::getInstance().createScript();
	printf("\n=============== ��ʼִ�нű� ================\n");
	pScript->dofile("./datas/scripts/server_task.lua");
	pScript->dostring("ServerTask_OnConnect()");
	ScriptManager::getInstance().printInfo();
	printf("\n=============== �����ű�ִ�� ================\n");
	ScriptManager::getInstance().destroyScript(pScript);
}

void execServerTaskOnDisconnect(ScriptObject* pScript)
{
	pScript = ScriptManager::getInstance().createScript();
	printf("\n=============== ��ʼִ�нű� ================\n");
	pScript->dofile("./datas/scripts/server_task.lua");
	pScript->dostring("ServerTask_OnDisconnect()");
	ScriptManager::getInstance().printInfo();
	printf("\n=============== �����ű�ִ�� ================\n");
	ScriptManager::getInstance().destroyScript(pScript);
}

void execServerTaskHandleProtoMsg(ScriptObject* pScript, const EventArgs& args)
{
	pScript = ScriptManager::getInstance().createScript();
	printf("\n=============== ��ʼִ�нű� ================\n");
	pScript->dofile("./datas/scripts/server_task.lua");
	pScript->dostring("ServerTask_handleProtoMsg()");
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
	execServerTaskOnConnect(NULL);

	return true;
}

// ���ӶϿ�ʱ������
void ServerTaskAsync::OnDisconnect()
{
	execServerTaskOnDisconnect(NULL);
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

	execServerTaskHandleProtoMsg(NULL, args);

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
	execServerTaskOnConnect(NULL);

	return true;
}

// ���ӶϿ�ʱ������
void ServerTaskSync::OnDisconnect()
{
	execServerTaskOnDisconnect(NULL);
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

	execServerTaskHandleProtoMsg(NULL, args);

	return true;
}
