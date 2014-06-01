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

	// 输入解析字符串
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

	// 清空数据
	void Properties::clear()
	{
		m_props.clear();
	}

	// 增加一个值
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

	// 根据key, 获得一个值
	const char* Properties::getValue(const char* key)
	{
		PropMapItr itFind = m_props.find(key);
		if (itFind == m_props.end())
			return "NULL";

		return m_props[key].c_str();
	}

	// 转换到字符串
	void Properties::toSting(string& strContent, const char* split)
	{
		for (PropMapItr it; it != m_props.end(); ++it) {
			strContent += it->first;
			strContent += "=";
			strContent += it->second;
			strContent += split;
		}
	}

	// 打印
	void Properties::print()
	{
		for (PropMapItr it=m_props.begin(); it != m_props.end(); ++it) {
			printf("%s=%s\n", it->first.c_str(), it->second.c_str());
		}
	}



}	// namespace

