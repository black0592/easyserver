#include "SpriteSheet.h"

#define THREAD_LOAD	1

namespace easy2d {

	SpriteSheet::SpriteSheet()
	{
		mLastTick = 0;
		mCurFrame = 0;
		mSprite = NULL;
		mTexture = NULL;
	}

	SpriteSheet::~SpriteSheet()
	{
		release();
	}

	void SpriteSheet::release()
	{
		for (size_t i=0; i<mFrameArray.size(); i++) {
			FRAME_INFO* pInfo = mFrameArray[i];
			delete pInfo;
		}

		mFrameArray.clear();
		mFrameMap.clear();

		IEngine* pEngine = GetEngine();
		if (mSprite && pEngine) {
			IVideo* pVideo = pEngine->getVideo();
			if (pVideo)
				pVideo->destroySprite(mSprite);
		}
		
		mCurFrame = 0;
		mLastTick = 0;
		mSprite = NULL;
		mTexture = NULL;
	}

	void SpriteSheet::parsePoint(const string& str, int& x, int& y)
	{
		string strNum = str.substr(1, str.length()-2);

		vector<string> list;
		stringtok(list, strNum, ",");

		x = atoi(list[0].c_str());
		y = atoi(list[1].c_str());
	}

	void SpriteSheet::parseRect(const string& str, int& x, int& y, int& width, int& height)
	{
		string strNum = str;
		for (size_t i=0; i<strNum.length(); i++) {
			if (strNum[i] == '{' || strNum[i] == '}')
				strNum[i] = ' ';
		}
		
		vector<string> list;
		stringtok(list, strNum, ",");

		x = atoi(list[0].c_str());
		y = atoi(list[1].c_str());
		width = atoi(list[2].c_str());
		height = atoi(list[3].c_str());
	}

	bool SpriteSheet::load(const char* filename)
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

		xml_node rootNode = doc.child("plist").child("dict");
		for (xml_node keyNode=rootNode.child("key"); keyNode; keyNode=keyNode.next_sibling("key")) { 
			string strKey = keyNode.text().as_string();
			
			if (strKey == "frames") {
				xml_node rootNode2=keyNode.next_sibling("dict");

				for (xml_node keyNode2=rootNode2.child("key"); keyNode2; keyNode2=keyNode2.next_sibling("key")) {
					string strKey2 = keyNode2.text().as_string();

					FRAME_INFO* pFrmInfo = new FRAME_INFO;
					pFrmInfo->name = strKey2;

					xml_node rootNode3=keyNode2.next_sibling("dict");
					for (xml_node keyNode3=rootNode3.child("key"); keyNode3; keyNode3=keyNode3.next_sibling("key")) {
						string strKey3 = keyNode3.text().as_string();
						if (strKey3 == "frame") {
							xml_node valueNode = keyNode3.next_sibling();
							string strText = valueNode.text().as_string();
							parseRect(strText, pFrmInfo->x, pFrmInfo->y, pFrmInfo->width, pFrmInfo->height);
						}
						if (strKey3 == "offset") {
							xml_node valueNode = keyNode3.next_sibling();
							string strText = valueNode.text().as_string();
							parsePoint(strText, pFrmInfo->offsetX, pFrmInfo->offsetY);
						}
						if (strKey3 == "rotated") {
							xml_node valueNode = keyNode3.next_sibling();
							bool ret = (string(valueNode.name()) == "true");
							pFrmInfo->rotated = ret;
						}
						if (strKey3 == "sourceColorRect") {
							xml_node valueNode = keyNode3.next_sibling();
							string strText = valueNode.text().as_string();
							parseRect(strText, pFrmInfo->srcX, pFrmInfo->srcY, pFrmInfo->srcW, pFrmInfo->srcH);
						}
						if (strKey3 == "sourceSize") {
							xml_node valueNode = keyNode3.next_sibling();
							string strText = valueNode.text().as_string();
							parsePoint(strText, pFrmInfo->orgWidth, pFrmInfo->orgHeight);
						}
					}

					mFrameArray.push_back(pFrmInfo);
					mFrameMap[pFrmInfo->name] = pFrmInfo;
				}
			}

			if (strKey == "metadata")
			{
				xml_node rootNode2=keyNode.next_sibling("dict");
				for (xml_node keyNode2=rootNode2.child("key"); keyNode2; keyNode2=keyNode2.next_sibling("key")) {
					string strKey2 = keyNode2.text().as_string();
					if (strKey2 == "format") {
						xml_node valueNode = keyNode2.next_sibling();
						mMetadata.format = valueNode.text().as_int();
					}
					if (strKey2 == "realTextureFileName") {
						xml_node valueNode = keyNode2.next_sibling();
						mMetadata.texFile = valueNode.text().as_string();
					}
					if (strKey2 == "size") {
						xml_node valueNode = keyNode2.next_sibling();
						string strText = valueNode.text().as_string();
						parsePoint(strText, mMetadata.texWidth, mMetadata.texHeight);
					}
				}
			}
		}

		initSprite();

		return true;
	}

	void SpriteSheet::initSprite()
	{
		IEngine* pEngine = GetEngine();
		IVideo* pVideo = pEngine->getVideo();

		string strTexFile = mFilePath;
		strTexFile += mMetadata.texFile;
		mTexture = TextureMgr::instance().loadTexture(strTexFile.c_str(), true);

		mSprite = pVideo->createSprite();
		vector<stFrameInfo> frmList;
		for (size_t i=0; i<mFrameArray.size(); i++) {
			FRAME_INFO* pFrmInfo = mFrameArray[i];
			stFrameInfo frm;
			frm.name = pFrmInfo->name;
			frm.x = pFrmInfo->x;
			frm.y = pFrmInfo->y;
			if (pFrmInfo->rotated) {
				// 旋转需要特殊处理
				frm.width = pFrmInfo->height;
				frm.height = pFrmInfo->width;
			} else {
				frm.width = pFrmInfo->width;
				frm.height = pFrmInfo->height;
			}
			frmList.push_back(frm);
		}
		mSprite->initialize(mMetadata.texWidth, mMetadata.texHeight, &frmList);
		mSprite->texture(mTexture);
	}


	FRAME_INFO* SpriteSheet::getFrame(int index)
	{
		if (index >= (int)mFrameArray.size())
			return NULL;

		return mFrameArray[index];
	}

	FRAME_INFO* SpriteSheet::getFrame(const char* name)
	{
		FrameIter it = mFrameMap.find(name);
		if (it == mFrameMap.end())
			return NULL;

		return it->second;
	}

	void SpriteSheet::render(int x, int y)
	{
		if (mFrameArray.empty())
			return;

		int curTick = Platform::clock();

		IEngine* pEngine = GetEngine();
		IVideo* pVideo = pEngine->getVideo();

		FRAME_INFO* pFrmInfo = mFrameArray[mCurFrame];

		RenderParam param;
		param.px = x + pFrmInfo->offsetX;
		param.py = y - pFrmInfo->offsetY;
		if (pFrmInfo->rotated) {
			param.rotation = -90.0f * 0.017452f;
		}

		pVideo->drawSprite(mSprite, mCurFrame, &param);

		if (curTick - mLastTick > 150) {
			mLastTick = curTick;

			mCurFrame++;
			if (mCurFrame >= mFrameArray.size())
				mCurFrame = 0;
		}
	}


} // namespace

