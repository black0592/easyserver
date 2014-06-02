#ifndef __SUPER_TASK_H__
#define __SUPER_TASK_H__


class SuperTask : public ProtoTaskAsync
{
public:
	SuperTask();
	virtual ~SuperTask();

	// 消息数据处理
	//virtual bool handleMessage(const void *cmd, int cmdLen);

	virtual bool handleProtoMsg(const EventArgs& args);

	//////////////////////////////////////////////////////////////////////////
	// 连接断开时被调用
	virtual void OnDisconnect();

private:
	uint64 mUniqueId;
};




#endif