#include "ClientHeader.h"
#include "TimeForMainLoop.h"

TimeForMainLoop::TimeForMainLoop(uint elapse)
: TimerEvent(elapse, true)
{
}

TimeForMainLoop::~TimeForMainLoop(void)
{
}

bool TimeForMainLoop::onTimer()
{
	FUNC_PF_EXT("主循环", 100);

	timeForGmCommand();

	return true;
}

void TimeForMainLoop::timeForGmCommand()
{
	// 执行gm命令(控制台命令)
	//GMCmdProcesser::execCommandList();
}



