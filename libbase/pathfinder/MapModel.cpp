#include "MapModel.h"

MapModel::MapModel()
{
	noteMap = NULL;
	_map = NULL;
	
}

MapModel::~MapModel(void)
{
	release();
}

void MapModel::release()
{
	_map = NULL;

	if (noteMap != NULL) {
		for (int x=0; x<mWidth; x++) {
			delete[] noteMap[x];
		}
		delete[] noteMap;
	}
}

void MapModel::setMap(byte** map, int width, int height)
{
	release();

	diagonal = true;
	_map = map;
	mWidth = width;
	mHeight = height;
	if (width != 0 && height != 0) {
		noteMap = new TraversalNote**[width];
		for (int x=0; x<width; ++x) {
			noteMap[x]=new TraversalNote*[height];
		}
	}
}

void MapModel::reset(void)
{
	for(int y=0; y<mWidth; y++) {
		for(int x=0; x<mHeight; x++) {
			noteMap[y][x] = NULL;
		}
	}
}

void MapModel::setNode(POINT& v, TraversalNote* node)
{
	noteMap[v.x][v.y] = node;
}

TraversalNote* MapModel::getNode(POINT& v)
{
	return noteMap[v.x][v.y];
}

bool MapModel::reachEnd(POINT& cur, POINT& end)
{
	return (cur.x == end.x && cur.y == end.y);
}

int MapModel::getCostAddon(POINT& start, POINT& cur, POINT& next, POINT& end)
{
	return (next.x == cur.x || next.y == cur.y) ? COST_STRAIGHT : COST_DIAGONAL;
}

int MapModel::getScoreAddon(POINT& start, POINT& cur, POINT& next, POINT& end)
{
	return (abs(end.x - next.x) + abs(end.y - next.y)) * COST_STRAIGHT;
}

void MapModel::getArounds(POINT& v, vector<POINT>& result)
{
	POINT cur = v;
	POINT p;
	bool canDiagonal;

	//右
	p.x = v.x + 1; p.y = v.y;
	bool canRight = !isBlock(p,cur);
	if (canRight)
		result.push_back(p);

	//下
	p.x = v.x; p.y = v.y + 1;
	bool canDown = !isBlock(p,cur);
	if (canDown)
		result.push_back(p);

	//左
	p.x = v.x - 1; p.y = v.y;
	bool canLeft = !isBlock(p,cur);
	if (canLeft)
		result.push_back(p);

	//上
	p.x = v.x; p.y = v.y - 1;
	bool canUp = !isBlock(p,cur);
	if (canUp)
		result.push_back(p);

	if (diagonal)
	{
		//右下
		p.x = v.x + 1; p.y = v.y + 1;
		canDiagonal = !isBlock(p,cur);
		if (canDiagonal && canRight && canDown)
			result.push_back(p);

		//左下
		p.x = v.x - 1; p.y = v.y + 1;
		canDiagonal = !isBlock(p,cur);
		if (canDiagonal && canLeft && canDown)
			result.push_back(p);

		//左上
		p.x = v.x - 1; p.y = v.y - 1;
		canDiagonal = !isBlock(p,cur);
		if (canDiagonal && canLeft && canUp)
			result.push_back(p);

		//右上
		p.x = v.x + 1; p.y = v.y - 1;
		canDiagonal = !isBlock(p,cur);
		if (canDiagonal && canRight && canUp)
			result.push_back(p);
	}
}

bool MapModel::isBlock(POINT& end, POINT& cur)
{
	int mapHeight = mHeight;
	int mapWidth = mWidth;

	if (end.x < 0 || end.x >= mapWidth || end.y < 0 || end.y >= mapHeight)
		return true;

	//return false;
	return _map[end.x][end.y] == 1;	// 0 - 不阻挡， 1 - 阻挡
}



