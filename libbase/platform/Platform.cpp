#include "platform/Platform.h"
#include "BaseMisc.h"
#include <time.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

#ifdef WIN32
#include <Windows.h>
#include <WinSock2.h>
#include <direct.h>
#else
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netdb.h>
#endif


/**
strftime 
Syntax: 
  #include <time.h>
  size_t strftime( char *str, size_t maxsize, const char *fmt, struct tm *time );

[en]
The function strftime() formats date and time information from time to a format specified by fmt, then stores the result in str (up to maxsize characters). Certain codes may be used in fmt to specify different types of time:

Code Meaning 
%a abbreviated weekday name (e.g. Fri) 
%A full weekday name (e.g. Friday) 
%b abbreviated month name (e.g. Oct) 
%B full month name (e.g. October) 
%c the standard date and time string 
%d day of the month, as a number (1-31) 
%H hour, 24 hour format (0-23) 
%I hour, 12 hour format (1-12) 
%j day of the year, as a number (1-366) 
%m month as a number (1-12). Note: some versions of Microsoft Visual C++ may use values that range from 0-11. 
%M minute as a number (0-59) 
%p locale's equivalent of AM or PM 
%S second as a number (0-59) 
%U week of the year, (0-53), where week 1 has the first Sunday 
%w weekday as a decimal (0-6), where Sunday is 0 
%W week of the year, (0-53), where week 1 has the first Monday 
%x standard date string 
%X standard time string 
%y year in decimal, without the century (0-99) 
%Y year in decimal, with the century 
%Z time zone name 
%% a percent sign 

The strftime() function returns the number of characters put into str, or zero if an error occurs.

[chs]
����strftime()�Ĳ�����Щ������sprintf()��ʶ���԰ٷֺ�(%)��ʼ�ĸ�ʽ����ϣ���ʽ������������һ���ַ����С���ʽ������˵����strDest�и������ں�ʱ����Ϣ��ȷ�б�ʾ��������ʽ���е������ַ�ԭ���Ž����С���ʽ�����������棬���������ִ�Сд�ġ�

%a ���ڼ��ļ�д
%A ���ڼ���ȫ��
%b �·ֵļ�д
%B �·ݵ�ȫ��
%c ��׼�����ڵ�ʱ�䴮
%C ��ݵĺ���λ����
%d ʮ���Ʊ�ʾ��ÿ�µĵڼ���
%D ��/��/��
%e �����ַ����У�ʮ���Ʊ�ʾ��ÿ�µĵڼ���
%F ��-��-��
%g ��ݵĺ���λ���֣�ʹ�û����ܵ���
%G ��֣�ʹ�û����ܵ���
%h ��д���·���
%H 24Сʱ�Ƶ�Сʱ
%I 12Сʱ�Ƶ�Сʱ
%j ʮ���Ʊ�ʾ��ÿ��ĵڼ���
%m ʮ���Ʊ�ʾ���·�
%M ʮʱ�Ʊ�ʾ�ķ�����
%n ���з�
%p ���ص�AM��PM�ĵȼ���ʾ
%r 12Сʱ��ʱ��
%R ��ʾСʱ�ͷ��ӣ�hh:mm
%S ʮ���Ƶ�����
%t ˮƽ�Ʊ��
%T ��ʾʱ���룺hh:mm:ss
%u ÿ�ܵĵڼ��죬����һΪ��һ�� ��ֵ��0��6������һΪ0��
%U ����ĵڼ��ܣ�����������Ϊ��һ�죨ֵ��0��53��
%V ÿ��ĵڼ��ܣ�ʹ�û����ܵ���
%w ʮ���Ʊ�ʾ�����ڼ���ֵ��0��6��������Ϊ0��
%W ÿ��ĵڼ��ܣ�������һ��Ϊ��һ�죨ֵ��0��53��
%x ��׼�����ڴ�
%X ��׼��ʱ�䴮
%y �������͵�ʮ������ݣ�ֵ��0��99��
%Y �����Ͳ��ֵ�ʮ�������
%z��%Z ʱ�����ƣ�������ܵõ�ʱ�������򷵻ؿ��ַ���
%% �ٷֺ�
*/



namespace easygame {


	//////////////////////////////////////////////////////////////////////////
	// ͨ��
	char * Platform::format(char * buff, const T_TIME & time)
	{
		sprintf(buff, "%04d-%02d-%02d %02d:%02d:%02d",
			time.tm_year, time.tm_mon, time.tm_mday, 
			time.tm_hour, time.tm_min, time.tm_sec);
		return buff;
	}
	char * Platform::format(char * buff, int len, const char * fmt, const T_TIME & time)
	{
		strftime(buff, len, fmt, &time);
		return buff;
	}
	char * Platform::fmttime(char * buff, const T_TIME & time)
	{
		sprintf(buff, "%02d:%02d:%02d",
			time.tm_hour, time.tm_min, time.tm_sec);
		return buff;
	}
	char * Platform::fmtdate(char * buff, const T_TIME & time)
	{
		sprintf(buff, "%04d-%02d-%02d",
			time.tm_year, time.tm_mon, time.tm_mday);
		return buff;
	}

	void Platform::replaceBacklashPath(char* path)
	{
		while (*path) {
			if (*path == '\\')
				*path = '/';
			++path;
		}
	}

	void Platform::replaceFrontlashPath(char* path)
	{
		while (*path) {
			if (*path == '/')
				*path = '\\';
			++path;
		}
	}

