#define _CRTDBG_MAP_ALLOC
#include "ServerHeader.h"

#include <crtdbg.h>

#if 0
#include "vld.h"
#endif


class ObjPoolTest : public ObjectPool<ObjPoolTest>
{
public:
	ObjPoolTest();
	virtual ~ObjPoolTest();

protected:
private:
};


void runServer()
{
	// ����������
	ServerServiceAsync service;
	service.main();
}

int main(int argc, char *argv[])
{
	runServer();
	//_CrtDumpMemoryLeaks();

	return 0;
}



