
#ifndef __EQUIPUPGRADE_TABLE_READER_H__
#define __EQUIPUPGRADE_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct EquipUpgradeKey
	{
		EquipUpgradeKey( int _typeLvId = 0 )
			:typeLvId(_typeLvId)
		{}
		int                    typeLvId;
		bool operator < ( const EquipUpgradeKey &obj ) const;
	};
	struct stItemStar
	{
		stItemStar()
		{
		}
		~stItemStar()
		{
		}
		EquipUpgradeKey        keyId;
		int                    iStuffItemId;
		int                    iStuffCount;
		int                    iSuccessRate;
		int                    iNeedMoney;
		int                    iFactor;

		typedef EquipUpgradeKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const stItemStar* rhs, const stItemStar* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool EquipUpgradeKey::operator <( const EquipUpgradeKey & obj ) const
	{
		if(typeLvId != obj.typeLvId)
			return typeLvId < obj.typeLvId;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, EquipUpgradeKey &row )
	{
		data >> row.typeLvId;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, stItemStar &row )
	{
		data >> row.keyId;
		data >> row.iStuffItemId;
		data >> row.iStuffCount;
		data >> row.iSuccessRate;
		data >> row.iNeedMoney;
		data >> row.iFactor;
		return data;
	}
}
class EquipUpgradeReader : public TableReader<Store::stItemStar>, public Vek::Singleton<EquipUpgradeReader>
{
	friend class Vek::Singleton<EquipUpgradeReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("equip_upgrade_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData equip_upgrade_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	EquipUpgradeReader(){ InitializeX(); };
};

#endif