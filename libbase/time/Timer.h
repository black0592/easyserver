#ifndef __TIMER_H__
#define __TIMER_H__

#include "thread/Thread.h"
#include "platform/Platform.h"

namespace easygame {

	// ��ʱ�������¼�
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
		// ����true - ������ʱ��  false - ɾ����ʱ��
		virtual bool onTimer() = 0;

	private:
		int64 mElapse;		// ��ʱʱ��
		int64 mLastTick;	// ��¼ʱ��
		bool mIsAutoDelete;	// �Ƿ��Զ�ɾ��
	};


	// ��ʱ��
	class Timer : public Thread
	{
	public:
		Timer(const string& name);
		virtual ~Timer(void);

		typedef map<TimerEvent*,TimerEvent*> TimerEventMap;
		typedef map<TimerEvent*,TimerEvent*>::iterator TimerEventMapIter;

		// ��Ӷ�ʱ��
		void addTimer(TimerEvent* event);

		// ɾ����ʱ��(ע�⣬�����ڶ�ʱ��onTimer����ɾ��)
		void deleteTimer(TimerEvent*& event);

		// ɾ�����еĶ�ʱ��
		void deleteAllTimer();

	protected:
		virtual bool run();

	private:
		TimerEventMap mTimerEventMap;
		Mutex mLock;
	};


	// ʱ�����Ŷ�ʱ��
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

