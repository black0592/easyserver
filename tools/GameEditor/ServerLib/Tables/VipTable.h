
#ifndef __VIP_TABLE_READER_H__
#define __VIP_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct VipKey
	{
		VipKey( int _level = 0 )
			:level(_level)
		{}
		int                    level;
		bool operator < ( const VipKey &obj ) const;
	};
	struct VipInfo
	{
		VipInfo()
		{
		}
		~VipInfo()
		{
		}
		VipKey                 keyId;
		int                    exp;
		struct Gift
		{
			Gift()
			{
			}
			~Gift()
			{
			}
			int                   iItemId;
			int                   iCount;
		};
		vector<Gift>           gifts;

		typedef VipKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const VipInfo* rhs, const VipInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool VipKey::operator <( const VipKey & obj ) const
	{
		if(level != obj.level)
			return level < obj.level;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, VipKey &row )
	{
		data >> row.level;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, VipInfo &row )
	{
		data >> row.keyId;
		data >> row.exp;
		int nSize = 0;
		data >> nSize;
		row.gifts.resize( nSize );
		for( int k = 0; k < nSize; ++k )
		{
			data >> row.gifts[k].iItemId;
			data >> row.gifts[k].iCount;
		};

		return data;
	}
}
class VipReader : public TableReader<Store::VipInfo>, public Vek::Singleton<VipReader>
{
	friend class Vek::Singleton<VipReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("vip_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData vip_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	VipReader(){ InitializeX(); };
};

#endif