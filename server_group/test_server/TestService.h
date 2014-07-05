#ifndef __SUPER_SERVICE_H__
#define __SUPER_SERVICE_H__
#include "TestTask.h"

class TestService : 
	public NetService<TestTask>, 
	public Singleton<TestService>
{
public:
	TestService();
	virtual ~TestService();
	virtual bool onInitialise();
	virtual void onShutdown();
	bool loadConfig();
	// ���´��ڱ�����Ϣ
	void updateWindowTitle();

	int getLocalPort() { return mLocalPort; }
	int getMaxClient() { return mMaxClient; }

protected:
	int mLocalPort;
	int mMaxClient;

private:
};




#endif