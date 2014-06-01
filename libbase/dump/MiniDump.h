#ifndef __MINIDUMP_H__
#define __MINIDUMP_H__

#include "BaseConfig.h"

#ifdef WIN32

#if ENABLE_MINIDUMP

#include <windows.h>
#include <Dbghelp.h>
//#pragma auto_inline (off)
#pragma comment( lib, "DbgHelp" )

namespace MiniDump{
	// 初始化dump参数（name不要扩展名）
	void InitMiniDump(const char* path, const char* name);

#if ENABLE_MINIDUMP
	// 异常处理
	LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo);
#endif
};

#endif	// ENABLE_MINIDUMP


#if ENABLE_MINIDUMP

#define TRY		__try
#define EXCEPT	__except(MiniDump::MyUnhandledExceptionFilter(GetExceptionInformation()))

#else

#define TRY
#define EXCEPT

#endif	// ENABLE_MINIDUMP

#else	// WIN32

namespace MiniDump{
	void InitMiniDump(const char* path, const char* name);
};

#endif	// WIN32

#endif
