#pragma once
#include "MapModel.h"

/**
* 遍历算法基类
* 数据源必须实现IMapModel
* 
* @author crackgame
*/	
class Traversal
{
public:
	Traversal(){}
	Traversal(MapModel* aMapModel, int aMaxTry = 5000);
	virtual ~Traversal(void);
	
	// 计算两个坐标之间的路径
	int find(POINT& start, POINT& end, vector<POINT>& vecValue);

protected:
	// 加入开放列表
	void openNote(POINT& p, int score, int cost, TraversalNote* parent);
	// 加入关闭列表
	void closeNote(TraversalNote* node);
	// 获得返回路径
	void getPath(POINT& start, TraversalNote* node, vector<POINT>& vecResult);

protected:
	int maxTry;
	// 地图模型 
	IMapModel* mapModel;
	// 开放列表 
	vector<TraversalNote*> openList;
};
