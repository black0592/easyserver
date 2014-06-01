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

		//�㲥�źŸ����еȴ���������������̣߳�ʹ��Щ�߳��ܹ���������ִ��
		void broadcast()
		{
			mCond.notify_all();
		}

		//�����źŸ����еȴ���������������̣߳�ʹ��Щ�߳��ܹ���������ִ��
		void signal()
		{
			mCond.notify_one();
		}

		//�ȴ��ض���������������
		void wait(Mutex& mutex)
		{
			mCond.wait(mutex.mMutex);
		}

	private:
		condition_variable_any mCond;
	};


}	// namespace


#endif