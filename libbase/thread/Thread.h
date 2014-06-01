#ifndef	_EASY2D_THREAD_H_
#define _EASY2D_THREAD_H_

#include "Cond.h"
#include "misc/Singleton.h"
#include <string>
#include <map>
#include <thread>
using namespace std;


namespace easygame {

	class Thread
	{
	public:
		Thread(const string& name);
		virtual ~Thread();

		const string& getName();
		bool isRunning();
		bool joinable();
		bool start();
		void stop();
		void join();
		void sleep(ulong milliseconds);
		void yield();

	protected:
		//线程主回调函数
		//返回值: true - 继续执行, false - 退出线程
		virtual bool run() = 0;

		virtual void onStart();
		virtual void onStop();

	private:
		static void* threadFunc(void* arg);

	private:
		string mName;
		std::thread mThread;
		bool mIsCreated;
		volatile bool mRunning;
	};
	

	//////////////////////////////////////////////////////////////////////////
	// 线程管理
	class ThreadManager : public Singleton<ThreadManager>
	{
		friend class Thread;

	public:
		ThreadManager();
		virtual ~ThreadManager();

		void stopAllThread();

		void joinAllThread();

	protected:
		void addThread(Thread* thread);
		void removeThread(Thread* thread);

	private:
		typedef map<Thread*,Thread*>		ThreadMap;
		typedef ThreadMap::iterator ThreadMapIter;
		Mutex mThreadMapMutex;
		ThreadMap mThreadMap;
	};


}	// namespace


#endif