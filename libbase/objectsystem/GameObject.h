#pragma once
#include "event/GameEvent.h"
#include "Component.h"
#include <vector>
#include <map>

namespace easygame {

	class Component;

	class GameObject
	{
	public:
		GameObject();
		virtual ~GameObject();

		//////////////////////////////////////////////////////////////////////////
		// 事件通知

		// 通知有事件发生(同步处理)
		void notify(const GameEvent& evt);

		// 通知有事件发生(同步处理)，简化接口
		void notify(EventType event, uint64 param = 0, void* data = NULL);


		//////////////////////////////////////////////////////////////////////////
		// 组件接口

		// ----------------- 组件相关接口  ----------------------
		inline Component* getComponent(const string& type) const
		{
			auto it = mComponents.find(type);
			if (it == mComponents.end())
				return NULL;

			return it->second;
		}

		template <class T>
		inline void addComponent(T *component)
		{
			assert( !hasComponent() );
			mComponents[T::type] = component;
			mComponents[T::type]->setMaster(this);
		}

		template <class T>
		inline void createComponent()
		{
			assert(mComponents[T::type] == NULL);
			mComponents[T::type] = new T();
			mComponents[T::type]->setMaster(this);
		}

		template <class T>
		inline bool hasComponent() const
		{
			return getComponent(T::type) != NULL;
		}

		template <class T>
		inline T* getComponent() const
		{
			T *component = static_cast<T*>(getComponent(T::type));
			assert(component);
			return component;
		}

	protected:

	private:
		std::map<string,Component*> mComponents;
	};


}	// namespace