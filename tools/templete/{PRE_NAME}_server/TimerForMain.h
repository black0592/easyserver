#ifndef __TIMER_FOR_MAIN_H__
#define __TIMER_FOR_MAIN_H__

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


#endif