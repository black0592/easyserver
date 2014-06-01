
#ifndef __MONSTER_TABLE_READER_H__
#define __MONSTER_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct MonsterKey
	{
		MonsterKey( int _typeId = 0 )
			:typeId(_typeId)
		{}
		int                    typeId;
		bool operator < ( const MonsterKey &obj ) const;
	};
	struct stMonBase
	{
		stMonBase()
		{
		}
		~stMonBase()
		{
		}
		MonsterKey             keyId;
		int                    id;
		string                 name;
		wstring                name1;
		int                    imgId;
		int                    level;
		int                    type;
		int                    maxHP;
		int                    addHP;
		int                    phyAttack;
		int                    magAttack;
		int                    phyDefence;
		int                    magDefence;
		int                    getExp;
		int                    dropMoney;
		int                    dropMoneyProb;
		int                    magicClass;
		int                    prop;
		int                    propValue;
		int                    flight;
		int                    sickness;
		int                    sicknessProb;

		typedef MonsterKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const stMonBase* rhs, const stMonBase* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool MonsterKey::operator <( const MonsterKey & obj ) const
	{
		if(typeId != obj.typeId)
			return typeId < obj.typeId;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, MonsterKey &row )
	{
		data >> row.typeId;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, stMonBase &row )
	{
		data >> row.keyId;
		data >> row.id;
		data >> row.name;
		data >> row.name1;
		data >> row.imgId;
		data >> row.level;
		data >> row.type;
		data >> row.maxHP;
		data >> row.addHP;
		data >> row.phyAttack;
		data >> row.magAttack;
		data >> row.phyDefence;
		data >> row.magDefence;
		data >> row.getExp;
		data >> row.dropMoney;
		data >> row.dropMoneyProb;
		data >> row.magicClass;
		data >> row.prop;
		data >> row.propValue;
		data >> row.flight;
		data >> row.sickness;
		data >> row.sicknessProb;
		return data;
	}
}
class MonsterReader : public TableReader<Store::stMonBase>, public Vek::Singleton<MonsterReader>
{
	friend class Vek::Singleton<MonsterReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("monster_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData monster_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	MonsterReader(){ InitializeX(); };
};

#endif