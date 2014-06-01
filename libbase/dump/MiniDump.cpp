#include "MiniDump.h"
#include "platform/Platform.h"

#ifdef WIN32

#if ENABLE_MINIDUMP

#include "ExceptionCapture.h"
#include <string>
using namespace std;


string getTimeStamp()
{
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);
	char temp[1024];
	sprintf(temp,"%04d%02d%02d_%02d%02d%02d", 
		localTime.wYear, localTime.wMonth, localTime.wDay,
		localTime.wHour,localTime.wMinute,localTime.wSecond);
	string strTime = temp;
	return strTime;
}

namespace MiniDump
{
	string g_dumpPath = "./crashlog/";
	string g_dumpName = "DumpFile";

	LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
	{
		// 先生成目录
		easygame::Platform::createDirectorys(g_dumpPath.c_str());

		// 输出minidump文件
		string timestamp = getTimeStamp();
		string strFilePrev = g_dumpPath;
		strFilePrev += g_dumpName;
		strFilePrev += "_";
		strFilePrev += timestamp;
		string strDumpFile = strFilePrev;
		strDumpFile += ".dmp";
		HANDLE lhDumpFile = CreateFile(strDumpFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL ,NULL);
		MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
		loExceptionInfo.ExceptionPointers = ExceptionInfo;
		loExceptionInfo.ThreadId = GetCurrentThreadId();
		loExceptionInfo.ClientPointers = TRUE;
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);
		CloseHandle(lhDumpFile);

		// 日志文件
		string strStackFile = strFilePrev;
		strStackFile += ".txt";
		FileStackWalker fsw(strStackFile.c_str());
		fsw.ShowCallstack(GetCurrentThread(), ExceptionInfo->ContextRecord);

		return EXCEPTION_EXECUTE_HANDLER;
	}



	void InitMiniDump(const char* path, const char* name)
	{
		g_dumpPath = path;
		g_dumpName = name;
		SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	}

};


#endif	// ENABLE_MINIDUMP

#else	// WIN32

namespace MiniDump{
	void InitMiniDump(const char* path, const char* name)
	{
		//nothing
	}
};

#endif	// WIN32

