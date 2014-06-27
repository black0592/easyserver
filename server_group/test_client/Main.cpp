#include "ClientHeader.h"
#include "TimeForMainLoop.h"

int rand_reconnect = 0;

int main(int argc, char *argv[])
{
	//{
	//	FileSystem::getInstance().detachAllPakFile();
	//	if (!FileSystem::getInstance().attachPakFile(PAK_FILE_MAIN, "127.0.0.1")) {
	//		LOGE("加载Pak文件失败, file=%s", PAK_FILE_MAIN);
	//	}

	//	//BuffReader::Instance();
	//	//CountryReader::Instance();
	//	//SkillBookReader::Instance();
	//	//TextReader::Instance();

	//	EquipmentReader::Instance();
	//	MonsterReader::Instance();

	//	/*
	//	EquipGrooveReader::Instance();
	//	//EquipmentReader::Instance();
	//	EquipUpgradeReader::Instance();
	//	GemAttributeReader::Instance();
	//	GemCompositeReader::Instance();
	//	//ItemReader::Instance();
	//	MainMissionReader::Instance();
	//	MapReader::Instance();
	//	
	//	//NpcReader::Instance();
	//	SkillReader::Instance();
	//	MarriageReader::Instance();
	//	ItemClassifyReader::Instance();
	//	*/

	//	string cmd;
	//	while (true)
	//	{
	//		cout << "请输入指令(exit - 退出服务器)：\n";
	//		cout << ">>";
	//		getline(cin, cmd);
	//		if (cmd == "exit") {
	//			break;
	//		} else if (cmd == "crash") {
	//			char* p = NULL;
	//			*p = 'a';
	//		} else if (cmd == "info") {
	//			//GameService::getInstance().PrintInfo();
	//		} else {
	//			GMCmdProcesser::pushCmd(cmd.c_str());
	//		}
	//	}
	//}

	//return 0;

	//int64 first1 = Platform::tick();
	//int64 first2 = Platform::tick();
	//int a = 0;
	//for (int i=0; i<500000000; i++) {
	//	a = 100%2;
	//}
	//int64 send1 = Platform::tick();
	//int64 send2 = Platform::tick();
	//int64 time1 = send1 - first1;
	//int64 time2 = send2 - first2;
	//printf("tick: %d\n", time1);
	//printf("clock: %d\n", time2);
	//system("pause");
	//return 0;

	if (argc < 3) {
		printf("参数错误 usage: test_client 开始编号 客户端数量 重连概率\n");
		return 0;
	}
	uint start  = atoi(argv[1]);
	uint num  = atoi(argv[2]);
	rand_reconnect = atoi(argv[3]);

	srand( (uint)time(NULL) );


	// 初始化minidump
	MiniDump::InitMiniDump("./crashlog/", "testclient");
	ClientLogger::getInstance().start("./log/client/client", "client", true);

	Platform::setWindowTitle("TestClient");

	Timer mainTimer("TestClient");

	int CLIENT_START = start;

#if 0
	int MAX_CLIENT = 1;
#else
	int MAX_CLIENT = num;
#endif

	mainTimer.addTimer(new TimeForMainLoop(1));
	mainTimer.start();


	string cmd;
	while (true)
	{
		cout << "请输入指令(exit - 退出服务器)：\n";
		cout << ">>";
		getline(cin, cmd);
		if (cmd == "exit") {
			break;
		} else if (cmd == "crash") {
			char* p = NULL;
			*p = 'a';
		} else if (cmd == "info") {
			//GameService::getInstance().PrintInfo();
		} else {
			//GMCmdProcesser::pushCmd(cmd.c_str());
		}
	}


	ClientLogger::getInstance().stop();

	ShutdownProtobufLibrary();

	return 0;
}



