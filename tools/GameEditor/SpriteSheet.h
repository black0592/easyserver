#ifndef __SPRITE_SHEET_H__
#define __SPRITE_SHEET_H__

#include "easy2d.h"

namespace easy2d {

	// plist中每一帧信息
	struct FRAME_INFO
	{
		// 单元图片名称
		string name;

		// 图片信息
		int x;
		int y;
		int width;
		int height;

		// 偏移信息
		int offsetX;
		int offsetY;

		// 是否旋转
		bool rotated;

		// 在图上的坐标
		int srcX;
		int srcY;
		int srcW;
		int srcH;

		// 原始宽高
		int orgWidth;
		int orgHeight;

		FRAME_INFO()
		{
			x = 0;
			y = 0;
			width = 0;
			height = 0;

			offsetX = 0;
			offsetY = 0;

			rotated = false;

			srcX = 0;
			srcY = 0;
			srcW = 0;
			srcH = 0;

			orgWidth = 0;
			orgHeight = 0;
		}
	};

	// plist文件描述信息
	struct METADATA
	{
		int format;
		string texFile;
		int texWidth;
		int texHeight;
	};

	typedef vector<FRAME_INFO*>		FrameArray;
	typedef map<string,FRAME_INFO*>	FrameMap;
	typedef FrameMap::iterator		FrameIter;



	// 动画文件
	class SpriteSheet
	{
	public:
		SpriteSheet();
		virtual ~SpriteSheet();

		void release();
		bool load(const char* filename);
		void render(int x, int y);

		FRAME_INFO* getFrame(int index);
		FRAME_INFO* getFrame(const char* name);

		const METADATA& getMetadata() { return mMetadata; }

	private:
		void initSprite();
		void parsePoint(const string& str, int& x, int& y);
		void parseRect(const string& str, int& x, int& y, int& width, int& height);

	protected:
		string mFilePath;
		string mFilePathName;

		METADATA mMetadata;
		FrameArray mFrameArray;
		FrameMap mFrameMap;

		ISprite* mSprite;
		ITexture* mTexture;

		uint mLastTick;
		int mCurFrame;
	};

} // namespace

#endif
