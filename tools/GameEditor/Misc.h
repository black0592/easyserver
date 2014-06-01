#pragma once
#include "stdafx.h"

class Misc
{
public:
	Misc(void);
	virtual ~Misc(void);

	static const char* countrName(UINT countryId);
	static const char* getNpcFuncStr(UINT func);

	static void writeUTF8Str(ofstream& filestream, const char* gkbString);

	static string utf8ToGbk(string strUtf8);

	// µØÍ¼×ø±ê×ª»»
	static uint pixel2CellX(uint x);
	static uint pixel2CellY(uint y);

	static uint cell2PixelX(uint x);
	static uint cell2PixelY(uint y);

};


