#ifndef __SUPER_TASK_H__
#define __SUPER_TASK_H__


class SuperTask : public ProtoTaskAsync
{
public:
	SuperTask();
	virtual ~SuperTask();

	// ��Ϣ���ݴ���
	//virtual bool handleMessage(const void *cmd, int cmdLen);

	virtual bool handleProtoMsg(const EventArgs& args);

	//////////////////////////////////////////////////////////////////////////
	// ���ӶϿ�ʱ������
	virtual void OnDisconnect();

private:
	uint64 mUniqueId;
};




#endif