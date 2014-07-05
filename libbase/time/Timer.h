#ifndef __TIMER_H__
#define __TIMER_H__

#include "thread/Thread.h"
#include "platform/Platform.h"

namespace easygame {

	// 定时器触发事件
	class TimerEvent
	{
		friend class Timer;
	public:
		TimerEvent(int64 elapse, bool isAutoDelete)
		{
			mElapse = elapse;
			mLastTick = Platform::tick();
			mIsAutoDelete = isAutoDelete;
		}
		virtual ~TimerEvent(){}


	protected:
		// 返回true - 继续定时。  false - 删除定时器
		virtual bool onTimer() = 0;

	private:
		int64 mElapse;		// 定时时间
		int64 mLastTick;	// 记录时间
		bool mIsAutoDelete;	// 是否自动删除
	};


	// 定时器
	class Timer : public Thread
	{
	public:
		Timer(const string& name);
		virtual ~Timer(void);

		typedef map<TimerEvent*,TimerEvent*> TimerEventMap;
		typedef map<TimerEvent*,TimerEvent*>::iterator TimerEventMapIter;

		// 添加定时器
		void addTimer(TimerEvent* event);

		// 删除定时器(注意，不能在定时器onTimer里面删除)
		void deleteTimer(TimerEvent*& event);

		// 删除所有的定时器
		void deleteAllTimer();

	protected:
		virtual bool run();

	private:
		TimerEventMap mTimerEventMap;
		Mutex mLock;
	};


	// 时间流逝定时器
	class ElapseTimer
	{
	public:
		ElapseTimer(int64 timeTick)
		{
			mTimeTick = timeTick;
			mLastTimeTick = Platform::tick();
		}

		bool isTimeout()
		{
			bool timeOut = false;
			int64 curTimeTick = Platform::tick();
			if (curTimeTick - mLastTimeTick > mTimeTick) {
				timeOut = true;
				mLastTimeTick = curTimeTick;
			}

			return timeOut;
		}

	private:
		int64 mLastTimeTick;
		int64 mTimeTick;
	};
	 

}	// namespace

#endif

