#pragma once

class TimerForMain : 
	public TimerEvent
{
public:
	TimerForMain();
	virtual ~TimerForMain();

protected:
	virtual bool onTimer();


private:
	ElapseTimer mTenSecTimer;
};

