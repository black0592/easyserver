#include "TestHeader.h"
#include "TestService.h"
#include <iostream>

int main(int argc, char *argv[])
{
	// ����������
	TestService service;
	service.initialise();
	service.start( service.getLocalPort() );
	service.updateWindowTitle();
	printf("��ʼ�����˿� %d \n", service.getLocalPort());


	ScriptObject* pScript = ScriptManager::getInstance().createScript();

	


	string cmd;
	int last_count = -1;
	while (true)
	{
		/*
		cout << "������ָ��(exit - �˳�������)��" << endl;
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


		cout << "������ָ��(exit - �˳�������)��" << endl;
		cout << ">>";
		getline(cin, cmd);

		// lua����
		{
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
		}



	}

	return 0;
}



