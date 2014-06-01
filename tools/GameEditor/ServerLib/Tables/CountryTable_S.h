
#ifndef __COUNTRY_TABLE_READER_H__
#define __COUNTRY_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct CountryKey
	{
		CountryKey( int _typeId = 0 )
			:typeId(_typeId)
		{}
		int                    typeId;
		bool operator < ( const CountryKey &obj ) const;
	};
	struct CountryInfo
	{
		CountryInfo()
			:Name( NULL )
			,Icon( NULL )
		{
		}
		~CountryInfo()
		{
			delete [] Name;
			delete [] Icon;
		}
		CountryKey             keyId;
		const char*            Name;
		const char*            Icon;

		typedef CountryKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const CountryInfo* rhs, const CountryInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool CountryKey::operator <( const CountryKey & obj ) const
	{
		if(typeId != obj.typeId)
			return typeId < obj.typeId;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, CountryKey &row )
	{
		data >> row.typeId;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, CountryInfo &row )
	{
		data >> row.keyId;
		data >> row.Name;
		data >> row.Icon;
		return data;
	}
}
class CountryReader : public TableReader<Store::CountryInfo>, public Vek::Singleton<CountryReader>
{
	friend class Vek::Singleton<CountryReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("country_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData country_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	CountryReader(){ InitializeX(); };
};

#endif