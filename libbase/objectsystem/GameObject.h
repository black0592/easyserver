#pragma once
#include "event/GameEvent.h"
#include "Component.h"
#include "data/Properties.h"
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
		// ����

		// �������Զ���
		Properties& getProperties() { return mProperties; }


		//////////////////////////////////////////////////////////////////////////
		// �¼�֪ͨ

		// ֪ͨ���¼�����(ͬ������)
		void notify(const GameEvent& evt);

		// ֪ͨ���¼�����(ͬ������)���򻯽ӿ�
		void notify(EventType event, uint64 param = 0, void* data = NULL);


		//////////////////////////////////////////////////////////////////////////
		// ����ӿ�

		// ----------------- �����ؽӿ�  ----------------------
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
			mComponents[T::type]->setOwner(this);
		}

		template <class T>
		inline void createComponent()
		{
			assert(mComponents[T::type] == NULL);
			mComponents[T::type] = new T();
			mComponents[T::type]->setOwner(this);
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
		// ����б�
		std::map<string,Component*> mComponents;

		// ���Խṹ
		Properties mProperties;
	};


}	// namespace