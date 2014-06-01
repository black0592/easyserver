#pragma once
#include "MapModel.h"
#include "Astar.h"

class PathFinder
{
public:
	PathFinder(void);
	virtual ~PathFinder(void);

	// PathFinder不负责map内存的释放
	void setBlockData(byte** map, int width, int height);

	int find(POINT& start, POINT& end, vector<POINT>& vecResult);

//	static PathFinder* instance();
//	static void destroy();
//
//
//private:
//	static PathFinder* m_pathFinder;

private:
	Astar* m_astar;
	MapModel* m_mapMode;

};
