
#ifndef __TEXT_TABLE_READER_H__
#define __TEXT_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct TextKey
	{
		TextKey( int _typeId = 0 )
			:typeId(_typeId)
		{}
		int                    typeId;
		bool operator < ( const TextKey &obj ) const;
	};
	struct stTextInfo
	{
		stTextInfo()
			:pText( NULL )
		{
		}
		~stTextInfo()
		{
			delete [] pText;
		}
		TextKey                keyId;
		const char*            pText;

		typedef TextKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const stTextInfo* rhs, const stTextInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool TextKey::operator <( const TextKey & obj ) const
	{
		if(typeId != obj.typeId)
			return typeId < obj.typeId;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, TextKey &row )
	{
		data >> row.typeId;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, stTextInfo &row )
	{
		data >> row.keyId;
		data >> row.pText;
		return data;
	}
}
class TextReader : public TableReader<Store::stTextInfo>, public Vek::Singleton<TextReader>
{
	friend class Vek::Singleton<TextReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("texts_cn_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData texts_cn_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	TextReader(){ InitializeX(); };
};

#endif