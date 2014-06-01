#include "stdafx.h"
#include "TableHelp.h"

namespace Store
{
	std::string GetStorePath(const char* pcFileName)
	{
		static const std::string strStoresDir = EditorConfig::Instance()->makeServerResPath("tables/");
		return strStoresDir+pcFileName;
	}
}
