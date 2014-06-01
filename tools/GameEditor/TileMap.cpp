#include "TileMap.h"
#include "easy2d.h"
#include "zlib.h"

#define THREAD_LOAD	1

enum ccTMXTileFlags {
	kCCTMXTileHorizontalFlag	= 0x80000000,
	kCCTMXTileVerticalFlag		= 0x40000000,
	kCCTMXTileDiagonalFlag		= 0x20000000,
	kCCFlipedAll				= (kCCTMXTileHorizontalFlag|kCCTMXTileVerticalFlag|kCCTMXTileDiagonalFlag),
	kCCFlippedMask				= ~(kCCFlipedAll),
};

namespace easy2d {

	TileMap::TileMap()
	{
		mIsLoaded = false;
		mMapX = 0;
		mMapY = 0;
	}

	TileMap::~TileMap()
	{
		
	}

	void TileMap::release()
	{
		mIsLoaded = false;
		mTileMapInfo.reset();
		mTileSetArray.clear();
		mTileLayerArray.clear();

		//IVideo* video = GetEngine()->getVideo();
		//for (size_t i=0; i<mTileSetArray.size(); i++) {
		//	TILE_SET_INFO& tilesetInfo = mTileSetArray[i];
		//	video->destroyTexture(tilesetInfo.pTex);
		//}
	}

	bool TileMap::isLoaded()
	{
		return mIsLoaded;
	}

	bool TileMap::load(const char* filename)
	{
		using namespace pugi;

		release();

		File file;
		if ( !file.open(filename) ) {
			return false;
		}

		xml_document doc;
		if (!doc.load_buffer(file.getBufferPtr(), file.getLength()))
			return false;

		mFilePathName = filename;
		mFilePath = mFilePathName.substr(0, mFilePathName.find_last_of("/")+1);

		xml_node mapNode = doc.child("map");
		mTileMapInfo.version = mapNode.attribute("version").as_string();
		mTileMapInfo.orientation = mapNode.attribute("orientation").as_string();
		mTileMapInfo.width = mapNode.attribute("width").as_int();
		mTileMapInfo.height = mapNode.attribute("height").as_int();
		mTileMapInfo.tileWidth = mapNode.attribute("tilewidth").as_int();
		mTileMapInfo.tileHeight = mapNode.attribute("tileheight").as_int();

		// 解析tileset信息
		for (xml_node tilesetNode=mapNode.child("tileset"); tilesetNode; tilesetNode=tilesetNode.next_sibling("tileset")) {
			TILE_SET_INFO tileSetInfo;
			tileSetInfo.firstGid = tilesetNode.attribute("firstgid").as_int();
			string strSource = tilesetNode.attribute("source").as_string();
			parseTileSet(tileSetInfo, strSource, tilesetNode);
			mTileSetArray.push_back(tileSetInfo);
		}

		// 解析layer信息
		for (xml_node layerNode=mapNode.child("layer"); layerNode; layerNode=layerNode.next_sibling("layer")) { 
			TILE_LAYER_INFO layerInfo;

			// 解析属性
			parseProperties(layerInfo.prop, layerNode.child("properties"));

			layerInfo.name = layerNode.attribute("name").as_string();
			layerInfo.visible = layerNode.attribute("visible").empty() ? true : false;
			layerInfo.opacity = layerNode.attribute("opacity").empty() ? 1.0f : layerNode.attribute("opacity").as_float();
			layerInfo.layerWidh = layerNode.attribute("width").as_int();
			layerInfo.layerHeight = layerNode.attribute("height").as_int();
			layerInfo.mMapTiles.resize(layerInfo.layerWidh * layerInfo.layerHeight);

			// data信息
			xml_node dataNode = layerNode.child("data");
			string encoding = dataNode.attribute("encoding").as_string();
			string compression = dataNode.attribute("compression").as_string();
			string zipData = dataNode.text().as_string();
			string text = "";
			uint *out = NULL;
			if (encoding == "base64") {
				//trim data
				zipData.erase(0,zipData.find_first_not_of("\r\t\n "));
				zipData.erase(zipData.find_last_not_of("\r\t\n ")+1);

				text = base64_decode(zipData);
				if (compression == "zlib") {
					uLongf outlen = layerInfo.layerWidh * layerInfo.layerHeight * 4;
					out = new uint[outlen];
					uncompress((byte*)out, &outlen, (const byte*)text.c_str(), text.size());
				}
			} else {
				LOGE("tmx: invalid encoding");
				return false;
			}

			// Convert the gids to map tiles.
			for (int y = 0; y < layerInfo.layerHeight; y++) {
				for (int x = 0; x < layerInfo.layerWidh; x++) {
					uint gidMask = out[y * layerInfo.layerWidh + x];
					if (gidMask == 0)
						continue;

					uint gid = (gidMask & kCCFlippedMask);

					// Find the tileset index.
					const int tilesetIndex = findTilesetIndex(gid);
					TILE_SET_INFO* tileset = getTileSet(tilesetIndex);
					if (tileset == NULL)
						continue;

					MAP_TILE mapTile(gidMask, tilesetIndex);
					stFrameInfo& frm = mapTile.frm;

					//--------------------------------------------
					// 优化性能
					mapTile.renderX = x * mTileMapInfo.tileWidth;
					mapTile.renderY = y * mTileMapInfo.tileHeight - tileset->tileHeight + mTileMapInfo.tileHeight;

#if 1
					if (tileset->tileWidth > 0 && tileset->tileHeight > 0) {
						int tilesetWidth = tileset->imgWidth / tileset->tileWidth;
						int index = gid - tileset->firstGid;
						int indX = index % tilesetWidth;
						int indY = index / tilesetWidth;

						frm.x = indX * tileset->tileWidth;
						frm.y = indY * tileset->tileHeight;
						frm.width = tileset->tileWidth;
						frm.height = tileset->tileHeight;
						frm.u0 = (float)frm.x  / (float)tileset->imgWidth;
						frm.v0 = (float)frm.y  / (float)tileset->imgHeight;
						frm.u1 = (float)(frm.x+frm.width) / (float)tileset->imgWidth;
						frm.v1 = (float)(frm.y+frm.height) / (float)tileset->imgHeight;
					}
#endif
					//---------------------------------------------------

					// 保存
					layerInfo.mMapTiles[y * layerInfo.layerWidh + x] = mapTile;
				}
			}

			// Free the temporary array from memory.
			delete[] out;

			mTileLayerArray.push_back(layerInfo);
		}

		mIsLoaded = true;

		return true;
	}

