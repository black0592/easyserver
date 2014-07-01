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

	// �¼�����
	{
		GameObject gameObj;
		gameObj.getProperties().addValue("hp", 100);
		gameObj.getProperties().addValue("mp", 200);
		gameObj.createComponent<TestComponent>();
		gameObj.notify(EVT_TestEvent);
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

bool ServerServiceAsync::initialise()
{ 
	// ��ʼ��minidump
	MiniDump::InitMiniDump("./crashlog/", mName.c_str());

	// ��ʼ����־
	string strLogFile = strformat("./log/%s/%s", mName.c_str(), mName.c_str());
	ServerLogger::getInstance().start(strLogFile.c_str(), "super", true);


	loadConfig();


	// ��������
	addTimer(new TimerForMain);
	start( getLocalPort() );
	updateWindowTitle();
	LOGI("��ʼ�����˿� %d", getLocalPort());

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
		LOGE("����config.xml�ļ�ʧ��");
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