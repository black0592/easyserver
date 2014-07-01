#include "Component.h"
#include "logger/Logger.h"

namespace easygame {

	//const string Component::type =  "BaseComponent";

	Component::Component()
		: mOwner(NULL)
	{

	}

	Component::~Component()
	{
		mOwner = NULL;
		mGameEvtFuncMap.clear();
	}

	void Component::setOwner(GameObject* owner)
	{
		mOwner = dynamic_cast<GameObject*>(owner);
	}

	GameObject* Component::getOwner()
	{
		return mOwner;
	}

	void Component::bindGameEvent(const EventType& event, GameEventFunc func)
	{
		auto it = mGameEvtFuncMap.find(event);
		if (it != mGameEvtFuncMap.end()) {
			//BLOGE("��Ϸ�¼������ظ���, event=%d", event);
			BLOGE("��Ϸ�¼������ظ���");
			return;
		}

		// ����Ϸ�¼�����
		mGameEvtFuncMap[event] = func;
	}

	void Component::runGameEventFunc(const GameEvent& evt)
	{
		auto it = mGameEvtFuncMap.find(evt.event);
		if (it == mGameEvtFuncMap.end())
			return;

		// ִ�а󶨵ĺ���
		it->second(evt);
	}


}	// namespace