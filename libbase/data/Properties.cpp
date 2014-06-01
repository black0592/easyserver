#include "BaseLib.h"
#include "Properties.h"

namespace easygame {

	Properties::Properties(void)
	{
		m_props.clear();
	}

	Properties::~Properties(void)
	{
	}

	// ��������ַ���
	void Properties::parse(const char* strSrc, const char* split)
	{
		string strData = strSrc;
		vector<string> vecData;
		splitString(vecData, strData, split);
		for (vector<string>::iterator it = vecData.begin(); it != vecData.end(); ++it) {
			vector<string> ssv;
			splitString(ssv, *it, "=", 1);
			if (2 == ssv.size()) {
				m_props[ssv[0]] = ssv[1];
			}
		}
	}

	// �������
	void Properties::clear()
	{
		m_props.clear();
	}

	// ����һ��ֵ
	void Properties::addValue(const char* key, const char* value)
	{
		m_props[key] = value;
	}

	void Properties::addValue(const char* key, uint value)
	{
		char buff[256];
		sprintf(buff, "%u", value);
		m_props[key] = buff;
	}

	// ����key, ���һ��ֵ
	const char* Properties::getValue(const char* key)
	{
		PropMapItr itFind = m_props.find(key);
		if (itFind == m_props.end())
			return "NULL";

		return m_props[key].c_str();
	}

	// ת�����ַ���
	void Properties::toSting(string& strContent, const char* split)
	{
		for (PropMapItr it; it != m_props.end(); ++it) {
			strContent += it->first;
			strContent += "=";
			strContent += it->second;
			strContent += split;
		}
	}

	// ��ӡ
	void Properties::print()
	{
		for (PropMapItr it=m_props.begin(); it != m_props.end(); ++it) {
			printf("%s=%s\n", it->first.c_str(), it->second.c_str());
		}
	}



}	// namespace

