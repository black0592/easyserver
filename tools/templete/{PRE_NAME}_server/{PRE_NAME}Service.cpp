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
	shutdown();
	ShutdownProtobufLibrary();
}

void SuperService::onInputCmd(const string& cmd)
{
	if (cmd == "i") {
		printf("task: %d\n", TCPTaskManager::getInstance().size());
	}
}

bool SuperService::initialise()
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

bool SuperService::shutdown()		
{
	SuperLogger::getInstance().stop();

	return true;
}

bool SuperService::loadConfig()
{
	pugi::xml_document doc;
	if (!doc.load_file("./config/config.xml")) {
		LOGE("�����ļ�ʧ��");
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