#include "ServerHeader.h"

#if 0
#include "vld.h"
#endif


//class ObjPoolTest : public ObjectPool<ObjPoolTest>
//{
//public:
//	ObjPoolTest();
//	virtual ~ObjPoolTest();
//
//protected:
//private:
//};


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

	if (isAsyncServer) {
		// �첽����
		runAsyncServer();
	} else {
		// ͬ������
		runSyncServer();
	}

	return 0;
}



