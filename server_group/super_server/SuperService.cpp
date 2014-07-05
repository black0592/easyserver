#include "SuperHeader.h"
#include "SuperService.h"
#include "TimerForMain.h"

SuperService::SuperService()
	: NetService("superserver")
{
	mLocalPort = 0;
	mMaxClient = 0;
}

SuperService::~SuperService()
{
}

void SuperService::onInputCmd(const string& cmd)
{
	if (cmd == "i") {
		printf("task: %d\n", TCPTaskManager::getInstance().size());
	}

	// lua����
	{
		ScriptObject* pScript = ScriptManager::getInstance().createScript();
		FUNC_PF("�ű�ִ��ʱ��");
		printf("\n=============== ��ʼִ�нű� ================\n");
		pScript->dofile("./datas/scripts/test.lua");
		int count = 0;
		for (int i=0; i<1; i++) {
			//pScript->dostring("main_test()");
			count++;
		}
		ScriptManager::getInstance().printInfo();
		printf("\n=============== �����ű�ִ�� ================\n");
		ScriptManager::getInstance().destroyScript(pScript);
	}
}

bool SuperService::onInitialise()
{ 
	// ��ʼ��minidump
	MiniDump::InitMiniDump("./crashlog/", mName.c_str());

	// ��ʼ����־
	string strLogFile = strformat("./log/%s/%s", mName.c_str(), mName.c_str());
	SuperLogger::getInstance().start(strLogFile.c_str(), "super", true);


	loadConfig();


	// ��������
	addTimer(new TimerForMain);
	start( getLocalPort() );
	updateWindowTitle();
	LOGI("��ʼ�����˿� %d", getLocalPort());

	return true;
}

void SuperService::onShutdown()		
{
	SuperLogger::getInstance().stop();
}

bool SuperService::loadConfig()
{
	pugi::xml_document doc;
	if (!doc.load_file("./config/config.xml")) {
		LOGE("����config.xml�ļ�ʧ��");
		return false;
	}

	pugi::xml_node node = doc.child("Config").child("Global").child("SuperServer");
	mLocalPort = atoi(node.attribute("port").value());
	mMaxClient = atoi(node.attribute("maxclient").value());

	return true;
}

void SuperService::updateWindowTitle()
{
	string strTitle = strformat("SuperServer %d",getLocalPort());
	strTitle = Platform::utf8ToGbk(strTitle);
	Platform::setWindowTitle(strTitle.c_str());
}