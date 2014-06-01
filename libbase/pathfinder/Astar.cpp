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

	openNote(start, 0, 0, NULL);//�����׽ڵ�

	while (heapOpenList.length() > 0) {
		if (++curTry > maxTry)
			return 0;

		TraversalNote* cur = (TraversalNote*)heapOpenList.shift();
		//�����ǰ�Ľڵ㣬����������ر��б�
		closeNote(cur);

		POINT curPOINT = cur->point;

		//��������յ�
		if (mapModel->reachEnd(curPOINT,end)) {
			getPath(start, cur, vecResult);
			return (int)vecResult.size();
		}

		vector<POINT> aroundNotes;
		mapModel->getArounds(cur->point, aroundNotes);
		for (size_t i=0; i<aroundNotes.size(); ++i) {
			POINT& p = aroundNotes[i];
			TraversalNote* n = mapModel->getNode(p);
			if (n && n->noteClosed)//�ڹر��б���������
				continue;

			//����F��Gֵ
			int cost = cur->cost + mapModel->getCostAddon(start,cur->point,p,end);
			int score = cost + mapModel->getScoreAddon(start,cur->point,p,end);;
			//����ڵ����ڿ����б���
			if (n && n->noteOpen) {
				//����µ�Gֵ�Ƚڵ�ԭ����GֵС,�޸�F,Gֵ����������
				if (cost < n->cost) {
					n->cost = cost;
					n->score = score;
					n->parent = cur;
					heapOpenList.modify(n);
				}
			} else {
				//������뿪���б� 
				openNote(p, score, cost, cur);
			}
		}
	}

	return 0;
}

// ���뿪���б�
void Astar::openNote(POINT& p, int score, int cost, TraversalNote* parent)
{
	TraversalNote* node = new TraversalNote;
	node->point = p;
	node->score = score;
	node->cost = cost;
	node->parent = parent;

	mapModel->setNode(p,node);

	// node�������heapOpenList(��BinaryHeap����)
	heapOpenList.push(node);

	// ���������ͷ�
	recyList.push_back(node);
}

bool Astar::sortMetord(void* n1, void* n2)
{
	return ((TraversalNote*)n1)->score < ((TraversalNote*)n2)->score;
}








