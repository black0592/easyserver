#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

#include <assert.h>
#include "BaseTypes.h"
#include "thread/Thread.h"
#include <vector>

namespace easygame {

// �Ƿ��ӡ��־��Ϣ
#define ENABLE_LOG		0
// �Ƿ񻺴�
#define ENABLE_CACHE	0

	/**
	����أ�����һЩ�����������ͷŵĶ��󣬴ﵽ��߷�������
	����汾��Ϊ�ͷŻ���һЩ���⣬���ǵ���������Խ��Խ��
	�Ƿ���Ҫ���Լ���ϵͳ�м������أ��д�����
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
			//����ͷ4���ֽڱ���ǿ�С�����Ϊָ����һ�ڴ���ָ�룬����mFreePointer��ָ������һ���ڴ�飬�Ա��´η���ʹ�á�
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
		// Ϊ��֧�ּ��װ��ڴ�й¶���ģ��
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
			//ǿ��ת��Ϊ˫ָ�룬�⽫�ı�ÿ���ڴ��ͷ4���ֽڵĺ��塣
			byte **cur = reinterpret_cast<byte**>(mFreePointer);
			byte *next = mFreePointer;
			for(int i = 0; i < BLOCK_NUM-1; i++)
			{
				next += sizeof(T);
				*cur = next;
				//�������������ÿ���ڴ���ͷ4���ֽھͱ���ǿ�С�����Ϊָ����һ���ڴ���ָ�룬 ���γ����ڴ�������ṹ��
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
			printf("[%s]���ж������ %d�� ������������ %d\n", mName.c_str(), mObjectPool.size(), mObjCount);
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
				printf("[%s]���ж������ %d�� �½�������� %d\n", mName.c_str(), mObjectPool.size(), mObjCount);
#endif	// ENABLE_LOG

			} else {
				pObj = mObjectPool[mObjectPool.size()-1];
				mObjectPool.pop_back();

#if ENABLE_LOG
				printf("[%s]���ж������ %d�� �ӳ���ȡ���������������� %d\n", mName.c_str(), mObjectPool.size(), mObjCount);
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

			// ���г���2�������Ļ�����ֱ���ͷ�
			if ((int)mObjectPool.size() > mPoolMaxSize) {
				delete pObj;
				printf("[%s] ����������ȫ�����������������ж������ %d�������������� %d\n", mName.c_str(), mObjectPool.size(), mObjCount);
				return;
			}

			mObjectPool.push_back(pObj);

#if ENABLE_LOG
			printf("[%s]���ж������ %d�� ����Żس��У������������� %d\n", mName.c_str(), mObjectPool.size(), mObjCount);
#endif	// ENABLE_LOG
#else
			delete pObj;

#endif	// ENABLE_CACHE
		}

	protected:
	private:
		string mName;
		int mPoolMaxSize;	// ���Ƴ��еĶ����С
		int mObjCount;
		Mutex mLock;
		std::vector<T*> mObjectPool;
	};

#endif


}	// namespace


#endif