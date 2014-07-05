#ifndef __SUPER_SERVICE_H__
#define __SUPER_SERVICE_H__

class SuperService : 
	public NetService<SuperTask>, 
	public Singleton<SuperService>
{
public:
	SuperService();
	virtual ~SuperService();
	virtual bool onInitialise();
	virtual void onShutdown();
	virtual void onInputCmd(const string& cmd);

	bool loadConfig();
	// 更新窗口标题信息
	void updateWindowTitle();

	int getLocalPort() { return mLocalPort; }
	int getMaxClient() { return mMaxClient; }

protected:
	int mLocalPort;
	int mMaxClient;

private:
};




#endif