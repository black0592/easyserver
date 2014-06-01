#include "PathFinder.h"

//PathFinder* PathFinder::m_pathFinder = NULL;

PathFinder::PathFinder(void)
{
	m_mapMode = new MapModel;
	m_astar = new Astar(m_mapMode);
}

PathFinder::~PathFinder(void)
{
	delete m_mapMode;
	m_mapMode = NULL;
	delete m_astar;
	m_astar = NULL;
}

void PathFinder::setBlockData(byte** map, int width, int height)
{
	m_mapMode->setMap(map, width, height);
}

int PathFinder::find(POINT& start, POINT& end, vector<POINT>& vecResult)
{
	if (m_mapMode->isBlock(end, start)) {
		return 0;
	}

	m_astar->find(start, end, vecResult);
	return (int)vecResult.size();
}

//PathFinder* PathFinder::instance()
//{
//	if (m_pathFinder == NULL) m_pathFinder = new PathFinder;
//	return m_pathFinder;
//}
//
//void PathFinder::destroy()
//{
//	delete m_pathFinder;
//	m_pathFinder = NULL;
//}

