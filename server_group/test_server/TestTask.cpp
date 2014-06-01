#include "TestHeader.h"
#include "TestTask.h"

TestTask::TestTask()
{

}

TestTask::~TestTask()
{

}

// 连接断开时被调用
void TestTask::OnDisconnect()
{
	TCPTask::OnDisconnect();
}

bool TestTask::handleMessage(const void *cmd, int cmdLen)
{
	return sendCmd(cmd, cmdLen);
}