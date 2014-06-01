
#ifndef __NPC_TABLE_READER_H__
#define __NPC_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct NpcKey
	{
		NpcKey( int _typeId = 0 )
			:typeId(_typeId)
		{}
		int                    typeId;
		bool operator < ( const NpcKey &obj ) const;
	};
	struct stNpcBase
	{
		stNpcBase()
			:name( NULL )
		{
		}
		~stNpcBase()
		{
			delete [] name;
		}
		NpcKey                 keyId;
		int                    id;
		const char*            name;
		wstring                name1;
		int                    imgId;
		int                    func;

		typedef NpcKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const stNpcBase* rhs, const stNpcBase* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool NpcKey::operator <( const NpcKey & obj ) const
	{
		if(typeId != obj.typeId)
			return typeId < obj.typeId;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, NpcKey &row )
	{
		data >> row.typeId;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, stNpcBase &row )
	{
		data >> row.keyId;
		data >> row.id;
		data >> row.name;
		data >> row.name1;
		data >> row.imgId;
		data >> row.func;
		return data;
	}
}
class NpcReader : public TableReader<Store::stNpcBase>, public Vek::Singleton<NpcReader>
{
	friend class Vek::Singleton<NpcReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("npc_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData npc_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	NpcReader(){ InitializeX(); };
};

#endif