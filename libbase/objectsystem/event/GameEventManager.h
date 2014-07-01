#pragma once
#include "GameEvent.h"
#include <vector>
using namespace std;

namespace easygame {

class GameEventManager
{
public:
	GameEventManager();
	virtual ~GameEventManager();

	void addEventListener(GameEventListener* pEvent);
	void removeEventListener(GameEventListener* pEvent);

protected:
private:
	vector<GameEventListener*> mListenerList;
};

}	// namespace