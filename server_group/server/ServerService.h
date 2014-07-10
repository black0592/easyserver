#pragma once

// �첽����
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
	// ���´��ڱ�����Ϣ
	void updateWindowTitle();

	int getLocalPort() { return mLocalPort; }
	int getMaxClient() { return mMaxClient; }

protected:
	int mLocalPort;
	int mMaxClient;

private:
};

//////////////////////////////////////////////////////////////////////////

// ͬ������
class ServerServiceSync : 
	public NetService<ServerTaskSync>, 
	public Singleton<ServerServiceSync>
{
public:
	ServerServiceSync();
	virtual ~ServerServiceSync();
	virtual bool onInitialise();
	virtual void onShutdown();
	virtual void onInputCmd(const string& cmd);

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
