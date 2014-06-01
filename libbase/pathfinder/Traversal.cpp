#include "Traversal.h"

Traversal::Traversal(MapModel* aMapModel, int aMaxTry)
: maxTry(aMaxTry)
, mapModel(aMapModel)
{
}

Traversal::~Traversal(void)
{
}

int Traversal::find(POINT& start, POINT& end, vector<POINT>& vecResult)
{
	return 0;
}

void Traversal::openNote(POINT& p, int score, int cost, TraversalNote* parent)
{
	TraversalNote* node = new TraversalNote();
	node->point = p;
	node->score = score;
	node->cost = cost;
	node->parent = parent;

	mapModel->setNode(p,node);

	openList.push_back(node);
}

void Traversal::closeNote(TraversalNote* node)
{
	node->noteOpen = false;
	node->noteClosed = true;
}

void Traversal::getPath(POINT& start, TraversalNote* node, vector<POINT>& vecValue)
{
	POINT cur = node->point;
	while (!mapModel->reachEnd(cur,start))
	{
		vecValue.insert(vecValue.begin(), cur);
		node = node->parent;
		cur = node->point;
	}
	vecValue.insert(vecValue.begin(), start);
}




