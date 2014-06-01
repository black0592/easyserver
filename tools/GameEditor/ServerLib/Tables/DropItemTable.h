
#ifndef __DROPITEM_TABLE_READER_H__
#define __DROPITEM_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct DropItemKey
	{
		DropItemKey( int _id = 0 )
			:id(_id)
		{}
		int                    id;
		bool operator < ( const DropItemKey &obj ) const;
	};
	struct DropItem
	{
		DropItem()
			:pDesc( NULL )
		{
		}
		~DropItem()
		{
			delete [] pDesc;
		}
		DropItemKey            keyId;
		const char*            pDesc;
		struct Drops
		{
			Drops()
			{
			}
			~Drops()
			{
			}
			int                   iItemId;
			int                   iProbability;
		};
		vector<Drops>          items;

		typedef DropItemKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const DropItem* rhs, const DropItem* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool DropItemKey::operator <( const DropItemKey & obj ) const
	{
		if(id != obj.id)
			return id < obj.id;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, DropItemKey &row )
	{
		data >> row.id;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, DropItem &row )
	{
		data >> row.keyId;
		data >> row.pDesc;
		int nSize = 0;
		data >> nSize;
		row.items.resize( nSize );
		for( int k = 0; k < nSize; ++k )
		{
			data >> row.items[k].iItemId;
			data >> row.items[k].iProbability;
		};

		return data;
	}
}
class DropItemReader : public TableReader<Store::DropItem>, public Vek::Singleton<DropItemReader>
{
	friend class Vek::Singleton<DropItemReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("drop_item_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData drop_item_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	DropItemReader(){ InitializeX(); };
};

#endif