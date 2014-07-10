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
	// 建立异步服务器
	ServerServiceAsync service;
	service.main();
}

void runSyncServer()
{
	// 建立同步服务器
	ServerServiceSync service;
	service.main();
}

int main(int argc, char *argv[])
{
	bool isAsyncServer = false;

	if (isAsyncServer) {
		// 异步服务
		runAsyncServer();
	} else {
		// 同步服务
		runSyncServer();
	}

	return 0;
}



