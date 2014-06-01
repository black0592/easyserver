#ifndef	__PLATFORM_H__
#define __PLATFORM_H__
#include "BaseTypes.h"
#include <time.h>

namespace easygame
{
	typedef struct tm T_TIME;

	class Platform
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		// 字符串操作
		static char* strlwr( char* str );
		static int strcmpi(const char* str1, const char* str2);
		static string utf8ToGbk(string strUtf8);

		//////////////////////////////////////////////////////////////////////////
		// 时间操作
		//static long clock();
		static int64 tick();
		static void sleep(ulong milliseconds);
		static void now(T_TIME & time);
		static char* format(char* buff, const T_TIME& time);
		static char* format(char* buff, int len, const char* fmt, const T_TIME& time);
		static char* fmttime(char* buff, const T_TIME& time);
		static char* fmtdate(char* buff, const T_TIME& time);

		//////////////////////////////////////////////////////////////////////////
		// 文件操作
		static void replaceBacklashPath(char* path);		// to '/'
		static void replaceFrontlashPath(char* path);		// to '\'
		static void createDirectorys(const char* path);
		static void mkdir(const char* dir);
		static bool isDirExist(const char* dir);

		//////////////////////////////////////////////////////////////////////////
		// 网路
		static bool getIPByDomain(char* szIP, const char* domain);


		//////////////////////////////////////////////////////////////////////////
		// 系统信息
		static int getNumberOfProcessors();
		

		//////////////////////////////////////////////////////////////////////////
		// 窗口
		static void setWindowTitle(const char* szTitle);


		//static void getLocalTime(tm& tv, time_t timeValue);
	};







}	// namespace



#endif