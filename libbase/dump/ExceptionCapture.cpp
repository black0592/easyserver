#include "ExceptionCapture.h"

#ifdef WIN32
#if ENABLE_MINIDUMP


FileStackWalker::FileStackWalker(const char* outputFile)
{
	strncpy(m_szFileName,outputFile, MAX_PATH-1);
	m_pFile = NULL;
}


FileStackWalker::~FileStackWalker()
{
	if (m_pFile) {
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

void FileStackWalker::OnOutput(LPCSTR szText)
{
	if (m_pFile == NULL) {
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		//char fileName[MAX_PATH];
		//sprintf(fileName, "%s.%04d-%02d-%02d_%02d_%02d_%02d", m_szFileName,
		//	tm.wYear,tm.wMonth,tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
		//m_pFile = fopen(fileName, "w+t");

		// 使用外部传入的文件名
		m_pFile = fopen(m_szFileName, "w+t");
	}

	if (m_pFile != NULL) {
		fwrite(szText, strlen(szText), 1, m_pFile);
	}

	printf(szText);
	StackWalker::OnOutput(szText);
}


//////////////////////////////////////////////////////////////////////////

char ExceptionCapture::m_szFileName[MAX_PATH];

ExceptionCapture::ExceptionCapture(const char* outputFile)
{
	strncpy(m_szFileName,outputFile, MAX_PATH-1);
	SetUnhandledExceptionFilter( UnhandledExceptionFilter );
}

ExceptionCapture::~ExceptionCapture(void)
{
}

LONG ExceptionCapture::UnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	FileStackWalker fsw(m_szFileName);
	fsw.ShowCallstack(GetCurrentThread(), ExceptionInfo->ContextRecord);
	return EXCEPTION_EXECUTE_HANDLER;
}



#endif		// ENABLE_MINIDUMP
#endif		// WIN32