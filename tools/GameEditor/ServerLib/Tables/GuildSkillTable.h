
#ifndef __GUILDSKILL_TABLE_READER_H__
#define __GUILDSKILL_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct GuildSkillKey
	{
		GuildSkillKey( int _id = 0, int _level = 0 )
			:id(_id)
			,level(_level)
		{}
		int                    id;
		int                    level;
		bool operator < ( const GuildSkillKey &obj ) const;
	};
	struct GuildSkillInfo
	{
		GuildSkillInfo()
		{
		}
		~GuildSkillInfo()
		{
		}
		GuildSkillKey          keyId;
		int                    iRequireOre;
		int                    iRequireCrystal;
		int                    iRequirePoint;
		int                    iRequireGuildLv;
		int                    iStudyCost;

		typedef GuildSkillKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const GuildSkillInfo* rhs, const GuildSkillInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool GuildSkillKey::operator <( const GuildSkillKey & obj ) const
	{
		if(id != obj.id)
			return id < obj.id;
		if(level != obj.level)
			return level < obj.level;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, GuildSkillKey &row )
	{
		data >> row.id;
		data >> row.level;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, GuildSkillInfo &row )
	{
		data >> row.keyId;
		data >> row.iRequireOre;
		data >> row.iRequireCrystal;
		data >> row.iRequirePoint;
		data >> row.iRequireGuildLv;
		data >> row.iStudyCost;
		return data;
	}
}
class GuildSkillReader : public TableReader<Store::GuildSkillInfo>, public Vek::Singleton<GuildSkillReader>
{
	friend class Vek::Singleton<GuildSkillReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("guild_skill_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData guild_skill_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	GuildSkillReader(){ InitializeX(); };
};

#endif