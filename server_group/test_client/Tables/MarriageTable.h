
#ifndef __MARRIAGE_TABLE_READER_H__
#define __MARRIAGE_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct MarriageKey
	{
		MarriageKey( int _Id = 0 )
			:Id(_Id)
		{}
		int                    Id;
		bool operator < ( const MarriageKey &obj ) const;
	};
	struct MarriageInfo
	{
		MarriageInfo()
			:pType( NULL )
		{
		}
		~MarriageInfo()
		{
			delete [] pType;
		}
		MarriageKey            keyId;
		const char*            pType;
		int                    iCost;
		struct SendItem
		{
			SendItem()
			{
			}
			~SendItem()
			{
			}
			int                   iItemId;
			int                   iCount;
		};
		vector<SendItem>       items;

		typedef MarriageKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const MarriageInfo* rhs, const MarriageInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool MarriageKey::operator <( const MarriageKey & obj ) const
	{
		if(Id != obj.Id)
			return Id < obj.Id;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, MarriageKey &row )
	{
		data >> row.Id;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, MarriageInfo &row )
	{
		data >> row.keyId;
		data >> row.pType;
		data >> row.iCost;
		int nSize = 0;
		data >> nSize;
		row.items.resize( nSize );
		for( int k = 0; k < nSize; ++k )
		{
			data >> row.items[k].iItemId;
			data >> row.items[k].iCount;
		};

		return data;
	}
}
class MarriageReader : public TableReader<Store::MarriageInfo>, public Vek::Singleton<MarriageReader>
{
	friend class Vek::Singleton<MarriageReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("marriage_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData marriage_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	MarriageReader(){ InitializeX(); };
};

#endif