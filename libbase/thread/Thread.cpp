#include "Thread.h"
#include "logger/Logger.h"
#include "platform/Platform.h"
#include <iostream>

#define ENALBE_LOG	0

namespace easygame {

	Thread::Thread(const string& name)
	{
		mName = name;
		mIsCreated = false;
		mRunning = false;

		ThreadManager::getInstance().addThread(this);
	}

	Thread::~Thread()
	{
		stop();

		ThreadManager::getInstance().removeThread(this);
	}

	bool Thread::joinable()
	{
		return mThread.joinable();
	}

	bool Thread::isRunning()
	{
		return mRunning;
	}

	bool Thread::start()
	{
		if (mIsCreated)
			return true;
		
		mRunning = true;
		mThread = std::thread( std::bind(&Thread::threadFunc,this) );
		
#if ENALBE_LOG
		cout << "创建线程 name = " << mName << ", id = " << mThread.get_id() << endl;
#endif

		if (!joinable())
			mThread.detach();

		mIsCreated = true;

		return mRunning;
	}

	void Thread::stop()
	{
		if (!mIsCreated)
			return;

		mRunning = false;
		join();
	}

	void Thread::join()
	{
		if (!mIsCreated)
			return;

		if (!joinable())
			return;

#if ENALBE_LOG
		cout << "Thread::join name = " << mName << ", id = " << mThread.get_id() << endl;
#endif

		mThread.join();
		mIsCreated = false;
	}

	void Thread::sleep(ulong milliseconds)
	{
		std::chrono::milliseconds duration(milliseconds);
		std::this_thread::sleep_for(duration);
		//Platform::sleep(milliseconds);
		
	}

	void Thread::yield()
	{
		this->sleep(1);
		// OR
		//std::this_thread::yield();
	}

	void Thread::onStart()
	{
	}
	void Thread::onStop()
	{
#if ENALBE_LOG
		cout << "Thread::onStop name = " << mName << ", id = " << mThread.get_id() << endl;
#endif
	}

	void* Thread::threadFunc(void* arg)
	{
		Thread* lpThis = (Thread*)arg;

		lpThis->onStart();
		while (lpThis->isRunning()) {
			if (!lpThis->run())
				break;
			lpThis->yield();
		}
		lpThis->onStop();

		return 0;
	}
	

	//////////////////////////////////////////////////////////////////////////


	ThreadManager::ThreadManager()
		: Singleton(false)
	{

	}

	ThreadManager::~ThreadManager()
	{

	}

	void ThreadManager::stopAllThread()
	{
		for (ThreadMapIter it = mThreadMap.begin(); it!=mThreadMap.end(); it++) {
			it->second->stop();
		}
	}

	void ThreadManager::joinAllThread()
	{
		for (ThreadMapIter it = mThreadMap.begin(); it!=mThreadMap.end(); it++) {
			it->second->join();
		}
	}

	void ThreadManager::addThread(Thread* thread)
	{
		//保存线程
		mThreadMapMutex.lock();
		mThreadMap[thread] = thread;
		mThreadMapMutex.unlock();
	}

	void ThreadManager::removeThread(Thread* thread)
	{
		//移除线程
		mThreadMapMutex.lock();
		ThreadMapIter itFind = mThreadMap.find(thread);
		if (itFind != mThreadMap.end()) {
			mThreadMap.erase(itFind);
		}
		mThreadMapMutex.unlock();
	}


}	// namespace


