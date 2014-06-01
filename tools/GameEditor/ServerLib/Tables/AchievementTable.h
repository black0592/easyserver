
#ifndef __ACHIEVEMENT_TABLE_READER_H__
#define __ACHIEVEMENT_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct AchievementKey
	{
		AchievementKey( int _id = 0 )
			:id(_id)
		{}
		int                    id;
		bool operator < ( const AchievementKey &obj ) const;
	};
	struct AchievementInfo
	{
		AchievementInfo()
			:pTitle( NULL )
			,pTarget( NULL )
		{
		}
		~AchievementInfo()
		{
			delete [] pTitle;
			delete [] pTarget;
		}
		AchievementKey         keyId;
		int                    iRequireId;
		const char*            pTitle;
		const char*            pTarget;
		int                    eShowType;
		int                    eJudType;
		int                    iParam;
		struct Award
		{
			Award()
			{
			}
			~Award()
			{
			}
			int                   iItemId;
			int                   iCount;
		};
		vector<Award>          awards;

		typedef AchievementKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const AchievementInfo* rhs, const AchievementInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool AchievementKey::operator <( const AchievementKey & obj ) const
	{
		if(id != obj.id)
			return id < obj.id;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, AchievementKey &row )
	{
		data >> row.id;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, AchievementInfo &row )
	{
		data >> row.keyId;
		data >> row.iRequireId;
		data >> row.pTitle;
		data >> row.pTarget;
		data >> row.eShowType;
		data >> row.eJudType;
		data >> row.iParam;
		int nSize = 0;
		data >> nSize;
		row.awards.resize( nSize );
		for( int k = 0; k < nSize; ++k )
		{
			data >> row.awards[k].iItemId;
			data >> row.awards[k].iCount;
		};

		return data;
	}
}
class AchievementReader : public TableReader<Store::AchievementInfo>, public Vek::Singleton<AchievementReader>
{
	friend class Vek::Singleton<AchievementReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("achievement_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData achievement_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	AchievementReader(){ InitializeX(); };
};

#endif