	void Platform::createDirectorys(const char* path)
	{
		char szFileNameTmp[1024];
		strncpy(szFileNameTmp, path, 1024);
		replaceBacklashPath(szFileNameTmp);

		std::string strPathName = szFileNameTmp;
		size_t pos = strPathName.find_last_of("\\");
		std::string strPath = strPathName.substr(0,pos);
		std::string strName = strPathName.substr(pos+1,strPathName.length());

		vector<string> list;
		splitString(list, strPath, "/");
		string strPathTemp;
		for (size_t i=0; i<list.size(); i++) {
			strPathTemp += list[i];
			strPathTemp += "/";
			if (strPathTemp == "./")
				continue;

			if (!Platform::isDirExist(strPathTemp.c_str()))
				Platform::mkdir(strPathTemp.c_str());
		}
	}

	bool Platform::getIPByDomain(char* szIP, const char* domain)
	{
		hostent *ent;
		ent = gethostbyname(domain);
		if (ent == NULL) return false;
		if (ent) {
			sprintf(szIP,"%d.%d.%d.%d", 
				(byte)ent->h_addr_list[0][0],
				(byte)ent->h_addr_list[0][1],
				(byte)ent->h_addr_list[0][2],
				(byte)ent->h_addr_list[0][3]);
		}

		return true;
	}

#ifdef WIN32

	//////////////////////////////////////////////////////////////////////////
	// win32
	char* Platform::strlwr( char* str )
	{
		return ::_strlwr(str);
	}

	int Platform::strcmpi(const char* str1, const char* str2)
	{
		return ::_strcmpi(str1, str2);
	}

	string Platform::utf8ToGbk(string strUtf8)
	{
		if (strUtf8.empty())
			return "";

		const int MAX_LEN = 4096;
		char szBuf[MAX_LEN];
		WCHAR wszBuf[MAX_LEN] = {0};
		MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, wszBuf, sizeof(wszBuf));
		WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);

		return szBuf;
	}

	//long Platform::clock()
	//{
	//	// ���CLOCKS_PER_SEC��1000�Ļ���ֱ�ӵ��ɺ�����
	//	// ��   = ::clock() / CLOCKS_PER_SEC
	//	// ���� = ::clock() / CLOCKS_PER_SEC * 1000
	//	return ::clock();
	//}

	void Platform::sleep(ulong milliseconds)
	{
		::Sleep(milliseconds);
	}

	int64 Platform::tick()
	{
		timeb t;
		ftime(&t);
		return 1000 * t.time + t.millitm;
	}

	void Platform::now(T_TIME & time)
	{
		SYSTEMTIME ct;
		GetLocalTime(&ct);
		time.tm_mday = ct.wDay;
		time.tm_wday = ct.wDayOfWeek;
		time.tm_hour = ct.wHour;
		time.tm_min = ct.wMinute;
		time.tm_mon = ct.wMonth;
		time.tm_sec = ct.wSecond;
		time.tm_year = ct.wYear;
	}

	void Platform::mkdir(const char* dir)
	{
		string strPath = dir;
		strPath += "\\";
		//::CreateDirectory(strPath.c_str(), NULL);
		_mkdir(strPath.c_str());
	}

	bool Platform::isDirExist(const char* dir)
	{
		uint dwFA = GetFileAttributes(dir);
		return ((dwFA != 0xffffffff) && (dwFA & FILE_ATTRIBUTE_DIRECTORY));
	}

	void Platform::setWindowTitle(const char* szTitle)
	{
		::SetConsoleTitle(szTitle);
	}

	int Platform::getNumberOfProcessors()
	{
		SYSTEM_INFO SystemInfo;
		GetSystemInfo(&SystemInfo);
		return (int)SystemInfo.dwNumberOfProcessors; 
	}

#else

	//////////////////////////////////////////////////////////////////////////
	// linux
	char* Platform::strlwr( char* str )
	{
		char* orig = str;
		// process the string
		for ( ; *str != '\0'; str++ )
			*str = tolower(*str);

		return orig;
	}

	int Platform::strcmpi(const char* str1, const char* str2)
	{
		return ::strcasecmp(str1, str2);
	}

	string Platform::utf8ToGbk(string strUtf8)
	{
		return strUtf8;
	}

	//long Platform::clock()
	//{
	//	//struct timespec ts;
	//	//clock_gettime(CLOCK_MONOTONIC,&ts);                      //�˴������ж�һ�·���ֵ
	//	//return ( ts.tv_sec*1000 + ts.tv_nsec/(1000*1000) );
	//	return (long)((float)::clock() / 1000.0f);
	//}

	void Platform::sleep(ulong milliseconds)
	{
		::usleep(milliseconds * 1000);
	}

	int64 Platform::tick()
	{
		timeb t;
		ftime(&t);
		return 1000 * t.time + t.millitm;
	}

	void Platform::now(T_TIME & time)
	{
		time_t ct = ::time(NULL);
		localtime_r(&ct, &time);
		time.tm_year += 1900;
	}

	void Platform::mkdir(const char* dir)
	{
		::mkdir(dir, 0777);
	}

	bool Platform::isDirExist(const char* dir)
	{
		return ::access(dir, R_OK) == 0;
	}

	void Platform::setWindowTitle(const char* szTitle)
	{
	}

	int Platform::getNumberOfProcessors()
	{
		return (int)sysconf(_SC_NPROCESSORS_ONLN);
	}

#endif



}	// namespace
