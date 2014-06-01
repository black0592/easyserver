#ifndef __TILE_MAP_H__
#define __TILE_MAP_H__

#include "easy2d.h"
using namespace pugi;

namespace easy2d {

	// ͼ��ĸ�������
	struct TILE_SET_PROP
	{
		int tileId;
		Properties prop;
	};
	typedef vector<TILE_SET_PROP> TileSetPropArray;

	// ͼ����Ϣ
	struct TILE_SET_INFO
	{
		string name;		// ����
		uint firstGid;		// ��ʼ���(Ψһ���)
		uint tileWidth;		// ��Ԫ����
		uint tileHeight;	// ��Ԫ��߶�
		int spacing;		// ���
		int margin;			// �߾�
		string imgSource;	// ͼƬ·��
		uint imgWidth;		// ͼƬ���
		uint imgHeight;		// ͼƬ�߶�
		Properties prop;	// ͼ������
		TileSetPropArray gidPropList;

		ITexture* pTex;

		TILE_SET_INFO()
		{
			firstGid = 0;
			tileWidth = 0;
			tileHeight = 0;
			spacing = 0;
			margin = 0;
			imgWidth = 0;
			imgHeight = 0;

			pTex = NULL;
		}
	};

	typedef vector<TILE_SET_INFO> TileSetArray;
	typedef map<uint,TILE_SET_INFO> TileSetMap;
	typedef map<uint,TILE_SET_INFO>::iterator TileSetIter;

	//////////////////////////////////////////////////////////////////////////

	// ��Ԫ����Ϣ
	struct MAP_TILE
	{
		uint gid;			// ��С��1
		int tilesetIndex;

		// Ϊ���Ż�Ч��
		int renderX;
		int renderY;
		stFrameInfo frm;

		MAP_TILE()
		{
			gid = 0;
			tilesetIndex = -1;
		}

		MAP_TILE(uint _gid, int _tilesetIndex)
		{
			gid = _gid;
			tilesetIndex = _tilesetIndex;
		}
	};

	typedef vector<MAP_TILE> MapTileArray;


	//////////////////////////////////////////////////////////////////////////

	// ͼ����Ϣ
	struct TILE_LAYER_INFO 
	{
		string name;			// ����
		bool visible;			// �Ƿ���ʾ
		float opacity;			// ͸����
		int layerWidh;			// ͼ����
		int layerHeight;		// ͼ��߶�
		MapTileArray mMapTiles;	// ������Ϣ
		Properties prop;		// ͼ�������

		TILE_LAYER_INFO()
		{
			visible = true;
			opacity = 1.0f;
			layerWidh = 0;
			layerHeight = 0;
		}
	};

	typedef vector<TILE_LAYER_INFO> TileLayerArray;
	typedef map<string,TILE_LAYER_INFO> TileLayerMap;
	typedef map<string,TILE_LAYER_INFO>::iterator TileLayerIter;

	//////////////////////////////////////////////////////////////////////////

	// ��ͼ��Ϣ
	struct TILE_MAP_INFO
	{
		string version;
		string orientation;
		int width;
		int height;
		int tileWidth;
		int tileHeight;

		TILE_MAP_INFO()
		{
			reset();	
		}

		void reset()
		{
			width = 0;
			height = 0;
			tileWidth = 0;
			tileHeight = 0;
		}
	};


	//////////////////////////////////////////////////////////////////////////



	// tmx��ͼ�����ļ�
	class TileMap
	{
	public:
		TileMap();
		virtual ~TileMap();

		void release();
		bool load(const char* filename);
		void render();
		void setPostion(int x, int y);
		bool isLoaded();

		int getMapWidth();
		int getMapHeight();
		int getTileWidth();
		int getTileHeight();
		bool isLayerVisible(const char* layerName);
		void setLayerVisible(const char* layerName, bool visible);
		TILE_LAYER_INFO* findLayer(const char* layerName);

	private:
		void renderLayer(const TILE_LAYER_INFO& layerInfo);
		int findTilesetIndex(uint gid);
		TILE_SET_INFO* getTileSet(uint index);
		// strSource ��Ϊ�գ���ʾ�ⲿͼ��
		bool parseTileSet(TILE_SET_INFO& tileSetInfo, const string& strSource, const xml_node& tilesetRoot);
		bool parseProperties(Properties& prop, const xml_node& propRoot);


	protected:
		bool mIsLoaded;
		string mFilePath;
		string mFilePathName;
		TILE_MAP_INFO mTileMapInfo;
		TileSetArray mTileSetArray;
		TileLayerArray mTileLayerArray;

		int mMapX;
		int mMapY;
	};

} // namespace

#endif
