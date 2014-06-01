
#ifndef __GEMATTRIBUTE_TABLE_READER_H__
#define __GEMATTRIBUTE_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct GemAttributeKey
	{
		GemAttributeKey( int _itemId = 0 )
			:itemId(_itemId)
		{}
		int                    itemId;
		bool operator < ( const GemAttributeKey &obj ) const;
	};
	struct GemAttributeInfo
	{
		GemAttributeInfo()
		{
		}
		~GemAttributeInfo()
		{
		}
		GemAttributeKey        keyId;
		float                  iAttack;
		float                  iPhyDef;
		float                  iMagDef;
		float                  iHP;
		float                  iMP;
		float                  iMiss;
		float                  iHit;
		float                  iCritical;
		float                  iToughness;
		int                    iGrade;

		typedef GemAttributeKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const GemAttributeInfo* rhs, const GemAttributeInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool GemAttributeKey::operator <( const GemAttributeKey & obj ) const
	{
		if(itemId != obj.itemId)
			return itemId < obj.itemId;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, GemAttributeKey &row )
	{
		data >> row.itemId;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, GemAttributeInfo &row )
	{
		data >> row.keyId;
		data >> row.iAttack;
		data >> row.iPhyDef;
		data >> row.iMagDef;
		data >> row.iHP;
		data >> row.iMP;
		data >> row.iMiss;
		data >> row.iHit;
		data >> row.iCritical;
		data >> row.iToughness;
		data >> row.iGrade;
		return data;
	}
}
class GemAttributeReader : public TableReader<Store::GemAttributeInfo>, public Vek::Singleton<GemAttributeReader>
{
	friend class Vek::Singleton<GemAttributeReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("gem_attribute_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData gem_attribute_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	GemAttributeReader(){ InitializeX(); };
};

#endif