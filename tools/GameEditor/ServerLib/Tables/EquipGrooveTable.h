
#ifndef __EQUIPGROOVE_TABLE_READER_H__
#define __EQUIPGROOVE_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct EquipGrooveKey
	{
		EquipGrooveKey( int _typeLvId = 0 )
			:typeLvId(_typeLvId)
		{}
		int                    typeLvId;
		bool operator < ( const EquipGrooveKey &obj ) const;
	};
	struct stItemGroove
	{
		stItemGroove()
		{
		}
		~stItemGroove()
		{
		}
		EquipGrooveKey         keyId;
		int                    iStuffItemId;
		int                    iStuffCount;
		int                    iSuccessRate;
		int                    iNeedMoney;

		typedef EquipGrooveKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const stItemGroove* rhs, const stItemGroove* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool EquipGrooveKey::operator <( const EquipGrooveKey & obj ) const
	{
		if(typeLvId != obj.typeLvId)
			return typeLvId < obj.typeLvId;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, EquipGrooveKey &row )
	{
		data >> row.typeLvId;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, stItemGroove &row )
	{
		data >> row.keyId;
		data >> row.iStuffItemId;
		data >> row.iStuffCount;
		data >> row.iSuccessRate;
		data >> row.iNeedMoney;
		return data;
	}
}
class EquipGrooveReader : public TableReader<Store::stItemGroove>, public Vek::Singleton<EquipGrooveReader>
{
	friend class Vek::Singleton<EquipGrooveReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("equip_groove_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData equip_groove_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	EquipGrooveReader(){ InitializeX(); };
};

#endif