#include "GameObject.h"
#include "debugger/Profiler.h"

namespace easygame {

	GameObject::GameObject()
	{

	}

	GameObject::~GameObject()
	{
		// ɾ�����
		while (!mComponents.empty()) {
			auto it = mComponents.begin();
			delete it->second;
			mComponents.erase(it);
		}
	}

	void GameObject::notify(const GameEvent& evt)
	{
		FUNC_PF_EXT(__FUNCTION__, 5);

		// ֪ͨ��ҵ��������
		for (auto it=mComponents.begin(); it!=mComponents.end(); it++) {
			Component* pComponent = it->second;
			if (pComponent == NULL)
				continue;

			pComponent->runGameEventFunc(evt);
			pComponent->onGameEvent(evt);
		}
	}

	void GameObject::notify(EventType event, uint64 param, void* data)
	{
		GameEvent evt;
		evt.event = event;
		evt.param = param;
		evt.data = data;
		notify(evt);
	}

}	// namespace