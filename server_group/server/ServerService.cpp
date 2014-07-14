#include "ServerHeader.h"
#include "ServerService.h"
#include "TimerForMain.h"
#include "ProtoSvrLogin.pb.h"
#include "protohandler\ProtobufDefine.h"

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

	// �¼�����
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
	printf("\n=============== ��ʼִ�нű� ================\n");
	pScript->dofile("./datas/scripts/server_service.lua");
	pScript->dostring( strformat("ServerService_OnInputCmd(\"%s\")", cmd.c_str()) );
	ScriptManager::getInstance().printInfo();
	printf("\n=============== �����ű�ִ�� ================\n");
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

	// ִ��lua����
	FUNC_PF("�ű�ִ��ʱ��");
	execServiceOnInputCmd(NULL, cmd);
}

bool ServerServiceAsync::onInitialise()
{ 
	// ��ʼ��minidump
	MiniDump::InitMiniDump("./crashlog/", mName.c_str());

	// ��ʼ����־
	string strLogFile = strformat("./log/%s/%s", mName.c_str(), mName.c_str());
	ServerLogger::getInstance().start(strLogFile.c_str(), "super", true);


	loadConfig();

	mLocalPort = 7102;

	// ��������
	addTimer(new TimerForMain);
	start( getLocalPort() );
	updateWindowTitle();
	LOGI("��ʼ�����˿� %d", getLocalPort());

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

	// ִ��lua����
	FUNC_PF("�ű�ִ��ʱ��");
	execServiceOnInputCmd(NULL, cmd);
}

bool ServerServiceSync::onInitialise()
{ 
	// ��ʼ��minidump
	MiniDump::InitMiniDump("./crashlog/", mName.c_str());

	// ��ʼ����־
	string strLogFile = strformat("./log/%s/%s", mName.c_str(), mName.c_str());
	ServerLogger::getInstance().start(strLogFile.c_str(), "super", true);


	loadConfig();

	mLocalPort = 7102;

	// ��������
	addTimer(new TimerForMain);
	start( getLocalPort() );
	updateWindowTitle();
	LOGI("��ʼ�����˿� %d", getLocalPort());

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
		LOGE("����config.xml�ļ�ʧ��");
		return false;
	}

	pugi::xml_node node = doc.child("Config").child("Global").child("SuperServer");
	mLocalPort = atoi(node.attribute("port").value());
	mMaxClient = atoi(node.attribute("maxclient").value());

	return true;
}


void IterateProtoFile(const std::string& name)
{
	// �� DescriptorPool ��ͨ�� .proto �ļ�����ȡ�� FileDescriptor
	// FileDescriptor �������������� .proto �ļ�
	const google::protobuf::FileDescriptor* fileDescriptor
		= google::protobuf::DescriptorPool::generated_pool()->FindFileByName(name);
	if (!fileDescriptor)
		return;

	// �������� .proto �ļ��ж���Ķ��� message
	for (int i=0; i<fileDescriptor->message_type_count(); ++i)
	{
		// message_type ��������ȡֵ��Χ�� 0 <= index < message_type_count()
		// ����ֵ i ��Ӧ message �� .proto �ļ��еĶ����˳��
		const google::protobuf::Descriptor* descriptor = fileDescriptor->message_type(i);
		descriptor = descriptor;

		// ...
	}
}


void ServerServiceSync::updateWindowTitle()
{
	string strTitle = strformat("ServerService %d",getLocalPort());
	strTitle = Platform::utf8ToGbk(strTitle);
	Platform::setWindowTitle(strTitle.c_str());

	//////////////////////////////////////////////////////////////////////////

	google::protobuf::Message* newQuery = createMessage("LoginCmd.RequestRegisterGameServer");
	assert(newQuery != NULL);
	LoginCmd::RequestRegisterGameServer::default_instance();

	//LoginCmd::RequestRegisterGameServer cmd;

	LoginCmd::RequestRegisterGameServer* pMsg = (LoginCmd::RequestRegisterGameServer*)createMessage("LoginCmd.RequestRegisterGameServer");
	pMsg->set_id(1);

	IterateProtoFile("ProtoSvrLogin.proto");
}
