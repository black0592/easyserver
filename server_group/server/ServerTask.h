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

// �첽��������
class ServerTaskAsync : public ProtoTask, ServerTaskBase
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
class ServerTaskSync : public ProtoTask, ServerTaskBase
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
