#include "7zFilePack.h"

__inline void NormalFileName(char* filename)
{
	size_t len = strlen(filename);
	for (size_t i=0; i<len; i++) {
		if (filename[i] == '\\')
			filename[i] = '/';
	}
}


SZFilePack::SZFilePack()
{
	init();
}

SZFilePack::SZFilePack(const char* packFileName, const char* fileName)
{
	init();
	if ( open(packFileName) ) {
		mIsPackOpened = true;
	}
	openFile(fileName);
}

SZFilePack::~SZFilePack(void)
{
	close();
	if (mFileBuffer) {

		delete mFileBuffer;
	}
}

void SZFilePack::init()
{
	mAllocImp.Alloc = SzAlloc;
	mAllocImp.Free = SzFree;
	mAllocTempImp.Alloc = SzAllocTemp;
	mAllocTempImp.Free = SzFreeTemp;
	mIsPackOpened = false;
	mFileBuffer = NULL;
}

bool SZFilePack::open(const char* packFileName)
{
	close();

	UInt32 i;
	SRes res;
	UInt16 *temp = NULL;
	size_t tempSize = 0;
	CBuf buf;
	buf.data = new byte[MAX_PATH];
	buf.size = MAX_PATH;
	mPackFileName = packFileName;
	if ( InFile_Open(&mArchiveStream.file, packFileName) ) {
		PrintError("can not open input file");
		return false;
	}

	FileInStream_CreateVTable(&mArchiveStream);
	LookToRead_CreateVTable(&mLookStream, False);

	mLookStream.realStream = &mArchiveStream.s;
	LookToRead_Init(&mLookStream);

	CrcGenerateTable();

	SzArEx_Init(&mDb);
	res = SzArEx_Open(&mDb, &mLookStream.s, &mAllocImp, &mAllocTempImp);
	if (res == SZ_OK) {
		for (i = 0; i < mDb.db.NumFiles; i++) {
			size_t len;
			const CSzFileItem *f = mDb.db.Files + i;

			if (f->IsDir)
				continue;

			len = SzArEx_GetFileNameUtf16(&mDb, i, NULL);
			if (len > tempSize) {
				SzFree(NULL, temp);
				tempSize = len;
				temp = (UInt16 *)SzAlloc(NULL, tempSize * sizeof(temp[0]));
				if (temp == 0) {
					res = SZ_ERROR_MEM;
					break;
				}
			}
			SzArEx_GetFileNameUtf16(&mDb, i, temp);
			if (res != SZ_OK)
				break;

			// 保存所有文件的索引号到hash表中
			res = Utf16_To_Char(&buf, temp, 0);
			if (res != SZ_OK)
				break;

			//名字转成小写
			_strlwr((char*)buf.data);
			string key = (const char*)buf.data;
			mIndexMap[key] = i;
		}
	}

	SzFree(NULL, temp);
	delete[] buf.data;
	mIsPackOpened = true;

	return true;
}

void SZFilePack::close()
{
	if (mIsPackOpened) {
		SzArEx_Free(&mDb, &mAllocImp);
		File_Close(&mArchiveStream.file);
		mIndexMap.clear();
		mIsPackOpened = false;
	}
}

CBuf* SZFilePack::openFile(const char* fileName)
{
	char szFileName[MAX_PATH+1] = "";
	strncpy(szFileName, fileName, MAX_PATH);
	NormalFileName(szFileName);
	_strlwr(szFileName);

	FileIndexMap::iterator itFind = mIndexMap.find(szFileName);
	if (itFind == mIndexMap.end())
		return NULL;

	//if you need cache, use these 3 variables.
	//if you use external function, you can make these variable as static.
	UInt32 blockIndex = 0xFFFFFFFF; // it can have any value before first call (if outBuffer = 0)
	Byte *outBuffer = 0; // it must be 0 before first call for each new archive.
	size_t outBufferSize = 0;  // it can have any value before first call (if outBuffer = 0)
	size_t offset = 0;
	size_t outSizeProcessed = 0;
	SRes res;

	size_t index = itFind->second;
	res = SzArEx_Extract(&mDb, &mLookStream.s, index,
		&blockIndex, &outBuffer, &outBufferSize,
		&offset, &outSizeProcessed,
		&mAllocImp, &mAllocTempImp);
	if (res != SZ_OK)
		return NULL;

	closeFile();
	mFileBuffer = new CBuf;
	mFileBuffer->data = new byte[outSizeProcessed];
	memcpy(mFileBuffer->data, outBuffer+offset, outSizeProcessed);
	mFileBuffer->size = outSizeProcessed;

	IAlloc_Free(&mAllocImp, outBuffer);

	return mFileBuffer;
}

void SZFilePack::closeFile()
{
	//if (mFileBuffer) {
	//	//delete[] mFileBuffer->data;
	//	delete mFileBuffer;
	//}
}





