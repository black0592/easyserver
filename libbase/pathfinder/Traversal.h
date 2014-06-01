#pragma once
#include "MapModel.h"

/**
* �����㷨����
* ����Դ����ʵ��IMapModel
* 
* @author crackgame
*/	
class Traversal
{
public:
	Traversal(){}
	Traversal(MapModel* aMapModel, int aMaxTry = 5000);
	virtual ~Traversal(void);
	
	// ������������֮���·��
	int find(POINT& start, POINT& end, vector<POINT>& vecValue);

protected:
	// ���뿪���б�
	void openNote(POINT& p, int score, int cost, TraversalNote* parent);
	// ����ر��б�
	void closeNote(TraversalNote* node);
	// ��÷���·��
	void getPath(POINT& start, TraversalNote* node, vector<POINT>& vecResult);

protected:
	int maxTry;
	// ��ͼģ�� 
	IMapModel* mapModel;
	// �����б� 
	vector<TraversalNote*> openList;
};
