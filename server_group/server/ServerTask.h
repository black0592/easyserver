#pragma once

// 异步网络任务
class ServerTaskAsync : public ProtoTaskAsync
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
class ServerTaskSync : public ProtoTaskSync
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
