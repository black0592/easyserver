#pragma once
#include "BaseTypes.h"

namespace easygame {

typedef string EventType;

// 游戏事件结构体
struct GameEvent
{
	EventType event;	// 事件类型
	uint64 param;		// 常用数值参数
	void* data;			// 任意参数

	GameEvent()
	{
		param = 0;
		data = NULL;
	}
}; 


// 游戏事件监听者(监听所有游戏事件)
class GameEventListener
{
public:
	GameEventListener();
	virtual ~GameEventListener();

protected:
private:
};



}	// namespace