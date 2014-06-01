
#ifndef __ONLINEAWARD_TABLE_READER_H__
#define __ONLINEAWARD_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct OnlineAwardKey
	{
		OnlineAwardKey( int _Id = 0 )
			:Id(_Id)
		{}
		int                    Id;
		bool operator < ( const OnlineAwardKey &obj ) const;
	};
	struct OnlineAwardInfo
	{
		OnlineAwardInfo()
		{
		}
		~OnlineAwardInfo()
		{
		}
		OnlineAwardKey         keyId;
		int                    nTime;
		int                    nAward;
		int                    nCount;

		typedef OnlineAwardKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const OnlineAwardInfo* rhs, const OnlineAwardInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool OnlineAwardKey::operator <( const OnlineAwardKey & obj ) const
	{
		if(Id != obj.Id)
			return Id < obj.Id;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, OnlineAwardKey &row )
	{
		data >> row.Id;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, OnlineAwardInfo &row )
	{
		data >> row.keyId;
		data >> row.nTime;
		data >> row.nAward;
		data >> row.nCount;
		return data;
	}
}
class OnlineAwardReader : public TableReader<Store::OnlineAwardInfo>, public Vek::Singleton<OnlineAwardReader>
{
	friend class Vek::Singleton<OnlineAwardReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("online_award_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData online_award_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	OnlineAwardReader(){ InitializeX(); };
};

#endif