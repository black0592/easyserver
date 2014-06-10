#include "TestHeader.h"
#include "TestService.h"
#include <iostream>

int main(int argc, char *argv[])
{
	// 建立服务器
	TestService service;
	service.initialise();
	service.start( service.getLocalPort() );
	service.updateWindowTitle();
	printf("开始监听端口 %d \n", service.getLocalPort());


	ScriptObject* pScript = ScriptManager::getInstance().createScript();

	


	string cmd;
	int last_count = -1;
	while (true)
	{
		/*
		cout << "请输入指令(exit - 退出服务器)：" << endl;
		cout << ">>";
		getline(cin, cmd);
		if (cmd == "exit") {
			break;
		} else if (cmd == "crash") {
			char* p = NULL;
			*p = 'a';
		} else if (cmd == "info" || cmd == "i" || cmd.empty()) {
			//GameService::getInstance().PrintInfo();
			printf("task: %d\n", TCPTaskManager::getInstance().size());
		}
		*/

		//cout << "task: " << TCPTaskManager::getInstance().size() << endl;
		//Platform::sleep(2000*10);


		cout << "请输入指令(exit - 退出服务器)：" << endl;
		cout << ">>";
		getline(cin, cmd);

		// lua测试
		{
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
		}



	}

	return 0;
}



