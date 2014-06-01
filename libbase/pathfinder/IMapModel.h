#pragma once
#include "BaseTypes.h"
#include "TraversalNote.h"
#include <vector>
using namespace std;
using namespace easygame;

class IMapModel
{
public:
	// 地图数据
	virtual byte** getMap(void) = 0;
	virtual void setMap(byte** map, int width, int height) = 0;
	// 初始化
	virtual void reset(void) = 0;
	// 保存Node
	virtual void setNode(POINT& v, TraversalNote* node) = 0;
	// 取出Node
	virtual TraversalNote* getNode(POINT& v) = 0;
	/**
	* 提供可遍历的节点
	* 这里提供的是八方向移动
	* @param p	当前节点
	* @return 
	*/
	virtual void getArounds(POINT& v, vector<POINT>& result) = 0;

	/**
	* 判断是否结束
	* 
	* @param cur	当前节点
	* @param end	终点
	* @return 	是否相同的布尔值
	* 
	*/
	virtual bool reachEnd(POINT& cur, POINT& end) = 0;

	/**
	* 获得Cost对于父节点Cost的加值
	* 
	* @param start	首节点
	* @param cur	父节点
	* @param next	当前节点
	* @param end	终点
	* @return 
	* 
	*/
	virtual int getCostAddon(POINT& start, POINT& cur, POINT& next, POINT& end) = 0;

/**
	* 获得Score对于Cost的加值
	*  
	* @param start	首节点
	* @param cur	父节点
	* @param next	当前节点
	* @param end	终点
	* @return 
	* 
	*/
	virtual int getScoreAddon(POINT& start, POINT& cur, POINT& next, POINT& end) = 0;
};


