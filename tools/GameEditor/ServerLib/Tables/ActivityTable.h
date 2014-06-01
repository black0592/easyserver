
#ifndef __ACTIVITY_TABLE_READER_H__
#define __ACTIVITY_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct ActivityKey
	{
		ActivityKey( int _id = 0 )
			:id(_id)
		{}
		int                    id;
		bool operator < ( const ActivityKey &obj ) const;
	};
	struct ActivityInfo
	{
		ActivityInfo()
			:pName( NULL )
			,pActivityIntroduce( NULL )
			,pAwardIntroduce( NULL )
			,pHint( NULL )
			,pOpenWeekend( NULL )
		{
		}
		~ActivityInfo()
		{
			delete [] pName;
			delete [] pActivityIntroduce;
			delete [] pAwardIntroduce;
			delete [] pHint;
			delete [] pOpenWeekend;
		}
		ActivityKey            keyId;
		const char*            pName;
		int                    eActivityType;
		int                    eActivityState;
		int                    iLevel;
		const char*            pActivityIntroduce;
		const char*            pAwardIntroduce;
		const char*            pHint;
		int                    iTimes;
		int                    iBeginDay;
		int                    iEndDay;
		double                 iBeginTime;
		double                 iEndTime;
		const char*            pOpenWeekend;
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

		typedef ActivityKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const ActivityInfo* rhs, const ActivityInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool ActivityKey::operator <( const ActivityKey & obj ) const
	{
		if(id != obj.id)
			return id < obj.id;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, ActivityKey &row )
	{
		data >> row.id;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, ActivityInfo &row )
	{
		data >> row.keyId;
		data >> row.pName;
		data >> row.eActivityType;
		data >> row.eActivityState;
		data >> row.iLevel;
		data >> row.pActivityIntroduce;
		data >> row.pAwardIntroduce;
		data >> row.pHint;
		data >> row.iTimes;
		data >> row.iBeginDay;
		data >> row.iEndDay;
		data >> row.iBeginTime;
		data >> row.iEndTime;
		data >> row.pOpenWeekend;
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
class ActivityReader : public TableReader<Store::ActivityInfo>, public Vek::Singleton<ActivityReader>
{
	friend class Vek::Singleton<ActivityReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("activity_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData activity_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	ActivityReader(){ InitializeX(); };
};

#endif