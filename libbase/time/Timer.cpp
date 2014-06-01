#include "BaseLib.h"
#include "Timer.h"

#pragma warning(disable:4311)

namespace easygame {

	Timer::Timer(const string& name)
		:Thread(name)
	{
	}

	Timer::~Timer(void)
	{
		//stop();

		//MutexScopeLock scopelock(mLock);
		//for (TimerEventMapIter it = mTimerEventMap.begin(); it != mTimerEventMap.end();) {
		//	TimerEvent* event = it->second;
		//	mTimerEventMap.erase(it++);
		//	if (event->mIsAutoDelete) {
		//		SAFE_DELETE(event)
		//	}
		//}
	}

	bool Timer::run()
	{
		while(isRunning()) {
			//MutexScopeLock scopelock(mLock);

			if (mTimerEventMap.empty()) {
				yield();
				continue;
			}

			bool isDeleted = false;
			int64 curTick = Platform::tick();
			for (TimerEventMapIter it = mTimerEventMap.begin(); it != mTimerEventMap.end();) {
				isDeleted = false;
				TimerEvent* event = it->second;
				if (curTick - event->mLastTick > event->mElapse) {
					event->mLastTick = curTick;
					if (!event->onTimer()) {
						// É¾³ý¶¨Ê±
						mTimerEventMap.erase(it++);
						isDeleted = true;
					}
				}

				if (!isDeleted) ++it;
			}

			yield();
		}

		return true;
	}

	void Timer::addTimer(TimerEvent* event)
	{
		//MutexScopeLock scopelock(mLock);

		if (event == NULL)
			return;

		event->mLastTick = Platform::tick();
		mTimerEventMap[event] = event;
	}

	void Timer::deleteTimer(TimerEvent*& event)
	{
		//MutexScopeLock scopelock(mLock);

		mTimerEventMap.erase(event);
		if (event->mIsAutoDelete) {
			SAFE_DELETE(event);
		}
	}


}	// namespace