	bool TileMap::parseTileSet(TILE_SET_INFO& tileSetInfo, const string& strSource, const xml_node& tilesetRoot)
	{
		File file;
		xml_document doc;
		xml_node tilesetNode = tilesetRoot;
		if ( !strSource.empty() ) {
			// 外部图块解析
			string strFileName = mFilePath;
			strFileName += strSource;

			if ( !file.open(strFileName.c_str()) ) {
				return false;
			}

			if (!doc.load_buffer(file.getBufferPtr(), file.getLength()))
				return false;

			tilesetNode = doc.child("tileset");
			if (!tilesetNode)
				return false;
		}

		// -----------------------
		tileSetInfo.name = tilesetNode.attribute("name").as_string();
		tileSetInfo.tileWidth = tilesetNode.attribute("tilewidth").as_int();
		tileSetInfo.tileHeight = tilesetNode.attribute("tileheight").as_int();
		tileSetInfo.spacing = tilesetNode.attribute("spacing").as_int();
		tileSetInfo.margin = tilesetNode.attribute("margin").as_int();

		// 解析tileset的属性信息
		parseProperties(tileSetInfo.prop, tilesetNode.child("properties"));

		// tileset的图片信息
		xml_node imgNode = tilesetNode.child("image");
		tileSetInfo.imgSource = imgNode.attribute("source").as_string();
		tileSetInfo.imgWidth = imgNode.attribute("width").as_int();
		tileSetInfo.imgHeight = imgNode.attribute("height").as_int();

		// 解析tileset的图块格子属性信息
		for (xml_node tileNode=tilesetNode.child("tile"); tileNode; tileNode=tileNode.next_sibling("tile")) {
			TILE_SET_PROP temp;
			tileSetInfo.gidPropList.push_back(temp);
			TILE_SET_PROP& tileSetProp = tileSetInfo.gidPropList[tileSetInfo.gidPropList.size()-1];
			tileSetProp.tileId = tileNode.attribute("id").as_int();
			parseProperties(tileSetProp.prop, tileNode.child("properties"));
		}

		return true;
	}

	bool TileMap::parseProperties(Properties& prop, const xml_node& propRoot)
	{
		if (!propRoot)
			return false;

		// 解析tileset的属性信息
		for (xml_node propNode=propRoot; propNode; propNode=propNode.next_sibling("properties")) {
			string strName = propNode.child("property").attribute("name").as_string();
			string strValue = propNode.child("property").attribute("value").as_string();
			prop.addValue(strName.c_str(), strValue.c_str());
		}

		return true;
	}

	int TileMap::findTilesetIndex(uint gid)
	{
		for (size_t i=mTileSetArray.size()-1; i>=0; i--) {
			TILE_SET_INFO& info = mTileSetArray[i];
			if (gid >= info.firstGid)
				return i;
		}

		return -1;
	}

