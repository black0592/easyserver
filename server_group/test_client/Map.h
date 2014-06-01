#ifndef __CLIENT_MAP_H__
#define __CLIENT_MAP_H__
#include "BaseLib.h"
#include "ProtoGameData.pb.h"

#define SCREEN_WIDTH	960
#define SCREEN_HEIGHT	640


// 是以960*640为例子
// 一屏X方向格子数
#define NXTILE 15
// 一屏Y方向格子数
#define NYTILE 20

// 一格的X，Y像素值
#define CXTILE 64
#define CYTILE 32


//////////////////////////////////////////////////////////////////////////
// duplicate 游戏副本
// mapid - 2字节

#define INVALID_DUPID	0		// 非副本id

#define INVALID_ITEM_ID	0

__inline uint hash_map_id(ushort mapid, ushort dupid)
{
	return dupid << 16 | mapid;
}

__inline uint get_mapid(uint hashMapid)
{
	return (hashMapid & 0x0000FFFF);
}

__inline uint get_dupid(uint hashMapid)
{
	return (hashMapid & 0xFFFF0000) >> 16;
}
//////////////////////////////////////////////////////////////////////////

// 地图坐标转换
__inline uint pixel2CellX(uint x)
{
	return x / CXTILE;
}

__inline uint pixel2CellY(uint y)
{
	return y / CYTILE;
}


__inline uint cell2PixelX(uint x)
{
	return x * CXTILE + CXTILE / 2;
}

__inline uint cell2PixelY(uint y)
{
	return y * CYTILE + CYTILE / 2;
}

//////////////////////////////////////////////////////////////////////////

// 单元格结构
struct stTILE
{
	POINT pos;		// 格子的坐标
	MapCellFlag Attrib;	// 格子属性
	ushort mapId;	// 传送的地图编号
	POINT ptLoading;// 目标地图的坐标

	uint itemId;	// 物品ID号
	bool bIsObject;	// 是否是物体
	ushort CharId;	// 角色ID
	ushort MopId;		// 未知，可能是怪物ID
	stTILE()
	{
		pos.x = pos.y = 0;
		itemId=INVALID_ITEM_ID;
		bIsObject=false;
		Attrib=CELL_FLAG_PASS;
		CharId=(ushort)-1;
		MopId=(ushort)-1;
	}
};

class Item {
public:
	Item(){}
	virtual ~Item(){}
};

struct stMapItem
{
	ushort x;			// 物品在地图上的坐标位置
	ushort y;
	Item* pItem;
	uint dropTick;	// 掉落时的时间
	uint LifeTime;	// 物品的消失时间(秒)

	stMapItem()
	{
		x = y = 0;
		pItem = NULL;
		dropTick = 0;
		LifeTime = 10;
	}
};

class Map
{
public:
	Map();
	~Map();

	typedef map<uint, stMapItem> ItemMap;
	typedef map<uint, stMapItem>::iterator ItemMapIt;

public:

	uint getID() { return mMapId; }

	// 读取地图
	bool loadMapData(uint mapId);

	// 判断地图是否已经被加载
	bool isLoaded();

	// 卸载地图数据
	void unloadMapData();

	// 取得以像素为单位的地图尺寸
	SIZE getMapSizePerPixel();

	// 取得以格子为单位的地图尺寸
	SIZE getMapSizePerTile();

	// 取得地图中物品的数量
	uint getMapItemCount();

	// 取得指定坐标的格子数据
	stTILE* getMapData(ushort x, ushort y);

	// 判断指定坐标的格子能否可走
	bool isCanMove(ushort x, ushort y, bool isCharMove = false);

	// 根据给定区域，随机取一个空格子
	stTILE* getRandTile(int x1, int y1, int x2, int y2);

	// 取出给定坐标最近的空格子
	stTILE* getSideTile(int x, int y);

	// 计算开始点周围距离目标点最近的一个点
	POINT getShortPos(int startX, int startY, int endX, int endY);

	// 通过地图搜索出一条最短路径
	void findPath(int startX, int startY, int endX, int endY, vector<POINT>& vecPath);

private:
	bool m_isLoaded;			// 是否已经被加载入内存
	uint mMapId;
	string m_mapFile;			// 地图名字
	SIZE m_MapPixelSize;		// 像素尺寸
	SIZE m_MapTileSize;			// 格子尺寸
	RECT m_pkRect;				// PK区域

	stTILE** m_ppMapData;		// 地图数据

	byte** m_blockData;			// 阻挡信息

	PathFinder m_pathFinder;	// 寻路器
};




class MapManager : public Singleton<MapManager>
{
public:
	MapManager();
	virtual ~MapManager();

	Map* loadMap(uint mapId);

	Map* getMap(uint mapId);

protected:
private:
	map<uint,Map*> mMap;
};



#endif

