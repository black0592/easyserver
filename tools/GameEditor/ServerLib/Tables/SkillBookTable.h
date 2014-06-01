
#ifndef __SKILLBOOK_TABLE_READER_H__
#define __SKILLBOOK_TABLE_READER_H__

#include "TableHelp.h"

namespace Store
{
	struct SkillBookKey
	{
		SkillBookKey( int _bookId = 0 )
			:bookId(_bookId)
		{}
		int                    bookId;
		bool operator < ( const SkillBookKey &obj ) const;
	};
	struct SkillBookInfo
	{
		SkillBookInfo()
		{
		}
		~SkillBookInfo()
		{
		}
		SkillBookKey           keyId;
		int                    iSkillId;
		int                    iSkillLevel;

		typedef SkillBookKey KeyType;
		struct KeyCompare
		{
			const bool operator()(const SkillBookInfo* rhs, const SkillBookInfo* lhs) const
			{
				return rhs->keyId < lhs->keyId;
			}
		};
		void SetID(const KeyType& InId){ keyId = InId; }
	};
	inline bool SkillBookKey::operator <( const SkillBookKey & obj ) const
	{
		if(bookId != obj.bookId)
			return bookId < obj.bookId;
		return false;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, SkillBookKey &row )
	{
		data >> row.bookId;
		return data;
	}
	inline Vek::Stream & operator >> ( Vek::Stream &data, SkillBookInfo &row )
	{
		data >> row.keyId;
		data >> row.iSkillId;
		data >> row.iSkillLevel;
		return data;
	}
}
class SkillBookReader : public TableReader<Store::SkillBookInfo>, public Vek::Singleton<SkillBookReader>
{
	friend class Vek::Singleton<SkillBookReader>;
public:
	int InitializeX()
	{
		if( !LoadData( Store::GetStorePath("skill_book_info_s.dat").c_str() ) )
		{
			assert(false && "LoadData skill_book_info_s.dat, fail!");
			return 1;
		}
		return 0;
	}
private:
	SkillBookReader(){ InitializeX(); };
};

#endif