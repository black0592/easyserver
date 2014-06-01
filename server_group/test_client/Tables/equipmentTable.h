
#ifndef __EQUIPMENT_TABLE_READER_H__
#define __EQUIPMENT_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct EquipmentKey
	{
		EquipmentKey( int _id = 0 )
			:id(_id)
		{}
		int                    id;
		bool operator < ( const EquipmentKey &obj ) const;
	};
	struct stEquipBase
	{
		stEquipBase()
			:name( NULL )
		{
		}
		~stEquipBase()
		{
			delete [] name;
		}
		EquipmentKey           keyId;
		int                    id;
		const char*            name;
		wstring                name1;
		int                    avatar;
		int                    equipIndex;
		bool                   bIsAvatar;
		int                    attack;
		int                    phyDefence;
		int                    magDefence;
		int                    critical;
		int                    toughness;
		int                    accuracy;
		int                    miss;
		int                    maxHP;
		int                    maxMP;
		int                    dur;
		int                    repairable;
		int                    needStr;
		int                    needDex;
		int                    needWiz;
		int                    needInt;
		int                    needCon;

		typedef EquipmentKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const stEquipBase* rhs, const stEquipBase* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool EquipmentKey::operator <( const EquipmentKey & obj ) const
	{
		if(id != obj.id)
			return id < obj.id;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, EquipmentKey &row )
	{
		data >> row.id;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, stEquipBase &row )
	{
		data >> row.keyId;
		data >> row.id;
		data >> row.name;
		data >> row.name1;
		data >> row.avatar;
		data >> row.equipIndex;
		data >> row.bIsAvatar;
		data >> row.attack;
		data >> row.phyDefence;
		data >> row.magDefence;
		data >> row.critical;
		data >> row.toughness;
		data >> row.accuracy;
		data >> row.miss;
		data >> row.maxHP;
		data >> row.maxMP;
		data >> row.dur;
		data >> row.repairable;
		data >> row.needStr;
		data >> row.needDex;
		data >> row.needWiz;
		data >> row.needInt;
		data >> row.needCon;
		return data;
	}
}
class EquipmentReader : public TableReader<Store::stEquipBase>, public Vek::Singleton<EquipmentReader>
{
	friend class Vek::Singleton<EquipmentReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("equipment_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData equipment_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	EquipmentReader(){ InitializeX(); };
};

#endif