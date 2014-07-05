#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <vector>
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

		Singleton(bool autoRelease = true)
		{
			//assert(nullptr == msInstance, "Singleton instance " << getClassTypeName() << " already exsist");
			msInstance = static_cast<T*>(this);

			if (autoRelease) {
				getSingletonRleaser().m_funcs.push_back(&destroy);
			} else {
				autoRelease = autoRelease;
			}
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
		template <class T> friend class Singleton;
	public:
		typedef void (*DESTROY_FUNC)();

	public:
		~SingletonRleaser();
		void push(DESTROY_FUNC func);
		void manualRelease();

	private:
		Mutex mMutex;
		std::vector<DESTROY_FUNC> m_funcs;
	};

	inline SingletonRleaser::~SingletonRleaser()
	{
		manualRelease();
	}

	inline void SingletonRleaser::manualRelease()
	{
		mMutex.lock();
		for(std::vector<DESTROY_FUNC>::const_iterator it=m_funcs.begin();it!=m_funcs.end();++it)
		{
			(**it)();
		}
		m_funcs.clear();
		mMutex.unlock();
	}

	inline void SingletonRleaser::push(DESTROY_FUNC func)
	{
		mMutex.lock();
		m_funcs.push_back(func);
		mMutex.unlock();
	}

	inline SingletonRleaser & getSingletonRleaser()
	{
		static SingletonRleaser releaser;
		return releaser;
	}


}	// namespace

#endif
