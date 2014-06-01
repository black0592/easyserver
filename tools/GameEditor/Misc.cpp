#include "StdAfx.h"
#include ".\misc.h"
#include "CountryTable_S.h"
#include "CodeConvert.h"

Misc::Misc(void)
{
}

Misc::~Misc(void)
{
}

void Misc::writeUTF8Str(ofstream& filestream, const char* gkbString)
{
	std::string strTemp = gkbString;
	CodeConvert::GB2312ToUTF8(strTemp, (char*)strTemp.c_str(), strTemp.size()) ;  
	filestream << strTemp.c_str() ;  
}

const char* Misc::countrName(UINT countryId)
{
	static string strCountryName;

	const CountryInfo* info = CountryReader::Instance()->Item(countryId);
	if (info != NULL) {
		strCountryName = utf8ToGbk(info->Name);
	} else {
		strCountryName = "(null)";
	}

	return strCountryName.c_str();

		/*
	switch(countryId)
	{
	case 1:
		return "玛尔斯";
		break;
	case 2:
		return "艾伯斯";
	    break;
	case 3:
		return "伊格斯";
	    break;
	case 4:
		return "菲尼克斯";
		break;
	default:
		return "默认";
	    break;
	}
	*/

	return "NULL";
}

const char* Misc::getNpcFuncStr(UINT func)
{
	static CString strFunc = "";
	strFunc = "";
	switch(func)
	{
	case NPC_Normal:
		strFunc = "普通对话";
		break;
	case NPC_BuySell:	// NPC商店
		strFunc = "NPC商店";
		break;
	case NPC_Warehouse:
		strFunc = "仓库管理";
		break;
	case NPC_Marriage:	// 婚姻管理
		strFunc = "婚姻管理";
		break;
	case NPC_GuildMgr:	// 工会管理
		strFunc = "工会管理";
		break;
	case NPC_Attrib:	// 属性
		strFunc = "属性管理";
		break;
	case NPC_NationMgr:	// 国家管理
		strFunc = "国家管理";
		break;
	case NPC_GuildWar:	// 工会战
		strFunc = "工会战";
		break;
	case NPC_RunEattach:// ？？？？
		strFunc = "不知道2";
		break;
	case NPC_Gamble:	// 赌博
		strFunc = "赌博管理";
		break;
	case NPC_NONE:	// 无
		strFunc = "装饰物件";
		break;
	default:
		break;
	}

	return strFunc;
}

string Misc::utf8ToGbk(string strUtf8)
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


// 地图坐标转换
uint Misc::pixel2CellX(uint x)
{
	return x / CXTILE;
}

uint Misc::pixel2CellY(uint y)
{
	return y / CYTILE;
}


uint Misc::cell2PixelX(uint x)
{
	return x * CXTILE + CXTILE / 2;
}

uint Misc::cell2PixelY(uint y)
{
	return y * CYTILE + CYTILE / 2;
}