#ifndef __TILE_MAP_H__
#define __TILE_MAP_H__

#include "easy2d.h"
using namespace pugi;

namespace easy2d {

	// 图块的格子属性
	struct TILE_SET_PROP
	{
		int tileId;
		Properties prop;
	};
	typedef vector<TILE_SET_PROP> TileSetPropArray;

	// 图素信息
	struct TILE_SET_INFO
	{
		string name;		// 名称
		uint firstGid;		// 初始编号(唯一编号)
		uint tileWidth;		// 单元格宽度
		uint tileHeight;	// 单元格高度
		int spacing;		// 间距
		int margin;			// 边距
		string imgSource;	// 图片路径
		uint imgWidth;		// 图片宽度
		uint imgHeight;		// 图片高度
		Properties prop;	// 图块属性
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

	// 单元格信息
	struct MAP_TILE
	{
		uint gid;			// 不小于1
		int tilesetIndex;

		// 为了优化效率
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

	// 图层信息
	struct TILE_LAYER_INFO 
	{
		string name;			// 名字
		bool visible;			// 是否显示
		float opacity;			// 透明度
		int layerWidh;			// 图层宽度
		int layerHeight;		// 图层高度
		MapTileArray mMapTiles;	// 格子信息
		Properties prop;		// 图层的属性

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

	// 地图信息
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



	// tmx地图配置文件
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
		// strSource 不为空，表示外部图块
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
