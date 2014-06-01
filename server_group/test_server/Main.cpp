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

		cout << "task: " << TCPTaskManager::getInstance().size() << endl;
		Platform::sleep(2000);
	}

	return 0;
}



