
#ifndef __GUILDLEVEL_TABLE_READER_H__
#define __GUILDLEVEL_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct GuildLevelKey
	{
		GuildLevelKey( int _level = 0 )
			:level(_level)
		{}
		int                    level;
		bool operator < ( const GuildLevelKey &obj ) const;
	};
	struct GuildLevelInfo
	{
		GuildLevelInfo()
		{
		}
		~GuildLevelInfo()
		{
		}
		GuildLevelKey          keyId;
		int                    iRequireOre;
		int                    iRequireCrystal;
		int                    iRequirePoint;
		int                    iShopId;

		typedef GuildLevelKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const GuildLevelInfo* rhs, const GuildLevelInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool GuildLevelKey::operator <( const GuildLevelKey & obj ) const
	{
		if(level != obj.level)
			return level < obj.level;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, GuildLevelKey &row )
	{
		data >> row.level;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, GuildLevelInfo &row )
	{
		data >> row.keyId;
		data >> row.iRequireOre;
		data >> row.iRequireCrystal;
		data >> row.iRequirePoint;
		data >> row.iShopId;
		return data;
	}
}
class GuildLevelReader : public TableReader<Store::GuildLevelInfo>, public Vek::Singleton<GuildLevelReader>
{
	friend class Vek::Singleton<GuildLevelReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("guild_level_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData guild_level_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	GuildLevelReader(){ InitializeX(); };
};

#endif