
#ifndef __ITEM_TABLE_READER_H__
#define __ITEM_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct ItemKey
	{
		ItemKey( int _id = 0 )
			:id(_id)
		{}
		int                    id;
		bool operator < ( const ItemKey &obj ) const;
	};
	struct stItemBase
	{
		stItemBase()
			:name( NULL )
			,desc( NULL )
			,scriptCode( NULL )
		{
		}
		~stItemBase()
		{
			delete [] name;
			delete [] desc;
			delete [] scriptCode;
		}
		ItemKey                keyId;
		int                    id;
		const char*            name;
		wstring                name1;
		int                    type;
		int                    requireLevel;
		int                    quality;
		int                    requireRace;
		int                    price;
		int                    overlay;
		bool                   sellable;
		bool                   mailable;
		bool                   saleable;
		int                    validDay;
		const char*            desc;
		int                    icon;
		const char*            scriptCode;

		typedef ItemKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const stItemBase* rhs, const stItemBase* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool ItemKey::operator <( const ItemKey & obj ) const
	{
		if(id != obj.id)
			return id < obj.id;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, ItemKey &row )
	{
		data >> row.id;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, stItemBase &row )
	{
		data >> row.keyId;
		data >> row.id;
		data >> row.name;
		data >> row.name1;
		data >> row.type;
		data >> row.requireLevel;
		data >> row.quality;
		data >> row.requireRace;
		data >> row.price;
		data >> row.overlay;
		data >> row.sellable;
		data >> row.mailable;
		data >> row.saleable;
		data >> row.validDay;
		data >> row.desc;
		data >> row.icon;
		data >> row.scriptCode;
		return data;
	}
}
class ItemReader : public TableReader<Store::stItemBase>, public Vek::Singleton<ItemReader>
{
	friend class Vek::Singleton<ItemReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("item_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData item_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	ItemReader(){ InitializeX(); };
};

#endif