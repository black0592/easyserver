#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include "BaseTypes.h"
#include "misc/Singleton.h"
#include <string>
#include <vector>
#include <map>
using namespace std;


namespace easygame {

class VFile;


// 自动处理文件的释放（强烈推荐使用此接口）
class File
{
public:
	File();
	virtual ~File();

	bool open(const char* filename);
	void close();
	bool isOpened();
	bool isEnd();
	uint read(void* buffer, uint len);
	void write(void* buffer, uint len);
	uint getLength() const;
	bool setPosition(uint pos);
	uint getPosition() const;
	void* getBufferPtr();

protected:
	VFile* mVFile;
};


// 虚拟文件, 此文件暂时只能读取，不能写入
// Virtual File
class VFile
{
	friend class FileSystem;
public:
	virtual ~VFile();

	// 是否文件的结束
	bool isEnd();

	// 文件大小
	uint size();

	// 获得缓存地址
	void* getBufferPtr();

	bool setPosition(uint pos);
	uint getPosition() const;

	// 读取文件内容
	uint read(void* buffer, uint len);

private:
	// 释放所有的资源
	VFile();
	void release();

protected:
	byte* m_ptrSrc;
	byte* m_ptrCur;
	uint m_size;
};


// 文件系统
class FileSystem : public Singleton<FileSystem>
{
public:
	struct stPakFile
	{
		string filename;		// pak文件名
		char key;				// 加密密码用的Key
		vector<byte> password;	// 经过加密后的密码

		stPakFile()
		{
			filename = "";
			key = 0;
			password.clear();
		}
	};

	enum EFileOpenType
	{
		PackFileFirst,	// 优先读取包中文件
		LocalFileFirst,	// 优先读取本地文件
	};

	typedef map<string,stPakFile> PakFileMap;
	typedef map<string,stPakFile>::iterator PakFileMapIter;

	typedef map<VFile*,VFile*> VFileMap;
	typedef map<VFile*,VFile*>::iterator VFileMapIter;

public:
	FileSystem();
	virtual ~FileSystem(void);

	void setFileOpenType(EFileOpenType openType);

	void setResourceRoot(string strPah);

	// 载入一个pak文件
	bool attachPakFile(const char* filename, const char* password = NULL);

	// 删除一个pak文件
	bool detachPakFile(const char* filename);

	// 删除所有的PAK文件
	void detachAllPakFile();

	// 打开一个文件
	VFile* openFile(const char* filename);

	// 关闭一个文件
	void closeFile(VFile* pVFile);

protected:
	// 从压缩包中读取文件
	VFile* openFileFromPak(const char* filename);

	// 从本地系统中读取文件
	VFile* openFileFromLocal(const char* filename);

private:
	// 所有打开的pak文件信息
	string mResourceRoot;
	EFileOpenType m_fileOpenType;
	PakFileMap m_pakFileMap;
	VFileMap m_vFileMap;
};


}	// namespace

#endif