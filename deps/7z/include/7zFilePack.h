#ifndef __FILE_PACK_H
#define __FILE_PACK_H

#include <windows.h>
#include "7zUtil.h"
#include <string>
#include <map>
using namespace std;

typedef map<string, size_t> FileIndexMap;

// seven zip file pack
class SZFilePack
{
public:
	SZFilePack();
	SZFilePack(const char* packFileName, const char* fileName);
	virtual ~SZFilePack(void);

	// 打开一个7z文件
	bool open(const char* packFileName);

	// 从7z文件中打开一个文件
	CBuf* openFile(const char* fileName);

private:
	void init();
	void close();
	void closeFile();

private:
	bool mIsPackOpened;
	CFileInStream mArchiveStream;
	CSzArEx mDb;
	CLookToRead mLookStream;
	ISzAlloc mAllocImp;
	ISzAlloc mAllocTempImp;

	FileIndexMap mIndexMap;	// 文件对应的索引号
	string mPackFileName;	// 当前7z文件名
	CBuf* mFileBuffer;		// 当前文件的缓存
};



#endif
