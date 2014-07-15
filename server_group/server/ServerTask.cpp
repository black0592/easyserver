#include "ServerHeader.h"
#include "ServerTask.h"
#include "ProtoSvrLogin.pb.h"


ServerTaskBase::ServerTaskBase()
{
	mScript = NULL;
}

ServerTaskBase::~ServerTaskBase()
{
}

void ServerTaskBase::createScript()
{
	assert(mScript == NULL);

	mScript = ScriptManager::getInstance().createScript();

	// �ȳ�ʼ���ű�·������
	mScript->dofile("package_path.lua");

	// ִ��task�ű��ļ�
	mScript->dofile("loginserver/server_task.lua");
}

void ServerTaskBase::destroyScript()
{
	ScriptManager::getInstance().destroyScript(mScript);
}

bool ServerTaskBase::execServerTaskOnConnect()
{
	printf("\n=============== ��ʼִ�нű� ================\n");

	lua_tinker::call<void,void*>(mScript->getState(), "ServerTask_OnConnect", this);

	ScriptManager::getInstance().printInfo();
	printf("\n=============== �����ű�ִ�� ================\n");

	return true;
}

void ServerTaskBase::execServerTaskOnDisconnect()
{
	printf("\n=============== ��ʼִ�нű� ================\n");

	lua_tinker::call<void,void*>(mScript->getState(), "ServerTask_OnDisconnect", this);

	ScriptManager::getInstance().printInfo();
	printf("\n=============== �����ű�ִ�� ================\n");
}

void ServerTaskBase::execServerTaskHandleProtoMsg(const EventArgs& args)
{
	const NetEventArgs& netArgs = (const NetEventArgs&)args;
	if (netArgs.protoMsg == NULL)
		return;

	LoginCmd::RequestRegisterGameServer* loinMsg = (LoginCmd::RequestRegisterGameServer*)netArgs.protoMsg;

	printf("\n=============== ��ʼִ�нű� ================\n");

	ProtoMessage* pMsg = netArgs.protoMsg;
	string strData;
	strData.reserve(netArgs.protoLen+1);
	strData.append((const char*)netArgs.protoData, netArgs.protoLen);
	lua_tinker::call<void,void*,const char*>(mScript->getState(), "ServerTask_handleProtoMsg", this, strData.c_str());

	ScriptManager::getInstance().printInfo();
	printf("\n=============== �����ű�ִ�� ================\n");
}

//////////////////////////////////////////////////////////////////////////

ServerTaskAsync::ServerTaskAsync()
{
	mUniqueId = 0;
	createScript();
}

ServerTaskAsync::~ServerTaskAsync()
{
	destroyScript();
}

bool ServerTaskAsync::OnConnect()
{
	this->execServerTaskOnConnect();

	return true;
}

// ���ӶϿ�ʱ������
void ServerTaskAsync::OnDisconnect()
{
	this->execServerTaskOnDisconnect();
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

	this->execServerTaskHandleProtoMsg(args);

	return true;
}


//////////////////////////////////////////////////////////////////////////


ServerTaskSync::ServerTaskSync()
{
	mUniqueId = 0;
	createScript();
}

ServerTaskSync::~ServerTaskSync()
{
	destroyScript();
}

bool ServerTaskSync::OnConnect()
{
	this->execServerTaskOnConnect();

	return true;
}

// ���ӶϿ�ʱ������
void ServerTaskSync::OnDisconnect()
{
	this->execServerTaskOnDisconnect();
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

	this->execServerTaskHandleProtoMsg(args);

	return true;
}
