#pragma once
#include "IMapModel.h"

#define COST_STRAIGHT 10	//水平分值
#define COST_DIAGONAL 14	//斜向分值

class MapModel :
	public IMapModel
{
public:
	MapModel();
	virtual ~MapModel(void);

	void release();

	// 地图数据
	virtual byte** getMap(void) { return _map; }
	virtual void setMap(byte** map, int width, int height);
	// 初始化
	virtual void reset(void);
	// 保存Node
	virtual void setNode(POINT& v, TraversalNote* node);
	// 取出Node
	virtual TraversalNote* getNode(POINT& v);
	/**
	* 提供可遍历的节点
	* 这里提供的是八方向移动
	* @param p	当前节点
	* @return 
	*/
	virtual void getArounds(POINT& v, vector<POINT>& result);

	/**
	* 判断是否结束
	* 
	* @param cur	当前节点
	* @param end	终点
	* @return 	是否相同的布尔值
	* 
	*/
	virtual bool reachEnd(POINT& cur, POINT& end);

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
	virtual int getCostAddon(POINT& start, POINT& cur, POINT& next, POINT& end);

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
	virtual int getScoreAddon(POINT& start, POINT& cur, POINT& next, POINT& end);

	bool isBlock(POINT& end, POINT& cur);
	
public:
	bool diagonal;	// 是否启用斜向移动

private:
	TraversalNote*** noteMap;	//node缓存
	byte** _map;
	int mWidth;
	int mHeight;
};
