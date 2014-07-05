#pragma once
#include "GameObject.h"
#include <map>
#include <functional>

typedef string	ComponentType;

namespace easygame {

	class GameObject;

	// 游戏事件处理函数
	typedef std::function<void (const GameEvent& evt)> GameEventFunc;

	// 便利的事件绑定宏定义
	#define BindGameEvent(event, func) bindGameEvent(event, std::bind(&func, this, std::placeholders::_1));


	// 游戏功能组件基础
	class Component
	{
		friend class GameObject;
	public:

		//static const string type;

		Component();
		virtual ~Component();

		void setOwner(GameObject* owner);

		GameObject* getOwner();

		// 绑定事件函数
		void bindGameEvent(const EventType& event, GameEventFunc func);

		// 把内存数据序列化到protobuf
		//virtual bool SerializeToProto(CharBinaryData& data) { return true; }

		// 从protobuf序列化到内存
		//virtual bool SerializeFromProto(const CharBinaryData& data) { return true; }

		// 接收到某一个游戏事件,注意不要函数内部调用notify函数
		virtual void onGameEvent(const GameEvent& evt) { }

	private:
		// 接收到某一个游戏事件,注意不要函数内部调用notify函数
		void runGameEventFunc(const GameEvent& evt);

	private:
		GameObject* mOwner;
		std::map<EventType, GameEventFunc> mGameEvtFuncMap;
	};



}	// namespace