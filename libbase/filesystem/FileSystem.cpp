#include "FileSystem.h"
#include "zlib/unzip.h"
#include "platform/Platform.h"
#include "BaseDefine.h"
#include <stdlib.h>

#pragma warning(disable:4311)

namespace easygame {

__inline void NormalFileName(char* filename)
{
	size_t len = strlen(filename);
	for (size_t i=0; i<len; i++) {
		if (filename[i] == '\\')
			filename[i] = '/';
	}
}

//////////////////////////////////////////////////////////////////////////
File::File()
:mVFile(NULL)
{
}

File::~File()
{
	close();	
}

bool File::open(const char* filename)
{
	if (mVFile != NULL)
		close();

	mVFile = FileSystem::getInstance().openFile(filename);
	return mVFile != NULL;
}

bool File::isOpened()
{
	return mVFile != NULL;
}

void File::close()
{
	FileSystem::getInstance().closeFile(mVFile);
	mVFile = NULL;
}

uint File::getLength() const
{
	if (mVFile == NULL)
		return 0;

	return mVFile->size();
}

bool File::setPosition(uint pos)
{
	if (mVFile == NULL)
		return false;

	return mVFile->setPosition(pos);
}

uint File::getPosition() const
{
	if (mVFile == NULL)
		return 0;

	return mVFile->getPosition();
}

bool File::isEnd()
{
	if (mVFile == NULL)
		return true;

	return mVFile->isEnd();
}

uint File::read(void* buffer, uint len)
{
	if (mVFile == NULL)
		return 0;

	return mVFile->read(buffer, len);
}

void File::write(void* buffer, uint len)
{
	if (mVFile == NULL)
		return;

	// TODO		
}

void* File::getBufferPtr()
{
	if (mVFile == NULL)
		return 0;

	return mVFile->getBufferPtr();
}

//////////////////////////////////////////////////////////////////////////

VFile::VFile()
{
	m_size = 0;
	m_ptrSrc = NULL;
	m_ptrCur = NULL;;	
}

VFile::~VFile()
{
	release();
}

void VFile::release()
{
	m_ptrCur = NULL;
	if (m_ptrSrc != NULL)
		delete[] m_ptrSrc;
	m_ptrSrc = NULL;
	m_size = 0;
}

uint VFile::size()
{
	return m_size;
}

bool VFile::isEnd()
{
	return m_ptrCur == m_ptrSrc + m_size;
}

void* VFile::getBufferPtr()
{
	return m_ptrSrc;
}

bool VFile::setPosition(uint pos)
{
	if (m_ptrCur + pos > m_ptrSrc + m_size)
		return false;

	m_ptrCur = m_ptrCur + pos;
	return true;
}

uint VFile::getPosition() const
{
	uint pos = (unsigned long)m_ptrCur - (unsigned long)m_ptrSrc;
	return pos;
}

uint VFile::read(void* buffer, uint len)
{
	if (isEnd())
		return 0;

	uint realSize = len;
	if (m_ptrCur + len > m_ptrSrc + m_size)
		realSize = (unsigned long)m_ptrSrc + (unsigned long)m_size - (unsigned long)m_ptrCur;

	memcpy(buffer, m_ptrCur, realSize);
	m_ptrCur += realSize;
	return realSize;
}




//////////////////////////////////////////////////////////////////////////


FileSystem::FileSystem()
{
	m_fileOpenType = LocalFileFirst;
}

FileSystem::~FileSystem(void)
{
	VFileMapIter it;
	while (!m_vFileMap.empty()) {
		it = m_vFileMap.begin();
		it->second->release();
	}

	m_vFileMap.clear();
}

void FileSystem::setFileOpenType(EFileOpenType openType)
{
	m_fileOpenType = openType;
}

void FileSystem::setResourceRoot(string strPah)
{
	mResourceRoot = strPah;
}

// 载入一个pak文件
bool FileSystem::attachPakFile(const char* filename, const char* password)
{
	char pakFileName[MAX_PATH];
	char pakFileName2[MAX_PATH];
	strncpy(pakFileName2, filename, sizeof(pakFileName2)-1);
	NormalFileName(pakFileName2);
	Platform::strlwr(pakFileName2);
	sprintf(pakFileName, "%s%s", mResourceRoot.c_str(), pakFileName2);
	if (pakFileName2[1] != ':') {
		if (pakFileName2[0] != '/' && pakFileName2[0] != '\\' && pakFileName2[0] != '.')
			sprintf(pakFileName, "./%s", pakFileName2);
	}

	if (!m_pakFileMap.empty()) {
		PakFileMapIter itFind = m_pakFileMap.find(pakFileName);
		if (itFind != m_pakFileMap.end())
			return false;
	}

	unzFile zip;
	zip=unzOpen(pakFileName);
	if(!zip)
		return false;
	unzClose(zip);

	stPakFile pakFile;
	pakFile.filename = pakFileName;
	if (password != NULL) {
		pakFile.key = 1 + rand() % 250;
		size_t len = strlen(password);
		for (size_t i=0; i<len; ++i) {
			pakFile.password.push_back(password[i]);
			pakFile.password[i] = pakFile.password[i] ^ pakFile.key;
		}
	}

	m_pakFileMap[pakFileName] = pakFile;

	return true;
}

// 删除一个pak文件
bool FileSystem::detachPakFile(const char* filename)
{
	char pakFileName[MAX_PATH];
	strncpy(pakFileName, filename, sizeof(pakFileName)-1);
	NormalFileName(pakFileName);
	Platform::strlwr(pakFileName);

	PakFileMapIter itFind = m_pakFileMap.find(pakFileName);
	if (itFind == m_pakFileMap.end())
		return false;

	m_pakFileMap.erase(itFind);
	return true;
}

void FileSystem::detachAllPakFile()
{
	m_pakFileMap.clear();
}


VFile* FileSystem::openFile(const char* filename)
{
	VFile* pVFile = NULL;
	if (m_fileOpenType == PackFileFirst) {
		// 优先读取包中的文件
		pVFile = openFileFromPak(filename);
		if ( pVFile == NULL ) {
			pVFile = openFileFromLocal(filename);
		}
	} else {
		// 优先读取本地系统中的文件
		pVFile = openFileFromLocal(filename);
		if ( pVFile == NULL ) {
			pVFile = openFileFromPak(filename);
		}
	}

	// 记录打开的虚拟文件
	if (pVFile != NULL) {
		m_vFileMap[pVFile] = pVFile;
	}

	if (pVFile == NULL) {
		//LOGI("VFile打开文件 %s 失败", filename);
	}

	return pVFile;
}

void FileSystem::closeFile(VFile* pVFile)
{
	if (pVFile != NULL) {
		m_vFileMap.erase(pVFile);
		pVFile->release();
		delete pVFile;
	}
}

// 从压缩包中读取文件
VFile* FileSystem::openFileFromPak(const char* filename)
{
	if (m_pakFileMap.empty())
		return NULL;

	int done = 0;
	unzFile zip;
	unz_file_info file_info;
	char szName[MAX_PATH];
	memset(szName, 0, sizeof(szName));
	char szZipName[MAX_PATH];
	memset(szZipName, 0, sizeof(szZipName));
	strncpy(szName,filename,sizeof(szName)-1);
	NormalFileName(szName);
	Platform::strlwr(szName);
	if (szName[0] == '.' && szName[1] == '/') {
		// 去除"./"符号
		memcpy(szName, szName+2, sizeof(szName));
	}

	VFile* pVFile = NULL;

	for (PakFileMapIter it=m_pakFileMap.begin(); it != m_pakFileMap.end(); ++it) {
		stPakFile& pakFile = it->second;

		zip=unzOpen(pakFile.filename.c_str());
		done=unzGoToFirstFile(zip);

		while(done==UNZ_OK) {
			unzGetCurrentFileInfo(zip, &file_info, szZipName, sizeof(szZipName), NULL, 0, NULL, 0);
			NormalFileName(szZipName);
			Platform::strlwr(szZipName);

			if ( strcmp(szZipName,szName) != 0 ) {
				done=unzGoToNextFile(zip);
				continue;
			}

			char password[2048];
			memset(password,0,sizeof(password));
			if (pakFile.password.size() > 0) {
				size_t pswdLen = pakFile.password.size();
				for (size_t i=0; i<pswdLen; ++i) {
					password[i] = pakFile.password[i];
					password[i] = password[i] ^ pakFile.key;
				}
			}

			if(unzOpenCurrentFilePassword(zip, password[0] ? password : 0) != UNZ_OK)
			{
				unzClose(zip);
				return NULL;
			}
			memset(password,0,sizeof(password));

			pVFile = new VFile;
			if (pVFile == NULL) {
				unzCloseCurrentFile(zip);
				unzClose(zip);
				return NULL;
			}

			pVFile->m_ptrSrc = new byte[file_info.uncompressed_size];
			if (pVFile->m_ptrSrc == NULL) {
				delete pVFile;
				unzCloseCurrentFile(zip);
				unzClose(zip);
				return NULL;
			}
			pVFile->m_size = file_info.uncompressed_size;
			pVFile->m_ptrCur = pVFile->m_ptrSrc;

			memset(pVFile->m_ptrSrc,0,file_info.uncompressed_size);
			if(unzReadCurrentFile(zip, pVFile->m_ptrSrc, file_info.uncompressed_size) < 0) {
				pVFile->release();
				delete pVFile;
				unzCloseCurrentFile(zip);
				unzClose(zip);
				return NULL;
			}

			unzCloseCurrentFile(zip);
			unzClose(zip);
			return pVFile;
		}

		unzClose(zip);
	}

	return NULL;
}

// 从本地系统中读取文件
VFile* FileSystem::openFileFromLocal(const char* filename)
{
	char szFileName[MAX_PATH];
	sprintf(szFileName, "%s", filename);
	if (filename[1] != ':') {
		if (filename[0] != '/' && filename[0] != '\\' && filename[0] != '.')
			sprintf(szFileName, "./%s", filename);
	}

	FILE* hFile = fopen(filename, "r+b");
	if(hFile == NULL) {
		return NULL;
	}

	fseek(hFile, 0, SEEK_END);
	long fileLen = ftell(hFile);
	fseek(hFile, 0, SEEK_SET);

	VFile* pVFile = new VFile;
	if (pVFile == NULL) {
		fclose(hFile);
		return NULL;
	}

	pVFile->m_ptrSrc = new byte[fileLen];
	if (pVFile->m_ptrSrc == NULL) {
		delete pVFile;
		fclose(hFile);
		return NULL;
	}

	pVFile->m_ptrCur = pVFile->m_ptrSrc;
	pVFile->m_size = fileLen;
	memset(pVFile->m_ptrSrc, 0, fileLen);
	fread(pVFile->m_ptrSrc, fileLen, 1, hFile);
	fclose(hFile);

	return pVFile;
}




}	// namespace