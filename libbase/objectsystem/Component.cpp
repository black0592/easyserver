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
			//BLOGE("游戏事件函数重复绑定, event=%d", event);
			BLOGE("游戏事件函数重复绑定");
			return;
		}

		// 绑定游戏事件函数
		mGameEvtFuncMap[event] = func;
	}

	void Component::runGameEventFunc(const GameEvent& evt)
	{
		auto it = mGameEvtFuncMap.find(evt.event);
		if (it == mGameEvtFuncMap.end())
			return;

		// 执行绑定的函数
		it->second(evt);
	}


}	// namespace