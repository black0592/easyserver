#include "ClientHeader.h"
#include "TimeForMainLoop.h"
#include "GMCmd.h"

TimeForMainLoop::TimeForMainLoop(uint elapse)
: TimerEvent(elapse, true)
{
}

TimeForMainLoop::~TimeForMainLoop(void)
{
}

bool TimeForMainLoop::onTimer()
{
	FUNC_PF_EXT("��ѭ��", 100);

	timeForGmCommand();

	return true;
}

void TimeForMainLoop::timeForGmCommand()
{
	// ִ��gm����(����̨����)
	GMCmdProcesser::execCommandList();
}



