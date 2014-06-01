#include "ClientHeader.h"
#include "Map.h"
#include <math.h>
#include "MapTable.h"

using namespace Protocol;

#define MAP_FILE_PATH "datas/maps"

//求两点间距离
__inline double dist(int x1, int y1, int x2, int y2)
{
	return sqrt(double((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)));
}

// 客户端格子结构
#pragma pack(1)
#define OBJECTNUMINTILE 4
typedef struct _bPOINT
{
	byte x;
	byte y;
} bPOINT;

typedef struct _wPOINT
{
	ushort x;
	ushort y;
} wPOINT;

typedef struct _tile
{
	byte    header[4];
	byte	Attrib;							//tile attrib
	bPOINT	ptTile;							//pt tile
	byte	sourceTile;						//res num (tile)
	bPOINT	ptObject[OBJECTNUMINTILE];		//pt object
	byte	sourceObject;					//res num (obj)
	byte	Dungon;							//loading dgn num
	wPOINT	ptLoading;						//loading point
	byte    tail[2];
} TILE;

#pragma pack()

class CTile : public TILE
{
public:
	CTile()
	{
		Attrib=CELL_FLAG_PASS;
		sourceTile=0;
		sourceObject=0;
		ptTile.x=0;
		ptTile.y=0;
		for(int i=0;i<OBJECTNUMINTILE;i++)
		{
			ptObject[i].x=0;
			ptObject[i].y=0;
		}
		Dungon=0;
		ptLoading.x=0;
		ptLoading.y=0;
	}

	~CTile(){}
};


POINT offset[] = 
{
	{ 1, 0},	// 右
	{-1, 0},	// 左
	{0,  1},	// 上
	{0, -1},	// 下
	{ 1, 1},	// 右下
	{-1, 1},	// 左下
	{ 1,-1},	// 右上
	{-1,-1},	// 左上
};



Map::Map()
{
	m_mapFile = "";
	m_isLoaded = false;
	m_ppMapData=NULL;
	m_MapPixelSize.cx=0;
	m_MapPixelSize.cy=0;
	m_MapTileSize.cx = 0;
	m_MapTileSize.cy = 0;
	m_pkRect.left = 0;
	m_pkRect.right = 0;
	m_pkRect.top = 0;
	m_pkRect.bottom = 0;
	m_blockData = NULL;
}

Map::~Map()
{
	unloadMapData();
}

void Map::unloadMapData()
{
	if (m_ppMapData != NULL) {
		for (int x=0; x<m_MapTileSize.cx; x++)
			delete []m_ppMapData[x];
		delete []m_ppMapData;
		m_ppMapData = NULL;
	}

	if (m_blockData != NULL) {
		// 释放资源
		for (int x=0; x<m_MapTileSize.cx; x++) {
			delete[] m_blockData[x];
		}
		delete[] m_blockData;
	}

	m_isLoaded = false;
}

SIZE Map::getMapSizePerPixel()
{
	return m_MapPixelSize;
}

SIZE Map::getMapSizePerTile()
{
	return m_MapTileSize;
}

bool Map::isLoaded()
{
	return m_isLoaded;
}

