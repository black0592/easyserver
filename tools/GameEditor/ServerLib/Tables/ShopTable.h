
#ifndef __SHOP_TABLE_READER_H__
#define __SHOP_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct ShopKey
	{
		ShopKey( int _id = 0 )
			:id(_id)
		{}
		int                    id;
		bool operator < ( const ShopKey &obj ) const;
	};
	struct ShopInfo
	{
		ShopInfo()
			:pDesc( NULL )
		{
		}
		~ShopInfo()
		{
			delete [] pDesc;
		}
		ShopKey                keyId;
		const char*            pDesc;
		int                    eCurrencyType;
		struct Goods
		{
			Goods()
			{
			}
			~Goods()
			{
			}
			int                   iItemId;
			int                   iPrice;
		};
		vector<Goods>          goods;

		typedef ShopKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const ShopInfo* rhs, const ShopInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool ShopKey::operator <( const ShopKey & obj ) const
	{
		if(id != obj.id)
			return id < obj.id;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, ShopKey &row )
	{
		data >> row.id;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, ShopInfo &row )
	{
		data >> row.keyId;
		data >> row.pDesc;
		data >> row.eCurrencyType;
		int nSize = 0;
		data >> nSize;
		row.goods.resize( nSize );
		for( int k = 0; k < nSize; ++k )
		{
			data >> row.goods[k].iItemId;
			data >> row.goods[k].iPrice;
		};

		return data;
	}
}
class ShopReader : public TableReader<Store::ShopInfo>, public Vek::Singleton<ShopReader>
{
	friend class Vek::Singleton<ShopReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("shop_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData shop_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	ShopReader(){ InitializeX(); };
};

#endif