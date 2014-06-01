
#ifndef __GEMCOMPOSITE_TABLE_READER_H__
#define __GEMCOMPOSITE_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct GemCompositeKey
	{
		GemCompositeKey( int _itemId = 0 )
			:itemId(_itemId)
		{}
		int                    itemId;
		bool operator < ( const GemCompositeKey &obj ) const;
	};
	struct GemCompositeInfo
	{
		GemCompositeInfo()
		{
		}
		~GemCompositeInfo()
		{
		}
		GemCompositeKey        keyId;
		int                    iTargetItemId;
		int                    iComRate;
		int                    iNeedMoney;
		struct ConsumeItem
		{
			ConsumeItem()
			{
			}
			~ConsumeItem()
			{
			}
			int                   iConsumeItemId;
			int                   iConsumeItemCount;
		};
		vector<ConsumeItem>    items;

		typedef GemCompositeKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const GemCompositeInfo* rhs, const GemCompositeInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool GemCompositeKey::operator <( const GemCompositeKey & obj ) const
	{
		if(itemId != obj.itemId)
			return itemId < obj.itemId;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, GemCompositeKey &row )
	{
		data >> row.itemId;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, GemCompositeInfo &row )
	{
		data >> row.keyId;
		data >> row.iTargetItemId;
		data >> row.iComRate;
		data >> row.iNeedMoney;
		int nSize = 0;
		data >> nSize;
		row.items.resize( nSize );
		for( int k = 0; k < nSize; ++k )
		{
			data >> row.items[k].iConsumeItemId;
			data >> row.items[k].iConsumeItemCount;
		};

		return data;
	}
}
class GemCompositeReader : public TableReader<Store::GemCompositeInfo>, public Vek::Singleton<GemCompositeReader>
{
	friend class Vek::Singleton<GemCompositeReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("gem_composite_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData gem_composite_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	GemCompositeReader(){ InitializeX(); };
};

#endif