#include "ServerHeader.h"

#if 0
#include "vld.h"
#endif


void runAsyncServer()
{
	// �����첽������
	ServerServiceAsync service;
	service.main();
}

void runSyncServer()
{
	// ����ͬ��������
	ServerServiceSync service;
	service.main();
}

int main(int argc, char *argv[])
{
	bool isAsyncServer = false;

	// ���ýű���Ŀ¼
	ScriptManager::getInstance().setSciptRootPath("datas/scripts/");

	if (isAsyncServer) {
		// �첽����
		runAsyncServer();
	} else {
		// ͬ������
		runSyncServer();
	}

	return 0;
}



