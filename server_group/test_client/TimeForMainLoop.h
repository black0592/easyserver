#pragma once

class TimeForMainLoop : public TimerEvent
{
public:
	TimeForMainLoop(uint elapse);
	virtual ~TimeForMainLoop(void);

protected:
	virtual bool onTimer();

	void timeForGmCommand();
};
