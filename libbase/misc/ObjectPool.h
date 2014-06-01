#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

#include <assert.h>
#include "BaseTypes.h"
#include "thread/Thread.h"
#include <vector>

namespace easygame {

// 是否打印日志信息
#define ENABLE_LOG		0
// 是否缓存
#define ENABLE_CACHE	0

	/**
	对象池：缓存一些经常创建和释放的对象，达到提高分配性能
	这个版本因为释放会有一些问题，考虑到机器性能越来越好
	是否需要在自己的系统中加入对象池，有待测试
	**/
#if 1


//#define THREAD_SAFE

	template <class T, int BLOCK_NUM= 50>
	class ObjectPool
	{
	public:
		static void *operator new(size_t allocLen)
		{
#ifdef THREAD_SAFE
			MutexScopeLock scopeLock(mLock);
#endif

#ifdef _DEBUG
			int objSize = sizeof(T);
			assert(objSize == allocLen);
#endif
			if(!mFreePointer)
				AllocNew();
			byte *rp = mFreePointer;
			//由于头4个字节被“强行”解释为指向下一内存块的指针，这里mFreePointer就指向了下一个内存块，以备下次分配使用。
			mFreePointer = *reinterpret_cast<byte**>(rp);
			mObjCount++;
			return rp;
		}

		static void operator delete(void* dp)
		{
#ifdef THREAD_SAFE
			MutexScopeLock scopeLock(mLock);
#endif

			*reinterpret_cast<byte**>(dp) = mFreePointer;
			mFreePointer = static_cast<byte*>(dp);
			mObjCount--;
		}

		static void *operator new [](size_t allocLen)
		{
			assert(false);
			return NULL;
		}

		static void operator delete [](void *dp)
		{
			assert(false);
		}

		//////////////////////////////////////////////////////////////////////////
		// 为了支持简易版内存泄露检测模块
		static void *operator new(size_t allocLen, const char* file, int line)
		{
			return operator new(allocLen);
		}
		static void operator delete(void* dp, const char* file, int line)
		{
			operator delete(dp);
		}

		static void *operator new [](size_t allocLen, const char* file, int line)
		{
			assert(false);
			return NULL;
		}

		static void operator delete [](void *dp, const char* file, int line)
		{
			assert(false);
		}
		//////////////////////////////////////////////////////////////////////////

	private:
		static void AllocNew()
		{
			mFreePointer = new byte[sizeof(T) * BLOCK_NUM];
			//强制转型为双指针，这将改变每个内存块头4个字节的含义。
			byte **cur = reinterpret_cast<byte**>(mFreePointer);
			byte *next = mFreePointer;
			for(int i = 0; i < BLOCK_NUM-1; i++)
			{
				next += sizeof(T);
				*cur = next;
				//这样，所分配的每个内存块的头4个字节就被“强行“解释为指向下一个内存块的指针， 即形成了内存块的链表结构。
				cur = reinterpret_cast<byte**>(next);
			}
			*cur = 0;
		}
#ifdef THREAD_SAFE
		static Mutex mLock;
#endif
		static byte *mFreePointer;
		static int mObjCount;

	protected:
		~ObjectPool()
		{
		}
	};

#ifdef THREAD_SAFE
	template<class T, int BLOCK_NUM >
	Mutex ObjectPool<T, BLOCK_NUM >::mLock;
#endif

	template<class T, int BLOCK_NUM >
	byte* ObjectPool<T, BLOCK_NUM >::mFreePointer = NULL;

	template<class T, int BLOCK_NUM >
	int ObjectPool<T, BLOCK_NUM >::mObjCount = NULL;

#endif



#if 1

	template<typename T>
	class ObjectCache
	{
	public:
		ObjectCache(const char* name, int poolMaxSize) 
			: mName(name)
			, mObjCount(0)
			, mPoolMaxSize(poolMaxSize)
		{

		}

		virtual ~ObjectCache()
		{
#if ENABLE_CACHE

#if ENABLE_LOG
			printf("[%s]池中对象个数 %d， 已申请对象个数 %d\n", mName.c_str(), mObjectPool.size(), mObjCount);
#endif	// ENABLE_LOG

			while (!mObjectPool.empty()) {
				delete mObjectPool[mObjectPool.size()-1];
				mObjectPool.pop_back();
			}

#endif	// ENABLE_CACHE
		}

		T* newObject()
		{
#if ENABLE_CACHE

			MutexScopeLock scopeLock(mLock);

			T* pObj = NULL;
			if (mObjectPool.empty()) {
				pObj = new T;
				mObjCount++;

#if ENABLE_LOG
				printf("[%s]池中对象个数 %d， 新建对象个数 %d\n", mName.c_str(), mObjectPool.size(), mObjCount);
#endif	// ENABLE_LOG

			} else {
				pObj = mObjectPool[mObjectPool.size()-1];
				mObjectPool.pop_back();

#if ENABLE_LOG
				printf("[%s]池中对象个数 %d， 从池中取出，已申请对象个数 %d\n", mName.c_str(), mObjectPool.size(), mObjCount);
#endif	// ENABLE_LOG

			}

			return pObj;
#else
			return new T;

#endif	// ENABLE_CACHE
		}

		void freeObject(T* pObj)
		{
#if ENABLE_CACHE

			MutexScopeLock scopeLock(mLock);

			// 池中超过2万个对象的话，就直接释放
			if ((int)mObjectPool.size() > mPoolMaxSize) {
				delete pObj;
				printf("[%s] ！！超过安全对象数量！！，池中对象个数 %d，已申请对象个数 %d\n", mName.c_str(), mObjectPool.size(), mObjCount);
				return;
			}

			mObjectPool.push_back(pObj);

#if ENABLE_LOG
			printf("[%s]池中对象个数 %d， 对象放回池中，已申请对象个数 %d\n", mName.c_str(), mObjectPool.size(), mObjCount);
#endif	// ENABLE_LOG
#else
			delete pObj;

#endif	// ENABLE_CACHE
		}

	protected:
	private:
		string mName;
		int mPoolMaxSize;	// 限制池中的对象大小
		int mObjCount;
		Mutex mLock;
		std::vector<T*> mObjectPool;
	};

#endif


}	// namespace


#endif