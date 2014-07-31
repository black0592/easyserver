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



class test
{
public:
	test(int val) : _test(val) {}
	~test() {}

	const char* is_test(){ return "this is test"; }

	void ret_void() {}
	int ret_int()				{ return _test;			}
	int ret_mul(int m) const	{ return _test * m;		}

	void sendProtoMsg(int a, int b)
	{
		a = a;
		b = b; 
	}

	int _test;
};

test g_test(11);
//ServerTaskSync g_task;

void ServerTaskBase::createScript()
{
	assert(mScript == NULL);

	mScript = ScriptManager::getInstance().createScript();
	lua_State* L = mScript->getState();

	//////////////////////////////////////////////////////////////////////////
	// ע��Task�ӿ�

	lua_tinker::class_add<ProtoTask>(L, "ProtoTask");
	lua_tinker::class_def<ProtoTask>(L, "sendProtoMsg", &ProtoTask::sendProtoMsg);
	lua_tinker::class_def<ProtoTask>(L, "sendProtoMsg2", &ProtoTask::sendProtoMsg2);
	lua_tinker::class_def<ProtoTask>(L, "sendProtoMsg3", &ProtoTask::sendProtoMsg3);
	//lua_tinker::class_def<ProtoTask>(L, "sendProtoMsg2", &ProtoTask::sendProtoMsg2);

	lua_tinker::class_add<ServerTaskSync>(L, "ServerTaskSync");
	lua_tinker::class_inh<ServerTaskSync, ProtoTask>(L);
	lua_tinker::class_con<ServerTaskSync>(L, lua_tinker::constructor<ServerTaskSync>);
	//lua_tinker::class_def<ServerTaskSync>(L, "sendProtoMsg", &ServerTaskSync::sendProtoMsg);

	ServerTaskSync* g_task = (ServerTaskSync*)this;
	lua_tinker::set(L, "g_task", g_task);

	//////////////////////////////////////////////////////////////////////////

	// test Ŭ������ Lua �� �߰��Ѵ�.
	lua_tinker::class_add<test>(L, "test");
	// test Ŭ���� �����ڸ� ����Ѵ�.
	lua_tinker::class_con<test>(L, lua_tinker::constructor<test,int>);
	// test �� �Լ����� ����Ѵ�.
	lua_tinker::class_def<test>(L, "is_test", &test::is_test);
	lua_tinker::class_def<test>(L, "ret_void", &test::ret_void);
	lua_tinker::class_def<test>(L, "ret_int", &test::ret_int);
	lua_tinker::class_def<test>(L, "ret_mul", &test::ret_mul);
	//lua_tinker::class_def<test>(L, "sendProtoMsg", &test::sendProtoMsg);
	lua_tinker::class_mem<test>(L, "_test", &test::_test);

	// Lua ���� ����ȣ g_test �� �����͸� ����Ѵ�.
	lua_tinker::set(L, "g_test", &g_test);

	//////////////////////////////////////////////////////////////////////////

	// �ȳ�ʼ���ű�·������
	mScript->dofile("package_path.lua");

	// ִ��task�ű��ļ�
	mScript->dofile("loginserver/server_task.lua");
	//mScript->dofile("D:/����Ŀ¼/test3/luatest/bin/datas/scripts/loginserver/server_task.lua");

	//lua_tinker::call<void,test*>(L, "test_func", &g_test);

	lua_tinker::call<void>(L, "test_func");

	mScript = mScript;
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
	//if (netArgs.protoMsg == NULL)
	//	return;

	//LoginCmd::RequestRegisterGameServer* loinMsg = (LoginCmd::RequestRegisterGameServer*)netArgs.protoMsg;

	printf("\n=============== ��ʼִ�нű� ================\n");

//#ifdef _DEBUG
//	destroyScript();
//	createScript();
//#endif

	ProtoMessage* pMsg = netArgs.protoMsg;
	string strData;
	strData.reserve(netArgs.protoLen+1);
	strData.append((const char*)netArgs.protoData, netArgs.protoLen);
	//lua_tinker::call<void,void*,const char*>(mScript->getState(), "ServerTask_handleProtoMsg", this, strData.c_str());
	string strCallBack = "handle";
	strCallBack += netArgs.pCmd->name;
	lua_tinker::call<void,ServerTaskSync*,const char*>(mScript->getState(), strCallBack.c_str(), (ServerTaskSync*)this, strData.c_str());

	ScriptManager::getInstance().printInfo();
	printf("\n=============== �����ű�ִ�� ================\n");
}

//////////////////////////////////////////////////////////////////////////

ServerTaskAsync::ServerTaskAsync()
	: ProtoTask(AsyncType)
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
		: ProtoTask(SyncType)
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
