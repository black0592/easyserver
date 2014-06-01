#ifndef __EXCEPTION_CAPTURE_H__
#define __EXCEPTION_CAPTURE_H__

#include "BaseConfig.h"

#ifdef WIN32
#if ENABLE_MINIDUMP
#include "StackWalker.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>


// ���ļ����
class FileStackWalker : public StackWalker
{
public:
	FileStackWalker(const char* outputFile);
	virtual ~FileStackWalker();

protected:
	virtual void OnOutput(LPCSTR szText);

private:
	char m_szFileName[MAX_PATH];
	FILE* m_pFile;
};


// �쳣��׽
// ������ 
// ��ȫ�������������£��Ϳ�������
// ExceptionCapture expCapture("./crashlog/crash.log");
class ExceptionCapture
{
public:
	ExceptionCapture(const char* outputFile);
	virtual ~ExceptionCapture(void);
	static LONG WINAPI UnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo);

private:
	static char m_szFileName[MAX_PATH];
};



#endif	// ENABLE_MINIDUMP
#endif	// WIN32


#endif
