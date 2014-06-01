
#ifndef __MAINMISSION_TABLE_READER_H__
#define __MAINMISSION_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct MainMissionKey
	{
		MainMissionKey( int _id = 0 )
			:id(_id)
		{}
		int                    id;
		bool operator < ( const MainMissionKey &obj ) const;
	};
	struct MainMissionInfo
	{
		MainMissionInfo()
			:pName( NULL )
			,pTargetDesc( NULL )
			,pBeforeDesc( NULL )
			,pAfterDesc( NULL )
		{
		}
		~MainMissionInfo()
		{
			delete [] pName;
			delete [] pTargetDesc;
			delete [] pBeforeDesc;
			delete [] pAfterDesc;
		}
		MainMissionKey         keyId;
		const char*            pName;
		int                    iRequireMissionId;
		int                    eMissionType;
		int                    iRequireLevel;
		int                    iMaxLevel;
		int                    eMissionRarity;
		int                    eTargetType;
		int                    iTargetParams1;
		int                    iTargetParams2;
		int                    iTargetParams3;
		const char*            pTargetDesc;
		const char*            pBeforeDesc;
		const char*            pAfterDesc;
		struct RaceAward
		{
			RaceAward()
				:pAward1Params( NULL )
				,pAward2Params( NULL )
			{
			}
			~RaceAward()
			{
				delete [] pAward1Params;
				delete [] pAward2Params;
			}
			int                   eRace;
			int                   iExp;
			int                   iMoney;
			const char*           pAward1Params;
			const char*           pAward2Params;
		};
		vector<RaceAward>      awards;

		typedef MainMissionKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const MainMissionInfo* rhs, const MainMissionInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool MainMissionKey::operator <( const MainMissionKey & obj ) const
	{
		if(id != obj.id)
			return id < obj.id;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, MainMissionKey &row )
	{
		data >> row.id;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, MainMissionInfo &row )
	{
		data >> row.keyId;
		data >> row.pName;
		data >> row.iRequireMissionId;
		data >> row.eMissionType;
		data >> row.iRequireLevel;
		data >> row.iMaxLevel;
		data >> row.eMissionRarity;
		data >> row.eTargetType;
		data >> row.iTargetParams1;
		data >> row.iTargetParams2;
		data >> row.iTargetParams3;
		data >> row.pTargetDesc;
		data >> row.pBeforeDesc;
		data >> row.pAfterDesc;
		int nSize = 0;
		data >> nSize;
		row.awards.resize( nSize );
		for( int k = 0; k < nSize; ++k )
		{
			data >> row.awards[k].eRace;
			data >> row.awards[k].iExp;
			data >> row.awards[k].iMoney;
			data >> row.awards[k].pAward1Params;
			data >> row.awards[k].pAward2Params;
		};

		return data;
	}
}
class MainMissionReader : public TableReader<Store::MainMissionInfo>, public Vek::Singleton<MainMissionReader>
{
	friend class Vek::Singleton<MainMissionReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("main_mission_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData main_mission_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	MainMissionReader(){ InitializeX(); };
};

#endif