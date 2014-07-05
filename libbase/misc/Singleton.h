#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <set>
#include "thread/Mutex.h"

namespace easygame {

	// 这种设计方式，有几个好处
	// 1. 单例的实例类型不受模版参数的影响，可以很方便的进行继承和扩展
	// 2. 可以控制单例对象的创建顺序

	class SingletonRleaser;

	template <typename T>
	class Singleton
	{
	public:
		typedef Singleton<T> Base;

		Singleton(int priority = 0)
		{
			//assert(nullptr == msInstance, "Singleton instance " << getClassTypeName() << " already exsist");
			msInstance = static_cast<T*>(this);

			getSingletonRleaser().push(&destroy, priority);
		}

		virtual ~Singleton()
		{
			//assert(nullptr != msInstance, "Destroying Singleton instance " << getClassTypeName() << " before constructing it.");
			msInstance = 0;
		}

		static void create()
		{
			if (msInstance == 0) {
				msInstance = new T;
			}
		}

		static void destroy()
		{
			if(msInstance)
			{
				//msInstance->shutdown();
				delete msInstance;
				msInstance = 0;
			}
		}

		static T& getInstance()
		{
			//assert(nullptr != getInstancePtr(), "Singleton instance " << getClassTypeName() << " was not created");
			return (*getInstancePtr());
		}

		static T* getInstancePtr()
		{
			if (msInstance == 0) {
				create();
			}

			return msInstance;
		}

		static bool hasInstance()
		{
			return msInstance != 0;
		}

		static const char* getClassTypeName()
		{
			return mClassTypeName;
		}

	private:
		static T* msInstance;

	protected:
		static const char* mClassTypeName;
	};

	template <typename T> T* Singleton <T>::msInstance = 0;


	//////////////////////////////////////////////////////////////////////////
	// 自动释放机制
	class SingletonRleaser
	{
		//template <class T> friend class Singleton;
	public:
		typedef void (*DESTROY_FUNC)();

		struct stFuncData
		{
			DESTROY_FUNC pFunc;
			int priority;
		};

		// 比较函数
		struct stFuncCompare
		{
			bool operator()(const stFuncData& s1, const stFuncData& s2) const
			{
				return s1.priority > s2.priority;  // 比较优先级
			}
		};

	public:
		~SingletonRleaser();
		void push(DESTROY_FUNC func, int priority = 0);
		void manualRelease();

	private:
		Mutex mMutex;
		std::multiset<stFuncData,stFuncCompare> m_funcs;
	};

	inline SingletonRleaser::~SingletonRleaser()
	{
		manualRelease();
	}

	inline void SingletonRleaser::manualRelease()
	{
		mMutex.lock();
		for (auto funcData : m_funcs) {
			(*funcData.pFunc)();
		}
		m_funcs.clear();
		mMutex.unlock();
	}

	inline void SingletonRleaser::push(DESTROY_FUNC func, int priority)
	{
		mMutex.lock();
		stFuncData funcData  = {func, priority};
		m_funcs.insert(funcData);
		mMutex.unlock();
	}

	inline SingletonRleaser & getSingletonRleaser()
	{
		static SingletonRleaser releaser;
		return releaser;
	}


}	// namespace

#endif
