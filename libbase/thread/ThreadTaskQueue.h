#ifndef	_THREAD_TASK_QUEUE_H_
#define _THREAD_TASK_QUEUE_H_
#include <queue>
#include <list>
#include <vector>
using namespace std;
#include "Thread.h"

namespace easygame {
	/////////////////////////////////////////////////////////////////
	// �߳�ִ������

	class ThreadTask
	{
	public:
		ThreadTask(void) : mPriority(0) {}
		virtual ~ThreadTask(void) {}

		// ����˳��
		// 1. onProcessInThread
		// 2. onProcessInLogic

		// ���Է����߳��д��������(ִֻ��һ��)������true�Ļ�������ڶ���
		virtual bool onProcessInThread(void) {return true;}

		// ֻ������ѭ���д��������(ִֻ��һ��)������true�Ļ����Ƴ�����
		virtual bool onProcessInLogic(void) {return true;}

		void setPriority(int priority) { mPriority = priority; }
		int getPriority() { return mPriority; }

	private:
		int mPriority;
	};



	/////////////////////////////////////////////////////////////////
	// �߳��������

	//���ֵ����
	struct ThreadTaskPriorityCmp{
		bool operator() (ThreadTask* task1, ThreadTask* task2)
		{
			return task1->getPriority() < task2->getPriority();
		}
	};

	typedef priority_queue<ThreadTask*,vector<ThreadTask*>, ThreadTaskPriorityCmp> TaskPriorityQueue;

	class ThreadTaskQueue
	{
	public:
		ThreadTaskQueue(void);
		virtual ~ThreadTaskQueue(void);
		void addTask(ThreadTask* task, int priority = 0);
		bool processOneTaskThread(void);
		bool processOneTaskLogic(void);
	private:
		TaskPriorityQueue mTaskQueueThread;
		Mutex mMutexThread;

		TaskPriorityQueue mTaskQueueLogic;
		Mutex mMutexLogic;
	};
	

}	// namespace


#endif