#pragma once

// 异步网络任务
class ServerTaskAsync : public ProtoTaskAsync
{
public:
	ServerTaskAsync();
	virtual ~ServerTaskAsync();

	virtual bool handleProtoMsg(const EventArgs& args);

	//////////////////////////////////////////////////////////////////////////
	// 连接断开时被调用
	virtual void OnDisconnect();

private:
	uint64 mUniqueId;
};


