#ifndef __BASE_MISC_H__
#define __BASE_MISC_H__

#include <math.h>
#include <stdio.h>
#include <stdarg.h> 
#include <string>
#include <vector>
#ifdef WIN32
#include <Windows.h>
#endif
using namespace std;

#define SAFE_DELETE(p)		 { delete (p); (p) = NULL; }
#define SAFE_DELETE_ARRAY(p) { delete[] (p); (p) = NULL; }

#if _MSC_VER >= 1400 //vs 2005 or higher
#define DEPRECATED __declspec(deprecated) 
#else
#define DEPRECATED
#endif 

//手动调用构造函数，不分配内存
template <class _T1>
inline void constructInPlace(_T1* _p)
{
	new (static_cast<void*>(_p)) _T1();
}

//#define BUFF_CMD(ptr,type)    \
//	char buffer##type[MAX_USER_PACK_SIZE];     \
//	memset(buffer##type,0,MAX_USER_PACK_SIZE); \
//	ptr = (type*)buffer##type;         \
//	ptr->type::type();

#define BUFF_CMD(ptr,type)	\
	char buffer##ptr[MAX_USER_CMD_SIZE];	\
	memset(buffer##ptr, 0, MAX_USER_CMD_SIZE);	\
	ptr = (type*)buffer##ptr;	\
	constructInPlace(ptr);


#ifdef WIN32
#ifndef bzero
#define bzero(data,size) memset(data, 0, size);
#endif
#endif

template<typename Container>
__inline void splitString(Container& container, const string& in, const char* delimiters = "\t\n", int deep = 0)
{
	size_t len = in.length();
	size_t i = 0;
	int count = 0;
	while (i < len) {
		i = in.find_first_not_of(delimiters, i);
		if (i == string::npos)
			return;

		size_t j = in.find_first_of(delimiters, i);
		count++;
		if (j == string::npos || (deep > 0 && count > deep)) {
			string strTmp = in.substr(i);
			container.push_back(strTmp);
			return;
		} else {
			string strTmp = in.substr(i, j-i);
			container.push_back(strTmp);
		}
		i = j + 1;
	}
}

__inline bool isFileExist(const char* filename)
{
	FILE* file = fopen(filename,"r+b");
	if (file == NULL)
		return false;
	fclose(file);
	return true;
}

// 计算数据中的元素个数
#define count_of(data) sizeof(data)/sizeof(data[0])


// wstring 和 string互转
#define ws2s(str) std::string(str.begin(), str.end())
#define s2ws(str) std::wstring(str.begin(), str.end())

// 数据裁剪
template<typename T>
T clipNumber(T val, T min, T max)
{
	if (val < min)
		val = min;
	
	if (val > max)
		val = max;

	return val;
}

// 计算平均值
template<typename T>
T average(T d1, T d2)
{
	T total = d1 + d2;
	T aver = total / 2;
	return aver;
}

// 获得字符串格式的IP地址
__inline string getStrIP(int ip)
{
	char strTemp[21];
	sprintf( strTemp,"%u.%u.%u.%u",
		(ip&0x000000ff),
		(ip&0x0000ff00)>>8,
		(ip&0x00ff0000)>>16,
		(ip&0xff000000)>>24);

	return string(strTemp);
}

__inline int getDigitalIP(string ip)
{
	vector<string> list;
	splitString(list, ip, ".");
	int ip1 = atoi(list[0].c_str());
	int ip2 = atoi(list[1].c_str());
	int ip3 = atoi(list[2].c_str());
	int ip4 = atoi(list[3].c_str());
	int DigitalIP = (ip4<<24) | (ip3<<16) | (ip2<<8) | (ip1);
	return DigitalIP;
}

// 非线程安全
__inline const char* strformat(const char* pszFmt, ...)
{
	static char szBuffer[ 1024 ];
	va_list ap;
	va_start( ap, pszFmt );
	vsnprintf( szBuffer, 1024, pszFmt, ap );
	szBuffer[ 1023 ] = 0;
	va_end( ap );
	return szBuffer;
}

//__inline void hexToStrLower(char* buffer, uint value)
//{
//	static const char HEX[16] = {
//		'0', '1', '2', '3',
//		'4', '5', '6', '7',
//		'8', '9', 'a', 'b',
//		'c', 'd', 'e', 'f'
//	};
//
//	int a = value / 16;
//	int b = value % 16;
//	buffer[0] = HEX[a];
//	buffer[1] = HEX[b];
//}
//
//__inline void hexToStrUpper(char* buffer, uint value)
//{
//	static const char HEX[16] = {
//		'0', '1', '2', '3',
//		'4', '5', '6', '7',
//		'8', '9', 'A', 'B',
//		'C', 'D', 'E', 'F'
//	};
//
//	int a = value / 16;
//	int b = value % 16;
//	buffer[0] = HEX[a];
//	buffer[1] = HEX[b];
//}

__inline bool equal_float(float f1, float f2, float precision = 0.0001f)
{
	if (fabs(f1-f2) < precision)
		return true;

	return false;
}

__inline bool equal_double(double f1, double f2, double precision = 0.0000001f)
{
	if (fabs(f1-f2) < precision)
		return true;

	return false;
}

__inline void trace(const char* msg, const char* file, int line)
{
	char buffer[1024] = {0};
	sprintf(buffer,"\r\n%s(%d), [!!!error!!!]%s\r\n", file, line, msg);
	printf("%s", buffer);
#ifdef WIN32
	OutputDebugString(buffer);
#endif
}

#define TRACE(msg) trace(msg,__FILE__,__LINE__)


#ifndef WIN32
struct POINT 
{
	long x;
	long y;
};

struct SIZE
{
	long cx;
	long cy;
};

struct RECT
{
	long    left;
	long    top;
	long    right;
	long    bottom;
};

__inline bool PtInRect(const RECT *lprc, POINT pt)
{
	if (pt.x < lprc->left)
		return false;

	if (pt.x > lprc->right)
		return false;

	if (pt.y < lprc->top)
		return false;

	if (pt.y > lprc->bottom)
		return false;

	return true;
}

#endif

#endif

