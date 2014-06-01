#pragma once

#include "BaseTypes.h"
#include "BaseMisc.h"

class TraversalNote
{
public:
	TraversalNote(void);
	~TraversalNote(void);
	
	void reset();

	// 是否在开放列表
	bool noteOpen;
	// 是否在关闭列表
	bool noteClosed;
	// 距离消耗
	int cost;
	// 节点得分
	int score;
	// 父节点
	TraversalNote* parent;
	// 坐标
	POINT point;
};
