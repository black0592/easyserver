#pragma once
#include "Traversal.h"
#include "BinaryHeap.h"

class Astar : public Traversal
{
public:
	Astar(MapModel* aMapModel, int aMaxTry = 5000);
	~Astar(void);

	void release();

	// 加入开放列表
	void openNote(POINT& p, int score, int cost, TraversalNote* parent);

	int find(POINT& start, POINT& end, vector<POINT>& vecResult);

	static bool sortMetord(void* n1, void* n2);

protected:
	BinaryHeap heapOpenList;
	vector<TraversalNote*> recyList;	// 节点回收列表
};
