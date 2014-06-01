#pragma once

#include <string>
using namespace std;

#ifdef WIN32

class CodeConvert
{
public:  
	static void UTF8ToUnicode(wchar_t* pOut,char *pText);  // 把UTF-8转换成Unicode  
	static void UnicodeToUTF8(char* pOut,wchar_t* pText);  //Unicode 转换成UTF-8  
	static void UnicodeToGB2312(char* pOut,wchar_t uData);  // 把Unicode 转换成 GB2312   
	static void Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer);// GB2312 转换成　Unicode  
	static void GB2312ToUTF8(string& pOut,char *pText, int pLen);//GB2312 转为 UTF-8  
	static void UTF8ToGB2312(string &pOut, char *pText, int pLen);//UTF-8 转为 GB2312  
};


#endif