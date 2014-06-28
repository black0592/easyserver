#include "ServerHeader.h"
#include "TimerForMain.h"

TimerForMain::TimerForMain()
:TimerEvent(0, true),
mTenSecTimer(10*1000)
{

}

TimerForMain::~TimerForMain()
{

}

bool TimerForMain::onTimer()
{
	if (mTenSecTimer.isTimeout()) {
		ZoneManager::getInstance().refreshZoneInfoToLogin();
	}

	return true;
}

