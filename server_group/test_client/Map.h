#ifndef __CLIENT_MAP_H__
#define __CLIENT_MAP_H__
#include "BaseLib.h"
#include "ProtoGameData.pb.h"

#define SCREEN_WIDTH	960
#define SCREEN_HEIGHT	640


// ����960*640Ϊ����
// һ��X���������
#define NXTILE 15
// һ��Y���������
#define NYTILE 20

// һ���X��Y����ֵ
#define CXTILE 64
#define CYTILE 32


//////////////////////////////////////////////////////////////////////////
// duplicate ��Ϸ����
// mapid - 2�ֽ�

#define INVALID_DUPID	0		// �Ǹ���id

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

// ��ͼ����ת��
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

// ��Ԫ��ṹ
struct stTILE
{
	POINT pos;		// ���ӵ�����
	MapCellFlag Attrib;	// ��������
	ushort mapId;	// ���͵ĵ�ͼ���
	POINT ptLoading;// Ŀ���ͼ������

	uint itemId;	// ��ƷID��
	bool bIsObject;	// �Ƿ�������
	ushort CharId;	// ��ɫID
	ushort MopId;		// δ֪�������ǹ���ID
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
	ushort x;			// ��Ʒ�ڵ�ͼ�ϵ�����λ��
	ushort y;
	Item* pItem;
	uint dropTick;	// ����ʱ��ʱ��
	uint LifeTime;	// ��Ʒ����ʧʱ��(��)

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

	// ��ȡ��ͼ
	bool loadMapData(uint mapId);

	// �жϵ�ͼ�Ƿ��Ѿ�������
	bool isLoaded();

	// ж�ص�ͼ����
	void unloadMapData();

	// ȡ��������Ϊ��λ�ĵ�ͼ�ߴ�
	SIZE getMapSizePerPixel();

	// ȡ���Ը���Ϊ��λ�ĵ�ͼ�ߴ�
	SIZE getMapSizePerTile();

	// ȡ�õ�ͼ����Ʒ������
	uint getMapItemCount();

	// ȡ��ָ������ĸ�������
	stTILE* getMapData(ushort x, ushort y);

	// �ж�ָ������ĸ����ܷ����
	bool isCanMove(ushort x, ushort y, bool isCharMove = false);

	// ���ݸ����������ȡһ���ո���
	stTILE* getRandTile(int x1, int y1, int x2, int y2);

	// ȡ��������������Ŀո���
	stTILE* getSideTile(int x, int y);

	// ���㿪ʼ����Χ����Ŀ��������һ����
	POINT getShortPos(int startX, int startY, int endX, int endY);

	// ͨ����ͼ������һ�����·��
	void findPath(int startX, int startY, int endX, int endY, vector<POINT>& vecPath);

private:
	bool m_isLoaded;			// �Ƿ��Ѿ����������ڴ�
	uint mMapId;
	string m_mapFile;			// ��ͼ����
	SIZE m_MapPixelSize;		// ���سߴ�
	SIZE m_MapTileSize;			// ���ӳߴ�
	RECT m_pkRect;				// PK����

	stTILE** m_ppMapData;		// ��ͼ����

	byte** m_blockData;			// �赲��Ϣ

	PathFinder m_pathFinder;	// Ѱ·��
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

