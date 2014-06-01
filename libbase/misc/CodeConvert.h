#pragma once

#include <string>
using namespace std;

#ifdef WIN32

class CodeConvert
{
public:  
	static void UTF8ToUnicode(wchar_t* pOut,char *pText);  // ��UTF-8ת����Unicode  
	static void UnicodeToUTF8(char* pOut,wchar_t* pText);  //Unicode ת����UTF-8  
	static void UnicodeToGB2312(char* pOut,wchar_t uData);  // ��Unicode ת���� GB2312   
	static void Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer);// GB2312 ת���ɡ�Unicode  
	static void GB2312ToUTF8(string& pOut,char *pText, int pLen);//GB2312 תΪ UTF-8  
	static void UTF8ToGB2312(string &pOut, char *pText, int pLen);//UTF-8 תΪ GB2312  
};


#endif