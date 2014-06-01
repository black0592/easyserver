#ifndef __SUPER_TASK_H__
#define __SUPER_TASK_H__

class TestTask : public TCPTaskAsync
{
public:
	TestTask();
	virtual ~TestTask();

	// 消息数据处理
	//virtual bool handleMessage(const void *cmd, int cmdLen);

	virtual bool handleMessage(const void *cmd, int cmdLen);

	//////////////////////////////////////////////////////////////////////////
	// 连接断开时被调用
	virtual void OnDisconnect();


};




#endif