#ifndef	_MUTEX_H_
#define _MUTEX_H_

#include <mutex>
#include "misc/Noncopyable.h"

namespace easygame {

	/*
	exp1: 
		// »á×èÈû
		lock();
		....
		unlock();

	exp2: 
		// ²»×èÈû
		if ( trylock() ) {
			....
			unlock();
		}
	*/

	class Mutex : private  Noncopyable
	{
		friend class Cond;

	public:
		Mutex()
		{
		}

		virtual ~Mutex()
		{
		}

		bool trylock()
		{
			return mMutex.try_lock();
		}

		void lock()
		{
			mMutex.lock();
		}

		void unlock()
		{
			mMutex.unlock();
		}

	protected:
		std::mutex mMutex;
	};

	class MutexScopeLock
	{
	private:
		Mutex& mMutex;

	public:
		MutexScopeLock(Mutex& mutex) : mMutex(mutex)
		{
			mMutex.lock();
		}

		~MutexScopeLock()
		{
			mMutex.unlock();
		}
	};

}	// namespace


#endif