// 采用protobuf格式
bool Map::loadMapData(uint mapId)
{
	if (m_isLoaded)
		return false;

	if (m_ppMapData != NULL)
		return false;

	const Store::stMapInfo* info = MapReader::Instance()->Item(mapId);
	if (info == NULL)
		return false;

	char szMapFile[512];
	sprintf(szMapFile, "%s/%s.map", MAP_FILE_PATH, info->mapfile);
	m_mapFile = szMapFile;

	if (m_mapFile.length() == 0)
		return false;

	
	File file;
	if ( !file.open(m_mapFile.c_str()) ) {
		LOGE("加载地图文件 %s 失败", m_mapFile.c_str());
		return false;
	}

	mMapId = mapId;

	// TODO
	TileMapData tileMapData;
	tileMapData.ParseFromArray(file.getBufferPtr(), file.getLength());

	if (tileMapData.tilewidth() != CXTILE || tileMapData.tileheight() != CYTILE) {
		LOGE("地图的单元格大小不正确 %s", m_mapFile.c_str());
		return false;
	}

	int nx = tileMapData.width();
	int ny = tileMapData.height();
	m_MapPixelSize.cx = nx * tileMapData.tilewidth();
	m_MapPixelSize.cy = ny * tileMapData.tileheight();
	m_MapTileSize.cx = nx;
	m_MapTileSize.cy = ny;

	//memory alloc
	m_ppMapData = new stTILE*[nx];	// 服务器结构
	m_blockData = new byte*[nx];	// 阻挡信息
	int x,y;
	for(x=0; x<nx; x++) {
		m_ppMapData[x] = new stTILE[ny];	// 服务器结构
		m_blockData[x] = new byte[ny];		// 阻挡信息
	}

	int index = 0;
	for(y=0; y<ny; y++) {
		for(x=0; x<nx; x++) {
			const MapCell& cell = tileMapData.cell_list(index++);
			m_ppMapData[x][y].pos.x = x;
			m_ppMapData[x][y].pos.y = y;
			//m_ppMapData[x][y].Attrib=title.Attrib;
			m_ppMapData[x][y].Attrib = cell.flag();	// 临时设置成全通过
			//m_ppMapData[x][y].mapId = title.Dungon;
			//m_ppMapData[x][y].ptLoading.x = title.ptLoading.x;
			//m_ppMapData[x][y].ptLoading.y = title.ptLoading.y;
			m_ppMapData[x][y].itemId=INVALID_ITEM_ID;

			// 生成阻挡信息
			if (cell.flag() == CELL_FLAG_PASS) {
				m_blockData[x][y] = 0;
			} else {
				m_blockData[x][y] = 1;
			}
		}
	}
	
	// 设置阻挡信息
	m_pathFinder.setBlockData(m_blockData, getMapSizePerTile().cx, getMapSizePerTile().cy);

	m_isLoaded = true;

	return true;
}

stTILE* Map::getMapData(ushort x, ushort y)
{
	if(m_ppMapData==NULL)
	{
		return NULL;
	}

	if (m_MapTileSize.cx <= x || m_MapTileSize.cy <= y)
	{
		return NULL;
	}

	return &m_ppMapData[x][y];
}

bool Map::isCanMove(ushort x, ushort y, bool isCharMove)
{
	// 暂时整个地图都能走
	//return true;

	if(m_ppMapData==NULL)
	{
		return false;
	}

	if (m_MapTileSize.cx <= x || m_MapTileSize.cy <= y)
	{
		return false;
	}

	stTILE* tile = &m_ppMapData[x][y];
	if (isCharMove) {
		if (tile->Attrib == CELL_FLAG_BLOCK)
			return false;
	} else {
		if (tile->Attrib != CELL_FLAG_PASS)
			return false;
	}

	return true;
}

stTILE* Map::getRandTile(int x1, int y1, int x2, int y2)
{
	if (x1 < 0) x1 = 0;
	if (x1 >= m_MapTileSize.cx) x1 = m_MapTileSize.cx - 1;
	if (x2 < 0) x2 = 0;
	if (x2 >= m_MapTileSize.cx) x2 = m_MapTileSize.cx - 1;
	if (y1 < 0) y1 = 0;
	if (y1 >= m_MapTileSize.cy) y1 = m_MapTileSize.cy - 1;
	if (y2 < 0) y2 = 0;
	if (y2 >= m_MapTileSize.cy) y2 = m_MapTileSize.cy - 1;

	stTILE* tile = NULL;
	int count = 0;
	int x = 0;
	int y = 0;
	while(count++ < 500) {
		x = Rand::randRange(x1,x2);
		y = Rand::randRange(y1,y2);
		tile = getMapData(x, y);
		if (tile == NULL)
			continue;

		// 刷怪暂时不判断阻挡信息
		//if (tile->Attrib != ATTRIB_PASS)
		//	continue;

		return tile;
	}

	return NULL;
}

