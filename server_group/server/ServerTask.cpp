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

	// 先初始化脚本路径搜索
	mScript->dofile("package_path.lua");

	// 执行task脚本文件
	mScript->dofile("loginserver/server_task.lua");
}

void ServerTaskBase::destroyScript()
{
	ScriptManager::getInstance().destroyScript(mScript);
}

bool ServerTaskBase::execServerTaskOnConnect()
{
	printf("\n=============== 开始执行脚本 ================\n");

	lua_tinker::call<void,void*>(mScript->getState(), "ServerTask_OnConnect", this);

	ScriptManager::getInstance().printInfo();
	printf("\n=============== 结束脚本执行 ================\n");

	return true;
}

void ServerTaskBase::execServerTaskOnDisconnect()
{
	printf("\n=============== 开始执行脚本 ================\n");

	lua_tinker::call<void,void*>(mScript->getState(), "ServerTask_OnDisconnect", this);

	ScriptManager::getInstance().printInfo();
	printf("\n=============== 结束脚本执行 ================\n");
}

void ServerTaskBase::execServerTaskHandleProtoMsg(const EventArgs& args)
{
	const NetEventArgs& netArgs = (const NetEventArgs&)args;
	if (netArgs.protoMsg == NULL)
		return;

	LoginCmd::RequestRegisterGameServer* loinMsg = (LoginCmd::RequestRegisterGameServer*)netArgs.protoMsg;

	printf("\n=============== 开始执行脚本 ================\n");

	ProtoMessage* pMsg = netArgs.protoMsg;
	string strData;
	strData.reserve(netArgs.protoLen+1);
	strData.append((const char*)netArgs.protoData, netArgs.protoLen);
	lua_tinker::call<void,void*,const char*>(mScript->getState(), "ServerTask_handleProtoMsg", this, strData.c_str());

	ScriptManager::getInstance().printInfo();
	printf("\n=============== 结束脚本执行 ================\n");
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

// 连接断开时被调用
void ServerTaskAsync::OnDisconnect()
{
	this->execServerTaskOnDisconnect();
}

bool ServerTaskAsync::handleProtoMsg(const EventArgs& args)
{
	stBaseCmd* pCmd = Event2BaseCmd(args);
	if (pCmd->cmd != SVR_MSG_SUPER_SERVICE) {

		// 回显测试
		//sendCmd(cmd, cmdLen);
		//return true;

		LOGD("非法的服务器消息号 cmd=%d", pCmd->cmd);
		return false;
	}

	// 消息性能记录
	FUNC_PF_EXT(strformat("消息执行时间过长para=%d",pCmd->para), 50);

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

// 连接断开时被调用
void ServerTaskSync::OnDisconnect()
{
	this->execServerTaskOnDisconnect();
}

bool ServerTaskSync::handleProtoMsg(const EventArgs& args)
{
	stBaseCmd* pCmd = Event2BaseCmd(args);
	//if (pCmd->cmd != SVR_MSG_SUPER_SERVICE) {

	//	// 回显测试
	//	//sendCmd(cmd, cmdLen);
	//	//return true;

	//	LOGD("非法的服务器消息号 cmd=%d", pCmd->cmd);
	//	return false;
	//}

	// 消息性能记录
	FUNC_PF_EXT(strformat("消息执行时间过长para=%d",pCmd->para), 50);

	this->execServerTaskHandleProtoMsg(args);

	return true;
}
