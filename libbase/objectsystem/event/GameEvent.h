#pragma once
#include "BaseTypes.h"

namespace easygame {

typedef string EventType;

// ��Ϸ�¼��ṹ��
struct GameEvent
{
	EventType event;	// �¼�����
	uint64 param;		// ������ֵ����
	void* data;			// �������

	GameEvent()
	{
		param = 0;
		data = NULL;
	}
}; 


// ��Ϸ�¼�������(����������Ϸ�¼�)
class GameEventListener
{
public:
	GameEventListener();
	virtual ~GameEventListener();

protected:
private:
};



}	// namespace