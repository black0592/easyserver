
#ifndef __MAP_TABLE_READER_H__
#define __MAP_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct MapKey
	{
		MapKey( int _Id = 0 )
			:Id(_Id)
		{}
		int                    Id;
		bool operator < ( const MapKey &obj ) const;
	};
	struct stMapInfo
	{
		stMapInfo()
			:name( NULL )
			,mapfile( NULL )
		{
		}
		~stMapInfo()
		{
			delete [] name;
			delete [] mapfile;
		}
		MapKey                 keyId;
		int                    id;
		const char*            name;
		int                    country;
		int                    eMapAttribute;
		const char*            mapfile;
		bool                   safeFlag;
		int                    pk_x1;
		int                    pk_y1;
		int                    pk_x2;
		int                    pk_y2;

		typedef MapKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const stMapInfo* rhs, const stMapInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool MapKey::operator <( const MapKey & obj ) const
	{
		if(Id != obj.Id)
			return Id < obj.Id;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, MapKey &row )
	{
		data >> row.Id;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, stMapInfo &row )
	{
		data >> row.keyId;
		data >> row.id;
		data >> row.name;
		data >> row.country;
		data >> row.eMapAttribute;
		data >> row.mapfile;
		data >> row.safeFlag;
		data >> row.pk_x1;
		data >> row.pk_y1;
		data >> row.pk_x2;
		data >> row.pk_y2;
		return data;
	}
}
class MapReader : public TableReader<Store::stMapInfo>, public Vek::Singleton<MapReader>
{
	friend class Vek::Singleton<MapReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("map_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData map_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	MapReader(){ InitializeX(); };
};

#endif