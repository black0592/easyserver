#pragma once

// �첽��������
class ServerTaskAsync : public ProtoTaskAsync
{
public:
	ServerTaskAsync();
	virtual ~ServerTaskAsync();

	virtual bool handleProtoMsg(const EventArgs& args);

	//////////////////////////////////////////////////////////////////////////
	// ���ӶϿ�ʱ������
	virtual void OnDisconnect();

private:
	uint64 mUniqueId;
};


