#ifndef __SUPER_TASK_H__
#define __SUPER_TASK_H__

class TestTask : public TCPTaskAsync
{
public:
	TestTask();
	virtual ~TestTask();

	// ��Ϣ���ݴ���
	//virtual bool handleMessage(const void *cmd, int cmdLen);

	virtual bool handleMessage(const void *cmd, int cmdLen);

	//////////////////////////////////////////////////////////////////////////
	// ���ӶϿ�ʱ������
	virtual void OnDisconnect();


};




#endif