stTILE* Map::getSideTile(int x, int y)
{
	if (x < 0) x = 0;
	if (x >= m_MapTileSize.cx) x = m_MapTileSize.cx - 1;
	if (y < 0) y = 0;
	if (y >= m_MapTileSize.cy) y = m_MapTileSize.cy - 1;

	const int RANGE = 10;
	stTILE* tile = NULL;
	stTILE* tileRtn = NULL;
	for (int i=1; i<RANGE; i++) {
		// 左
		tile = getMapData(x-i, y);
		if (tile != NULL && 
			tile->Attrib == CELL_FLAG_PASS &&
			tile->itemId == INVALID_ITEM_ID) 
		{
			tileRtn = tile;
			break;
		}

		// 右
		tile = getMapData(x+i, y);
		if (tile != NULL && 
			tile->Attrib == CELL_FLAG_PASS &&
			tile->itemId == INVALID_ITEM_ID) 
		{
			tileRtn = tile;
			break;
		}

		// 上
		tile = getMapData(x, y-i);
		if (tile != NULL && 
			tile->Attrib == CELL_FLAG_PASS &&
			tile->itemId == INVALID_ITEM_ID) 
		{
			tileRtn = tile;
			break;
		}

		// 下
		tile = getMapData(x, y+i);
		if (tile != NULL && 
			tile->Attrib == CELL_FLAG_PASS &&
			tile->itemId == INVALID_ITEM_ID) 
		{
			tileRtn = tile;
			break;
		}

		// 左上
		tile = getMapData(x-i, y-i);
		if (tile != NULL && 
			tile->Attrib == CELL_FLAG_PASS &&
			tile->itemId == INVALID_ITEM_ID) 
		{
			tileRtn = tile;
			break;
		}

		// 左上
		tile = getMapData(x-i, y+i);
		if (tile != NULL && 
			tile->Attrib == CELL_FLAG_PASS &&
			tile->itemId == INVALID_ITEM_ID) 
		{
			tileRtn = tile;
			break;
		}

		// 右上
		tile = getMapData(x+i, y-i);
		if (tile != NULL && 
			tile->Attrib == CELL_FLAG_PASS &&
			tile->itemId == INVALID_ITEM_ID) 
		{
			tileRtn = tile;
			break;
		}

		// 右下
		tile = getMapData(x+i, y+i);
		if (tile != NULL && 
			tile->Attrib == CELL_FLAG_PASS &&
			tile->itemId == INVALID_ITEM_ID) 
		{
			tileRtn = tile;
			break;
		}
	}

	return tileRtn;
}

POINT Map::getShortPos(int startX, int startY, int endX, int endY)
{
	POINT pt = {startX, startY};
	POINT pt1 = {startX, startY};
	POINT pt2 = {startX, startY};
	int count = count_of(offset);
	double minDis = 1000;
	stTILE* tile = NULL;
	for (int i=0; i<count; ++i) {
		pt2.x = pt1.x + offset[i].x;
		pt2.y = pt1.y + offset[i].y;
		tile = getMapData((ushort)pt2.x, (ushort)pt2.y);
		if (tile == NULL) continue;

		// 暂时不判断阻挡
		//if (tile->Attrib != 1) 
		//	continue;

		double dis = ::dist(pt2.x, pt2.y, endX, endY);
		if (dis < minDis) {
			minDis = dis;
			pt = pt2;
		}
	}

	return pt;
}


// 通过地图搜索出一条最短路径
void Map::findPath(int startX, int startY, int endX, int endY, vector<POINT>& vecPath)
{
	if (startX == 0 || startY == 0 || endX == 0 || endY == 0) 
	{
		return;
	}

	if (startX == m_MapTileSize.cx || startY == m_MapTileSize.cy
		|| endX == m_MapTileSize.cx || endY == m_MapTileSize.cy) 
	{
		return;
	}

	POINT start, end;
	start.x = startX;
	start.y = startY;
	end.x = endX;
	end.y = endY;
	m_pathFinder.find(start, end, vecPath);
	if (vecPath.size() > 0) {
		// 去掉自己的坐标
		vecPath.erase(vecPath.begin());
	}
}



//////////////////////////////////////////////////////////////////////////

MapManager::MapManager()
{
	
}

MapManager::~MapManager()
{
	while (!mMap.empty()) {
		map<uint,Map*>::iterator it = mMap.begin();
		delete it->second;
	}

	mMap.clear();
}

Map* MapManager::loadMap(uint mapId)
{
	Map* pMap = getMap(mapId);
	if (pMap != NULL)
		return pMap;

	pMap = new Map;
	if ( !pMap->loadMapData(mapId) )
		return NULL;

	mMap[mapId] = pMap;

	return pMap;
}

Map* MapManager::getMap(uint mapId)
{
	map<uint,Map*>::iterator it = mMap.find(mapId);
	if (it == mMap.end())
		return NULL;

	return it->second;
}


