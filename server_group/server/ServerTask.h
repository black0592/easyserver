#pragma once

// �첽��������
class ServerTaskAsync : public ProtoTaskAsync
{
public:
	ServerTaskAsync();
	virtual ~ServerTaskAsync();

	virtual bool handleProtoMsg(const EventArgs& args);


	virtual bool OnConnect();

	// ���ӶϿ�ʱ������
	virtual void OnDisconnect();

private:
	uint64 mUniqueId;
};


//////////////////////////////////////////////////////////////////////////


// ͬ����������
class ServerTaskSync : public ProtoTaskSync
{
public:
	ServerTaskSync();
	virtual ~ServerTaskSync();

	virtual bool handleProtoMsg(const EventArgs& args);

	virtual bool OnConnect();

	// ���ӶϿ�ʱ������
	virtual void OnDisconnect();

private:
	uint64 mUniqueId;
};
