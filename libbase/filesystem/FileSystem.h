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


// �Զ������ļ����ͷţ�ǿ���Ƽ�ʹ�ô˽ӿڣ�
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


// �����ļ�, ���ļ���ʱֻ�ܶ�ȡ������д��
// Virtual File
class VFile
{
	friend class FileSystem;
public:
	virtual ~VFile();

	// �Ƿ��ļ��Ľ���
	bool isEnd();

	// �ļ���С
	uint size();

	// ��û����ַ
	void* getBufferPtr();

	bool setPosition(uint pos);
	uint getPosition() const;

	// ��ȡ�ļ�����
	uint read(void* buffer, uint len);

private:
	// �ͷ����е���Դ
	VFile();
	void release();

protected:
	byte* m_ptrSrc;
	byte* m_ptrCur;
	uint m_size;
};


// �ļ�ϵͳ
class FileSystem : public Singleton<FileSystem>
{
public:
	struct stPakFile
	{
		string filename;		// pak�ļ���
		char key;				// ���������õ�Key
		vector<byte> password;	// �������ܺ������

		stPakFile()
		{
			filename = "";
			key = 0;
			password.clear();
		}
	};

	enum EFileOpenType
	{
		PackFileFirst,	// ���ȶ�ȡ�����ļ�
		LocalFileFirst,	// ���ȶ�ȡ�����ļ�
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

	// ����һ��pak�ļ�
	bool attachPakFile(const char* filename, const char* password = NULL);

	// ɾ��һ��pak�ļ�
	bool detachPakFile(const char* filename);

	// ɾ�����е�PAK�ļ�
	void detachAllPakFile();

	// ��һ���ļ�
	VFile* openFile(const char* filename);

	// �ر�һ���ļ�
	void closeFile(VFile* pVFile);

protected:
	// ��ѹ�����ж�ȡ�ļ�
	VFile* openFileFromPak(const char* filename);

	// �ӱ���ϵͳ�ж�ȡ�ļ�
	VFile* openFileFromLocal(const char* filename);

private:
	// ���д򿪵�pak�ļ���Ϣ
	string mResourceRoot;
	EFileOpenType m_fileOpenType;
	PakFileMap m_pakFileMap;
	VFileMap m_vFileMap;
};


}	// namespace

#endif