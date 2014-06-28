#include "ServerHeader.h"
#include "ServerService.h"
#include "TimerForMain.h"

ServerServiceAsync::ServerServiceAsync()
	: NetService("ServerServiceAsync")
{
	mLocalPort = 0;
	mMaxClient = 0;
}

ServerServiceAsync::~ServerServiceAsync()
{
	shutdown();
	ShutdownProtobufLibrary();
}

void ServerServiceAsync::onInputCmd(const string& cmd)
{
	if (cmd == "i") {
		printf("task: %d\n", TCPTaskManager::getInstance().size());
	}

	// lua测试
	{
		ScriptObject* pScript = ScriptManager::getInstance().createScript();
		FUNC_PF("脚本执行时间");
		printf("\n=============== 开始执行脚本 ================\n");
		pScript->dofile("./datas/scripts/test.lua");
		int count = 0;
		for (int i=0; i<1; i++) {
			//pScript->dostring("main_test()");
			count++;
		}
		ScriptManager::getInstance().printInfo();
		printf("\n=============== 结束脚本执行 ================\n");
		ScriptManager::getInstance().destroyScript(pScript);
	}
}

bool ServerServiceAsync::initialise()
{ 
	// 初始化minidump
	MiniDump::InitMiniDump("./crashlog/", mName.c_str());

	// 初始化日志
	string strLogFile = strformat("./log/%s/%s", mName.c_str(), mName.c_str());
	ServerLogger::getInstance().start(strLogFile.c_str(), "super", true);


	loadConfig();


	// 开启服务
	addTimer(new TimerForMain);
	start( getLocalPort() );
	updateWindowTitle();
	LOGI("开始监听端口 %d", getLocalPort());

	return true;
}

bool ServerServiceAsync::shutdown()		
{
	ServerLogger::getInstance().stop();

	return true;
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
	string strTitle = strformat("SuperServer %d",getLocalPort());
	strTitle = Platform::utf8ToGbk(strTitle);
	Platform::setWindowTitle(strTitle.c_str());
}