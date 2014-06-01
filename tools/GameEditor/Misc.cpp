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
		return "���˹";
		break;
	case 2:
		return "����˹";
	    break;
	case 3:
		return "����˹";
	    break;
	case 4:
		return "�����˹";
		break;
	default:
		return "Ĭ��";
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
		strFunc = "��ͨ�Ի�";
		break;
	case NPC_BuySell:	// NPC�̵�
		strFunc = "NPC�̵�";
		break;
	case NPC_Warehouse:
		strFunc = "�ֿ����";
		break;
	case NPC_Marriage:	// ��������
		strFunc = "��������";
		break;
	case NPC_GuildMgr:	// �������
		strFunc = "�������";
		break;
	case NPC_Attrib:	// ����
		strFunc = "���Թ���";
		break;
	case NPC_NationMgr:	// ���ҹ���
		strFunc = "���ҹ���";
		break;
	case NPC_GuildWar:	// ����ս
		strFunc = "����ս";
		break;
	case NPC_RunEattach:// ��������
		strFunc = "��֪��2";
		break;
	case NPC_Gamble:	// �Ĳ�
		strFunc = "�Ĳ�����";
		break;
	case NPC_NONE:	// ��
		strFunc = "װ�����";
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


// ��ͼ����ת��
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