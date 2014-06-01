#ifndef __TABLE_READER_H__
#define __TABLE_READER_H__

#include "Stream.h"

static char		s_pData[READER_BUF_SIZE];


class FTableReaderBase
{
public:
	FTableReaderBase()
	{
		ms_Readers.push_back( this );
	}

	virtual ~FTableReaderBase(){}
	virtual bool		LoadData(const char *file) = 0;

	static void Reload()
	{
		for( int k = 0; k < (int)ms_Readers.size(); ++k )
		{
			FTableReaderBase* pReader = ms_Readers[k];
			pReader->LoadData( pReader->m_strFile.c_str() );
		}
	}
protected:
	string			m_strFile;
private:
	static vector<FTableReaderBase*>	ms_Readers;
};

//���ݶ�ȡģ��
template < class ITEM > 
class TableReader: public FTableReaderBase
{
public:
	typedef std::set<ITEM *, typename ITEM::KeyCompare> SET;		//��Ŀ����

	~TableReader();

	//����ID�����Ŀ
	const ITEM*		Item( const typename ITEM::KeyType &id ) const;

	const SET&		GetItemSet() const { return m_data; }

	void			GetAllItem(vector<ITEM*>& list);

protected:
	TableReader( const char *file );
	TableReader();	

	inline virtual bool		LoadData(const char *file);
	void			ClearData();
	void			ReadItem( Vek::Stream& stream, int& remainDataSize );
	int				ReadItem( Vek::Stream& stream );

protected:
	SET 			m_data;
	bool			m_loaded;

};

template < class ITEM > 
TableReader<ITEM>::TableReader()
	:m_loaded(false)
{
}

template < class ITEM > 
TableReader<ITEM>::~TableReader()
{	
	ClearData();
}

template < class ITEM > 
const ITEM * TableReader<ITEM>::Item( const typename ITEM::KeyType &id ) const
{
	static ITEM  cmpItem;
	cmpItem.SetID( id );
	typename SET::const_iterator result = m_data.find( &cmpItem );
	if( result != m_data.end() )
	{
		return (*result);
	}

	return NULL;
}

//template < class ITEM > 
//const TableReader<ITEM>::SET& TableReader<ITEM>::GetItemSet() const
//{
//	return m_data;
//}

template < class ITEM > 
void TableReader<ITEM>::GetAllItem(vector<ITEM*>& list)
{
	for(typename SET::iterator it=m_data.begin();it!=m_data.end();++it)
	{
		list.push_back(*it);
	}
}

template < class ITEM > 
void TableReader<ITEM>::ClearData()
{
	for(typename SET::iterator it=m_data.begin();it!=m_data.end();++it)
	{
		delete *it;
	}
	m_data.clear();
	m_loaded = false;
}

template < class ITEM >
int	TableReader<ITEM>::ReadItem( Vek::Stream& stream )
{
	ITEM* pItem = new ITEM;
	stream >> *pItem;

	if( m_data.find(pItem) == m_data.end() )
	{
		m_data.insert(pItem);
	}
	else
	{
		delete pItem;	
	}

	int remainDataSize = stream.getRemainingRData();
	return remainDataSize;
	//if( remainDataSize )
	//{
	//	stream.readData( s_pData, remainDataSize );
	//	stream.Clear();
	//	stream.writeData( s_pData, remainDataSize );
	//}
	//else
	//{
	//	stream.Clear();
	//}
}


template < class ITEM >
bool TableReader<ITEM>::LoadData( const char *filename )
{
	m_strFile = filename;
	ClearData();

	File file;
	if (!file.open(filename)) {
		LOGE( "��ȡ���:%sʧ�� !!", filename );
		return false;
	}

	unsigned long nSize = file.getLength();
	unsigned char* pBuffer = (unsigned char*)file.getBufferPtr();
	unsigned char* pTemp			= pBuffer;
	unsigned long nRemainDataSize	= nSize;

	uint itemsCount = 0;
	//����ͷ4���ֽڣ���ʾ�ܹ���item������itemsCount����
	memcpy( &itemsCount, pTemp, sizeof(uint) );
	//ǰ��
	pTemp			+= sizeof(uint);
	nRemainDataSize -= sizeof(uint);

	int leftSize = 0;
	Vek::Stream stream;
	while ( nRemainDataSize >= READER_BUF_SIZE )
	{
		//ÿ�ζ�ȡһ��READER_BUF_SIZE��С�����ݣ����赥����Ŀ���϶�С��READER_BUF_SIZE��
		stream.Clear();
		stream.writeData( pTemp, READER_BUF_SIZE );
		leftSize = ReadItem( stream );

		//ǰ��
		pTemp			+= ( READER_BUF_SIZE - leftSize );
		nRemainDataSize	-= ( READER_BUF_SIZE - leftSize );
	}

	leftSize = nRemainDataSize;
	stream.Clear();
	stream.writeData( pTemp, leftSize );
	while( leftSize > 0 )
	{
		leftSize = ReadItem( stream );
	}
	m_loaded = true;

	if (itemsCount != m_data.size()) {
		LOGE("��������һ��");
	}

	if (leftSize != 0) {
		LOGE("���������δ������");
	}

	//assert( itemsCount == m_data.size(), "open file: error" );
	//assert( leftSize == 0, "open file: error" );
	//LOGI( "read file:%s success, item count = %d", file, (int)itemsCount );

	return true;
}



#endif	// __TABLE_READER_H__