#include "BaseLib.h"
#include "IniParser.h"

#ifdef WIN32
#include <Windows.h>
#endif

namespace easygame {

	IniParser::IniParser(void)
	{
		m_filename = "";
	}

	IniParser::~IniParser(void)
	{
	}


	// ����ini�ļ�
	bool IniParser::load(const char* filename)
	{
		if (!isFileExist(filename))
			return false;

		m_filename = filename;
		return true;
	}

	bool IniParser::load(const void* data, uint len)
	{
		//string strData = (const char*)data;
		//vector<string> lines;
		//splitString(lines, strData, "\r\n");
		//lines

		return true;
	}

	// ��ȡ��������
	int IniParser::readInt(const char* app, const char* key, int defaultValue)
	{
#ifdef WIN32
		return GetPrivateProfileInt(app, key, defaultValue, m_filename.c_str());
#else
		return 0;
#endif
	}

	// ��ȡ�ַ�������
	string IniParser::readString(const char* app, const char* key, const char* defaultValue)
	{
		string value = "";
#ifdef WIN32
		char szTemp[250];
		GetPrivateProfileString(app, key, defaultValue, szTemp, sizeof(szTemp), m_filename.c_str());
		value = szTemp;
#endif
		return value;
	}

	// д����������
	void IniParser::writeInt(const char* app, const char* key, int value)
	{
#ifdef WIN32
		char szBuff[250];
		sprintf(szBuff, "%d", value);
		WritePrivateProfileString(app, key, szBuff, m_filename.c_str());
#endif
	}

	// д���ַ�������
	void IniParser::writeString(const char* app, const char* key, const char* value)
	{
#ifdef WIN32
		WritePrivateProfileString(app, key, value, m_filename.c_str());
#endif
	}

}	// namespace

