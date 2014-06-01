
#ifndef __ITEMCLASSIFY_TABLE_READER_H__
#define __ITEMCLASSIFY_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct ItemClassifyKey
	{
		ItemClassifyKey( int _mainId = 0 )
			:mainId(_mainId)
		{}
		int                    mainId;
		bool operator < ( const ItemClassifyKey &obj ) const;
	};
	struct ItemClassifyInfo
	{
		ItemClassifyInfo()
			:mainType( NULL )
		{
		}
		~ItemClassifyInfo()
		{
			delete [] mainType;
		}
		ItemClassifyKey        keyId;
		const char*            mainType;
		struct SubClassifys
		{
			SubClassifys()
				:pSubType( NULL )
			{
			}
			~SubClassifys()
			{
				delete [] pSubType;
			}
			const char*           pSubType;
			int                   eItemType;
		};
		vector<SubClassifys>   subTypes;

		typedef ItemClassifyKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const ItemClassifyInfo* rhs, const ItemClassifyInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool ItemClassifyKey::operator <( const ItemClassifyKey & obj ) const
	{
		if(mainId != obj.mainId)
			return mainId < obj.mainId;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, ItemClassifyKey &row )
	{
		data >> row.mainId;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, ItemClassifyInfo &row )
	{
		data >> row.keyId;
		data >> row.mainType;
		int nSize = 0;
		data >> nSize;
		row.subTypes.resize( nSize );
		for( int k = 0; k < nSize; ++k )
		{
			data >> row.subTypes[k].pSubType;
			data >> row.subTypes[k].eItemType;
		};

		return data;
	}
}
class ItemClassifyReader : public TableReader<Store::ItemClassifyInfo>, public Vek::Singleton<ItemClassifyReader>
{
	friend class Vek::Singleton<ItemClassifyReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("item_classify_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData item_classify_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	ItemClassifyReader(){ InitializeX(); };
};

#endif