#pragma once

class ServerTaskBase
{
public:
	ServerTaskBase();
	~ServerTaskBase();

	void createScript();
	void destroyScript();

protected:
	bool execServerTaskOnConnect();
	void execServerTaskOnDisconnect();
	void execServerTaskHandleProtoMsg(const EventArgs& args);

protected:
	ScriptObject* mScript;
};



//////////////////////////////////////////////////////////////////////////

// 异步网络任务
class ServerTaskAsync : public ProtoTaskAsync, ServerTaskBase
{
public:
	ServerTaskAsync();
	virtual ~ServerTaskAsync();

	virtual bool handleProtoMsg(const EventArgs& args);


	virtual bool OnConnect();

	// 连接断开时被调用
	virtual void OnDisconnect();

private:
	uint64 mUniqueId;
};


//////////////////////////////////////////////////////////////////////////


// 同步网络任务
class ServerTaskSync : public ProtoTaskSync, ServerTaskBase
{
public:
	ServerTaskSync();
	virtual ~ServerTaskSync();

	virtual bool handleProtoMsg(const EventArgs& args);

	virtual bool OnConnect();

	// 连接断开时被调用
	virtual void OnDisconnect();

private:
	uint64 mUniqueId;
};
