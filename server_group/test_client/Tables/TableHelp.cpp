#include "ClientHeader.h"
#include "TableHelp.h"

namespace Store
{
	std::string GetStorePath(const char* pcFileName)
	{
		static const std::string strStoresDir = ( "datas/tables/" );
		return strStoresDir+pcFileName;
	}
}
