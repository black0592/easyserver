
#ifndef __SKILL_TABLE_READER_H__
#define __SKILL_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct SkillKey
	{
		SkillKey( int _id = 0, int _level = 0 )
			:id(_id)
			,level(_level)
		{}
		int                    id;
		int                    level;
		bool operator < ( const SkillKey &obj ) const;
	};
	struct stSkillBase
	{
		stSkillBase()
			:name( NULL )
		{
		}
		~stSkillBase()
		{
			delete [] name;
		}
		SkillKey               keyId;
		const char*            name;
		int                    useType;
		int                    requireLevel;
		int                    requireRaceMask;
		int                    distance;
		int                    needHP;
		int                    needMP;
		int                    attrib;
		int                    targetNum;
		int                    coolDown;
		bool                   isAttack;
		struct BuffObj
		{
			BuffObj()
				:desc( NULL )
			{
			}
			~BuffObj()
			{
				delete [] desc;
			}
			int                   target;
			int                   buffId;
			bool                  bSaveDB;
			int                   buffTime;
			int                   excuteIntervalTime;
			int                   param1;
			int                   param2;
			int                   param3;
			int                   param4;
			const char*           desc;
		};
		vector<BuffObj>        buffs;

		typedef SkillKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const stSkillBase* rhs, const stSkillBase* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool SkillKey::operator <( const SkillKey & obj ) const
	{
		if(id != obj.id)
			return id < obj.id;
		if(level != obj.level)
			return level < obj.level;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, SkillKey &row )
	{
		data >> row.id;
		data >> row.level;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, stSkillBase &row )
	{
		data >> row.keyId;
		data >> row.name;
		data >> row.useType;
		data >> row.requireLevel;
		data >> row.requireRaceMask;
		data >> row.distance;
		data >> row.needHP;
		data >> row.needMP;
		data >> row.attrib;
		data >> row.targetNum;
		data >> row.coolDown;
		data >> row.isAttack;
		int nSize = 0;
		data >> nSize;
		row.buffs.resize( nSize );
		for( int k = 0; k < nSize; ++k )
		{
			data >> row.buffs[k].target;
			data >> row.buffs[k].buffId;
			data >> row.buffs[k].bSaveDB;
			data >> row.buffs[k].buffTime;
			data >> row.buffs[k].excuteIntervalTime;
			data >> row.buffs[k].param1;
			data >> row.buffs[k].param2;
			data >> row.buffs[k].param3;
			data >> row.buffs[k].param4;
			data >> row.buffs[k].desc;
		};

		return data;
	}
}
class SkillReader : public TableReader<Store::stSkillBase>, public Vek::Singleton<SkillReader>
{
	friend class Vek::Singleton<SkillReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("skill_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData skill_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	SkillReader(){ InitializeX(); };
};

#endif