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
			// ȡ������
			ThreadTask *task = mTaskQueueThread.top();
			mTaskQueueThread.pop();
			mMutexThread.unlock();

			// ��������
			if ( task->onProcessInThread() ) {
				// ִ�гɹ��������߼��������
				mMutexLogic.lock();
				mTaskQueueLogic.push(task);
				mMutexLogic.unlock();
			} else {
				// ִ��ʧ�ܣ��ӻ��̶߳���
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
				// ȡ������
				ThreadTask *threadTask = mTaskQueueLogic.top();
				mTaskQueueLogic.pop();
				mMutexLogic.unlock();

				// ��������
				if ( threadTask->onProcessInLogic() ) {
					// ִ�гɹ����ͷ��ڴ�
					delete threadTask;
				} else {
					// ִ��ʧ�ܣ��ӻ��߼�����
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


