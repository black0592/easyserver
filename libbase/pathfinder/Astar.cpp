#include "Astar.h"

Astar::Astar(MapModel* aMapModel, int aMaxTry)
{
	//Traversal(aMapModel, aMaxTry);
	mapModel = aMapModel;
	maxTry = aMaxTry;

	heapOpenList.sortMetord = sortMetord;
}

Astar::~Astar(void)
{
	release();
}

void Astar::release()
{
	while (!recyList.empty()) {
		delete recyList[recyList.size()-1];
		recyList.pop_back();
	}
}

int Astar::find(POINT& start, POINT& end, vector<POINT>& vecResult)
{
	release();

	heapOpenList.clear();

	mapModel->reset();
	int curTry = 0;

	openNote(start, 0, 0, NULL);//建立首节点

	while (heapOpenList.length() > 0) {
		if (++curTry > maxTry)
			return 0;

		TraversalNote* cur = (TraversalNote*)heapOpenList.shift();
		//获得最前的节点，并将它加入关闭列表
		closeNote(cur);

		POINT curPOINT = cur->point;

		//如果到达终点
		if (mapModel->reachEnd(curPOINT,end)) {
			getPath(start, cur, vecResult);
			return (int)vecResult.size();
		}

		vector<POINT> aroundNotes;
		mapModel->getArounds(cur->point, aroundNotes);
		for (size_t i=0; i<aroundNotes.size(); ++i) {
			POINT& p = aroundNotes[i];
			TraversalNote* n = mapModel->getNode(p);
			if (n && n->noteClosed)//在关闭列表中则跳过
				continue;

			//计算F和G值
			int cost = cur->cost + mapModel->getCostAddon(start,cur->point,p,end);
			int score = cost + mapModel->getScoreAddon(start,cur->point,p,end);;
			//如果节点已在开放列表中
			if (n && n->noteOpen) {
				//如果新的G值比节点原来的G值小,修改F,G值，重新排序
				if (cost < n->cost) {
					n->cost = cost;
					n->score = score;
					n->parent = cur;
					heapOpenList.modify(n);
				}
			} else {
				//否则加入开放列表 
				openNote(p, score, cost, cur);
			}
		}
	}

	return 0;
}

// 加入开放列表
void Astar::openNote(POINT& p, int score, int cost, TraversalNote* parent)
{
	TraversalNote* node = new TraversalNote;
	node->point = p;
	node->score = score;
	node->cost = cost;
	node->parent = parent;

	mapModel->setNode(p,node);

	// node对象存入heapOpenList(由BinaryHeap管理)
	heapOpenList.push(node);

	// 用于最后的释放
	recyList.push_back(node);
}

bool Astar::sortMetord(void* n1, void* n2)
{
	return ((TraversalNote*)n1)->score < ((TraversalNote*)n2)->score;
}








