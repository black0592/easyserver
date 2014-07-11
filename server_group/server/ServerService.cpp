#include "ServerHeader.h"
#include "ServerService.h"
#include "TimerForMain.h"

const EventType EVT_TestEvent = "TestEvent";

class TestComponent : public Component
{
public:
	static const string type;

	TestComponent()
	{
		BindGameEvent(EVT_TestEvent, TestComponent::onEventTest);
	}

	virtual ~TestComponent(){}

	void onEventTest(const GameEvent& evt)
	{
		string event = evt.event;
		event = event;
		LOGE("onEventTest evt=%s", evt.event.c_str());

		int hp = atoi(getOwner()->getProperties().getValue("hp"));
		int mp = atoi(getOwner()->getProperties().getValue("mp"));
		LOGE("Prop: hp=%d, mp=%d", hp, mp);
	}

protected:
private:
};

const string TestComponent::type = "TestComponent";


//////////////////////////////////////////////////////////////////////////

void execServiceOnInputCmd(ScriptObject* pScript, const string& cmd)
{
	if (cmd.empty())
		return;

	// 事件测试
	{
		//GameObject gameObj;
		//gameObj.getProperties().addValue("hp", 100);
		//gameObj.getProperties().addValue("mp", 200);
		//gameObj.createComponent<TestComponent>();
		//gameObj.notify(EVT_TestEvent);
	}

	//for (int i=0; i<1; i++) {
	//	ScriptObject* pScript2 = ScriptManager::getInstance().createScript();
	//	//ScriptManager::getInstance().destroyScript(pScript2);
	//}

	pScript = ScriptManager::getInstance().createScript();
	printf("\n=============== 开始执行脚本 ================\n");
	pScript->dofile("./datas/scripts/server_service.lua");
	pScript->dostring( strformat("ServerService_OnInputCmd(\"%s\")", cmd.c_str()) );
	ScriptManager::getInstance().printInfo();
	printf("\n=============== 结束脚本执行 ================\n");
	ScriptManager::getInstance().destroyScript(pScript);
}

//////////////////////////////////////////////////////////////////////////

ServerServiceAsync::ServerServiceAsync()
	: NetService("ServerServiceAsync")
{
	mLocalPort = 0;
	mMaxClient = 0;
}

ServerServiceAsync::~ServerServiceAsync()
{
}

void ServerServiceAsync::onInputCmd(const string& cmd)
{
	if (cmd == "i") {
		printf("task: %d\n", TCPTaskManager::getInstance().size());
	}

	// 执行lua函数
	FUNC_PF("脚本执行时间");
	execServiceOnInputCmd(NULL, cmd);
}

bool ServerServiceAsync::onInitialise()
{ 
	// 初始化minidump
	MiniDump::InitMiniDump("./crashlog/", mName.c_str());

	// 初始化日志
	string strLogFile = strformat("./log/%s/%s", mName.c_str(), mName.c_str());
	ServerLogger::getInstance().start(strLogFile.c_str(), "super", true);


	loadConfig();

	mLocalPort = 7102;

	// 开启服务
	addTimer(new TimerForMain);
	start( getLocalPort() );
	updateWindowTitle();
	LOGI("开始监听端口 %d", getLocalPort());

	return true;
}

void ServerServiceAsync::onShutdown()		
{
	ServerLogger::getInstance().stop();
}

bool ServerServiceAsync::loadConfig()
{
	pugi::xml_document doc;
	if (!doc.load_file("./config/config.xml")) {
		LOGE("加载config.xml文件失败");
		return false;
	}

	pugi::xml_node node = doc.child("Config").child("Global").child("SuperServer");
	mLocalPort = atoi(node.attribute("port").value());
	mMaxClient = atoi(node.attribute("maxclient").value());

	return true;
}

void ServerServiceAsync::updateWindowTitle()
{
	string strTitle = strformat("ServerService %d",getLocalPort());
	strTitle = Platform::utf8ToGbk(strTitle);
	Platform::setWindowTitle(strTitle.c_str());
}


//////////////////////////////////////////////////////////////////////////


ServerServiceSync::ServerServiceSync()
	: NetService("ServerServiceSync")
{
	mLocalPort = 0;
	mMaxClient = 0;
}

ServerServiceSync::~ServerServiceSync()
{
}

void ServerServiceSync::onInputCmd(const string& cmd)
{
	if (cmd == "i") {
		printf("task: %d\n", TCPTaskManager::getInstance().size());
	}

	// 执行lua函数
	FUNC_PF("脚本执行时间");
	execServiceOnInputCmd(NULL, cmd);
}

bool ServerServiceSync::onInitialise()
{ 
	// 初始化minidump
	MiniDump::InitMiniDump("./crashlog/", mName.c_str());

	// 初始化日志
	string strLogFile = strformat("./log/%s/%s", mName.c_str(), mName.c_str());
	ServerLogger::getInstance().start(strLogFile.c_str(), "super", true);


	loadConfig();

	mLocalPort = 7102;

	// 开启服务
	addTimer(new TimerForMain);
	start( getLocalPort() );
	updateWindowTitle();
	LOGI("开始监听端口 %d", getLocalPort());

	return true;
}

void ServerServiceSync::onShutdown()		
{
	ServerLogger::getInstance().stop();
}

bool ServerServiceSync::loadConfig()
{
	pugi::xml_document doc;
	if (!doc.load_file("./config/config.xml")) {
		LOGE("加载config.xml文件失败");
		return false;
	}

	pugi::xml_node node = doc.child("Config").child("Global").child("SuperServer");
	mLocalPort = atoi(node.attribute("port").value());
	mMaxClient = atoi(node.attribute("maxclient").value());

	return true;
}

void ServerServiceSync::updateWindowTitle()
{
	string strTitle = strformat("ServerService %d",getLocalPort());
	strTitle = Platform::utf8ToGbk(strTitle);
	Platform::setWindowTitle(strTitle.c_str());
}
