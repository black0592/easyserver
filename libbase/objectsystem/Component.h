#pragma once
#include "GameObject.h"
#include <map>
#include <functional>

typedef string	ComponentType;

namespace easygame {

	class GameObject;

	// ��Ϸ�¼�������
	typedef std::function<void (const GameEvent& evt)> GameEventFunc;

	// �������¼��󶨺궨��
	#define BindGameEvent(event, func) bindGameEvent(event, std::bind(&func, this, std::placeholders::_1));


	// ��Ϸ�����������
	class Component
	{
		friend class GameObject;
	public:

		//static const string type;

		Component();
		virtual ~Component();

		void setOwner(GameObject* owner);

		GameObject* getOwner();

		// ���¼�����
		void bindGameEvent(const EventType& event, GameEventFunc func);

		// ���ڴ��������л���protobuf
		//virtual bool SerializeToProto(CharBinaryData& data) { return true; }

		// ��protobuf���л����ڴ�
		//virtual bool SerializeFromProto(const CharBinaryData& data) { return true; }

		// ���յ�ĳһ����Ϸ�¼�,ע�ⲻҪ�����ڲ�����notify����
		virtual void onGameEvent(const GameEvent& evt) { }

	private:
		// ���յ�ĳһ����Ϸ�¼�,ע�ⲻҪ�����ڲ�����notify����
		void runGameEventFunc(const GameEvent& evt);

	private:
		GameObject* mOwner;
		std::map<EventType, GameEventFunc> mGameEvtFuncMap;
	};



}	// namespace