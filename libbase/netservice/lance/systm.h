/***

Copyright 2006 bsmith@qq.com

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/


/*
# 
# 
# @author		bsmith, bsmith@qq.com
# @date			2005-02-01
# @version		beta 0.0.0001
# @corp
# @describe	part of Lance Misc.
# @copyright
# @remark		time
# 

*/


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



#pragma once


#ifdef LANCE_WIN32
	#include <stdio.h>
	#include <time.h>
	#include <windows.h>
//	#ifndef _DEBUG
		#pragma warning(disable : 4996)
//	#endif
#endif

#ifdef LANCE_LINUX
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
	#include <sys/time.h> 
#endif

#ifdef LANCE_FREEBSD
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
	#include <sys/time.h> 
#endif


namespace lance
{

/**
win32/linux system time.
*/
namespace systm
{





#ifdef LANCE_LINUX
typedef struct tm T_TIME;
inline long clock()
{
	return ::clock()/1000;
}
inline long tick()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec*1000+tv.tv_usec/1000;
}
inline void sleep(int ms)
{
	struct timespec req;
	req.tv_nsec = ms%1000;
	req.tv_sec = (ms-req.tv_nsec)/1000;
	req.tv_nsec = req.tv_nsec*1000000;
	struct timespec rem;
	nanosleep(&req, &rem);
}
inline void now(T_TIME & time)
{
	time_t ct = ::time(NULL);
	localtime_r(&ct, &time);
	time.tm_year += 1900;
}
inline void utcnow(T_TIME & time)
{
	time_t ct = ::time(NULL);
	gmtime_r(&ct, &time);
	time.tm_year += 1900;
}
#endif

#ifdef LANCE_FREEBSD
typedef struct tm T_TIME;
inline long clock()
{
	return ::clock()/1000;
}
inline long tick()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec*1000+tv.tv_usec/1000;
}
inline void sleep(int ms)
{
	struct timespec req;
	req.tv_nsec = ms%1000;
	req.tv_sec = (ms-req.tv_nsec)/1000;
	req.tv_nsec = req.tv_nsec*1000000;
	struct timespec rem;
	nanosleep(&req, &rem);
}
inline void now(T_TIME & time)
{
	time_t ct = ::time(NULL);
	localtime_r(&ct, &time);
	time.tm_year += 1900;
}
inline void utcnow(T_TIME & time)
{
	time_t ct = ::time(NULL);
	gmtime_r(&ct, &time);
	time.tm_year += 1900;
}
#endif

#ifdef LANCE_WIN32


typedef struct tm T_TIME;
inline long clock()
{
	return ::clock();
}
inline long tick()
{
	return Platform::tick();
}
inline void sleep(int ms)
{
	Sleep(ms);
}
inline void now(T_TIME & time)
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
inline void utcnow(T_TIME & time)
{
	SYSTEMTIME ct;
	GetSystemTime(&ct);
	time.tm_mday = ct.wDay;
	time.tm_wday = ct.wDayOfWeek;
	time.tm_hour = ct.wHour;
	time.tm_min = ct.wMinute;
	time.tm_mon = ct.wMonth;
	time.tm_sec = ct.wSecond;
	time.tm_year = ct.wYear;
}
#endif

inline char * format(char * buff, const T_TIME & time)
{
	sprintf(buff, "%04d-%02d-%02d %02d:%02d:%02d",
			time.tm_year, time.tm_mon, time.tm_mday, 
			time.tm_hour, time.tm_min, time.tm_sec);
	return buff;
}
inline char * format(char * buff, int len, const char * fmt, const T_TIME & time)
{
	strftime(buff, len, fmt, &time);
	return buff;
}
inline char * fmttime(char * buff, const T_TIME & time)
{
	sprintf(buff, "%02d:%02d:%02d",
			time.tm_hour, time.tm_min, time.tm_sec);
	return buff;
}
inline char * fmtdate(char * buff, const T_TIME & time)
{
	sprintf(buff, "%04d-%02d-%02d",
			time.tm_year, time.tm_mon, time.tm_mday);
	return buff;
}


}

}
















