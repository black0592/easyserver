#ifndef	_COND_H_
#define _COND_H_
#include <stdio.h>
#include "misc/Noncopyable.h"
#include "platform/Platform.h"
#include "Mutex.h"
#include <condition_variable>

namespace easygame {

	class Cond : private  Noncopyable
	{
	public:
		Cond()
		{
		}

		virtual ~Cond()
		{
		}

		//广播信号给所有等待这个条件变量的线程，使这些线程能够继续往下执行
		void broadcast()
		{
			mCond.notify_all();
		}

		//发送信号给所有等待这个条件变量的线程，使这些线程能够继续往下执行
		void signal()
		{
			mCond.notify_one();
		}

		//等待特定的条件变量满足
		void wait(Mutex& mutex)
		{
			mCond.wait(mutex.mMutex);
		}

	private:
		condition_variable_any mCond;
	};


}	// namespace


#endif