#include "TestHeader.h"
#include "TestTask.h"

TestTask::TestTask()
{

}

TestTask::~TestTask()
{

}

// ���ӶϿ�ʱ������
void TestTask::OnDisconnect()
{
	TCPTask::OnDisconnect();
}

bool TestTask::handleMessage(const void *cmd, int cmdLen)
{
	return sendCmd(cmd, cmdLen);
}