	TILE_SET_INFO* TileMap::getTileSet(uint index)
	{
		if (index >= (uint)mTileSetArray.size())
			return NULL;

		return &mTileSetArray[index];
	}

	int TileMap::getMapWidth()
	{
		return mTileMapInfo.width;
	}

	int TileMap::getMapHeight()
	{
		return mTileMapInfo.height;
	}

	int TileMap::getTileWidth()
	{
		return mTileMapInfo.tileWidth;
	}

	int TileMap::getTileHeight()
	{
		return mTileMapInfo.tileHeight;
	}

	bool TileMap::isLayerVisible(const char* layerName)
	{
		TILE_LAYER_INFO* pLayerInfo = findLayer(layerName);
		if (pLayerInfo == NULL)
			return false;

		return pLayerInfo->visible;
	}

	void TileMap::setLayerVisible(const char* layerName, bool visible)
	{
		TILE_LAYER_INFO* pLayerInfo = findLayer(layerName);
		if (pLayerInfo == NULL)
			return;

		pLayerInfo->visible = visible;
	}

	TILE_LAYER_INFO* TileMap::findLayer(const char* layerName)
	{
		for (size_t i=0; i<mTileLayerArray.size(); i++) {
			TILE_LAYER_INFO* layerInfo = &mTileLayerArray[i];
			if (layerInfo->name == layerName)
				return layerInfo;
		}

		return NULL;
	}

	void TileMap::render()
	{
		for (size_t i=0; i<mTileLayerArray.size(); i++) {
			TILE_LAYER_INFO& layerInfo = mTileLayerArray[i];
			if (!layerInfo.visible)
				continue;
			renderLayer(layerInfo);
		}
	}

	void TileMap::renderLayer(const TILE_LAYER_INFO& layerInfo)
	{
		// 计算出可视范围
		int mapGridX = mMapX>>6;
		int mapGridY = mMapY>>5;
		int NXDRAWTILE = GetEngine()->getScreenWidth()/mTileMapInfo.tileWidth + 2;
		int NYDRAWTILE = GetEngine()->getScreenHeight()/mTileMapInfo.tileHeight + 2;
		int nx1=mapGridX - NXDRAWTILE;
		int ny1=mapGridY - NYDRAWTILE;
		int nx2=mapGridX + NXDRAWTILE * 2;
		int ny2=mapGridY + NYDRAWTILE * 2;

		int offsetX = mTileMapInfo.tileWidth / 2;
		int offsetY = mTileMapInfo.tileHeight / 2;
		offsetX = offsetY = 0;

		// 图层透明度
		byte alpha = (byte)(layerInfo.opacity * 255.0f);

		IVideo* video = GetEngine()->getVideo();

		const MapTileArray& tileArray = layerInfo.mMapTiles;

		RenderParam renderParam;
		renderParam.setColor(255, 255, 255, alpha);

		easy2d::Image image;
		ResourceLoader& readLoader = ResourceLoader::instance();

		for (int y = ny1; y < ny2; y++) {
			for (int x = nx1; x < nx2; x++) {

				if (x < 0 || y < 0)
					continue;

				if (x >= mTileMapInfo.width || y >= mTileMapInfo.height)
					continue;

				const MAP_TILE& tile = tileArray[y * layerInfo.layerWidh + x];
				uint gid = tile.gid;
				if (gid == 0)	// 空的格子，不需要绘制
					continue;

				TILE_SET_INFO* tileset = getTileSet(tile.tilesetIndex);
				if (tileset == NULL)
					continue;

				if (tileset->pTex == NULL) {
					// 创建tileset的贴图
					string strTexFile = mFilePath;
					strTexFile += tileset->imgSource;
					tileset->pTex = TextureMgr::instance().loadTexture(strTexFile.c_str());
				}

				// 绘制当前格子
				renderParam.scalx = 1;
				renderParam.scaly = 1;
				renderParam.px = (float)(tile.renderX - mMapX + offsetX);
				renderParam.py = (float)(tile.renderY - mMapY + offsetY);

				if (gid & kCCTMXTileDiagonalFlag) {
					// TODO
					LOGE("未实现的TileMap功能");
					continue;
				} else {
					if (gid & kCCTMXTileHorizontalFlag) {
						// 水平翻转
						renderParam.scalx = -1;
						renderParam.px += tileset->tileWidth;
					} 

					if (gid & kCCTMXTileVerticalFlag) {
						// 垂直翻转
						renderParam.scaly = -1;
					}
				}

				video->drawTexture(tileset->pTex, &tile.frm, &renderParam, &ZeroPoint);
			}
		}
	}

	void TileMap::setPostion(int x, int y)
	{
		mMapX = x;
		mMapY = y;
	}

	
} // namespace

