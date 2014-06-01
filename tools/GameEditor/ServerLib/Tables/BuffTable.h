
#ifndef __BUFF_TABLE_READER_H__
#define __BUFF_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct BuffKey
	{
		BuffKey( int _typeId = 0 )
			:typeId(_typeId)
		{}
		int                    typeId;
		bool operator < ( const BuffKey &obj ) const;
	};
	struct BuffInfo
	{
		BuffInfo()
			:pName( NULL )
			,pIcon( NULL )
			,pEffect( NULL )
			,pDesc( NULL )
		{
		}
		~BuffInfo()
		{
			delete [] pName;
			delete [] pIcon;
			delete [] pEffect;
			delete [] pDesc;
		}
		BuffKey                keyId;
		const char*            pName;
		const char*            pIcon;
		const char*            pEffect;
		const char*            pDesc;

		typedef BuffKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const BuffInfo* rhs, const BuffInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool BuffKey::operator <( const BuffKey & obj ) const
	{
		if(typeId != obj.typeId)
			return typeId < obj.typeId;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, BuffKey &row )
	{
		data >> row.typeId;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, BuffInfo &row )
	{
		data >> row.keyId;
		data >> row.pName;
		data >> row.pIcon;
		data >> row.pEffect;
		data >> row.pDesc;
		return data;
	}
}
class BuffReader : public TableReader<Store::BuffInfo>, public Vek::Singleton<BuffReader>
{
	friend class Vek::Singleton<BuffReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("buff_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData buff_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	BuffReader(){ InitializeX(); };
};

#endif