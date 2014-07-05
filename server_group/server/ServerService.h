#pragma once

class ServerServiceAsync : 
	public NetService<ServerTaskAsync>, 
	public Singleton<ServerServiceAsync>
{
public:
	ServerServiceAsync();
	virtual ~ServerServiceAsync();
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

