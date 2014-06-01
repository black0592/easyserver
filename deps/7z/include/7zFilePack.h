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

	// ��һ��7z�ļ�
	bool open(const char* packFileName);

	// ��7z�ļ��д�һ���ļ�
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

	FileIndexMap mIndexMap;	// �ļ���Ӧ��������
	string mPackFileName;	// ��ǰ7z�ļ���
	CBuf* mFileBuffer;		// ��ǰ�ļ��Ļ���
};



#endif
