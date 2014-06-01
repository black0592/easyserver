#include "ThreadTaskQueue.h"

namespace easygame {

	ThreadTaskQueue::ThreadTaskQueue()
	{

	}

	ThreadTaskQueue::~ThreadTaskQueue()
	{
		while(!mTaskQueueThread.empty())
		{
			ThreadTask *threadTask = mTaskQueueThread.top();
			mTaskQueueThread.pop();
			delete threadTask;
		}

		while(!mTaskQueueLogic.empty())
		{
			ThreadTask *threadTask = mTaskQueueLogic.top();
			mTaskQueueLogic.pop();
			delete threadTask;
		}
	}

	void ThreadTaskQueue::addTask(ThreadTask* task, int priority)
	{
		mMutexThread.lock();
		task->setPriority(priority);
		mTaskQueueThread.push(task);
		mMutexThread.unlock();
	}

	bool ThreadTaskQueue::processOneTaskThread(void)
	{
		bool processed = false;
		mMutexThread.lock();
		if(mTaskQueueThread.empty())
		{
			mMutexThread.unlock();
			processed = false;
		}
		else
		{
			// 取出任务
			ThreadTask *task = mTaskQueueThread.top();
			mTaskQueueThread.pop();
			mMutexThread.unlock();

			// 处理任务
			if ( task->onProcessInThread() ) {
				// 执行成功，加入逻辑处理队列
				mMutexLogic.lock();
				mTaskQueueLogic.push(task);
				mMutexLogic.unlock();
			} else {
				// 执行失败，加回线程队列
				mMutexThread.lock();
				mTaskQueueThread.push(task);
				mMutexThread.unlock();
			}

			processed = true;
		}
		return processed;
	}

	bool ThreadTaskQueue::processOneTaskLogic(void)
	{
		bool processed = false;
		if ( mMutexLogic.trylock() ) {
			if(mTaskQueueLogic.empty())
			{
				mMutexLogic.unlock();
				processed = false;
			}
			else
			{
				// 取出任务
				ThreadTask *threadTask = mTaskQueueLogic.top();
				mTaskQueueLogic.pop();
				mMutexLogic.unlock();

				// 处理任务
				if ( threadTask->onProcessInLogic() ) {
					// 执行成功，释放内存
					delete threadTask;
				} else {
					// 执行失败，加回逻辑队列
					mMutexLogic.lock();
					mTaskQueueLogic.push(threadTask);
					mMutexLogic.unlock();
				}

				processed = true;
			}
		}

		return processed;
	}


}	// namespace


