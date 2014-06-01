#ifndef	_THREAD_TASK_QUEUE_H_
#define _THREAD_TASK_QUEUE_H_
#include <queue>
#include <list>
#include <vector>
using namespace std;
#include "Thread.h"

namespace easygame {
	/////////////////////////////////////////////////////////////////
	// 线程执行任务

	class ThreadTask
	{
	public:
		ThreadTask(void) : mPriority(0) {}
		virtual ~ThreadTask(void) {}

		// 调用顺序
		// 1. onProcessInThread
		// 2. onProcessInLogic

		// 可以放在线程中处理的任务(只执行一次)，返回true的话，进入第二步
		virtual bool onProcessInThread(void) {return true;}

		// 只能在主循环中处理的任务(只执行一次)，返回true的话，移除队列
		virtual bool onProcessInLogic(void) {return true;}

		void setPriority(int priority) { mPriority = priority; }
		int getPriority() { return mPriority; }

	private:
		int mPriority;
	};



	/////////////////////////////////////////////////////////////////
	// 线程任务队列

	//最大值优先
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