
#ifndef __Stream_H__
#define __Stream_H__

const unsigned int READER_BUF_SIZE		=	1024 * 16;

namespace Vek
{
	//��������
	class Stream
	{
	public:
		Stream();

		//!�������
		void Clear();

		// ���ʣ�����ݴ�С
		int getRemainingRData();

		//!�����Ϣ���߼����ݳ���
		int getSize() const;

		// ���ַ���
		void	readString(	char *buffer, int size );

		//��һ�����ȵ�����
		void	readData( void * pData, int n );

		//д��һ����������
		void	writeData( const void * pData, int n );

		template<typename ValueType>
		Stream & operator >> (ValueType & val);

		Stream & operator >> (std::string &val);
		Stream & operator >> (std::wstring &val);
		Stream & operator >> ( const char*& val );

	private:
		uint m_nSize;					// ����
		byte * m_pRead;					//��ȡλ��
		byte * m_pWrite;				//д��λ��
		byte m_pBuf[READER_BUF_SIZE];	//���ݻ���
	};

	inline void Stream::Clear()
	{
		m_nSize		= 0;
		m_pRead		= m_pWrite = &m_pBuf[0];
	}

	inline int Stream::getRemainingRData()
	{
		if ( m_pRead < m_pBuf + getSize() )
		{
			return static_cast<int>( m_pBuf + getSize() - m_pRead );
		}
		return 0;
	}
	
	inline int Stream::getSize() const
	{
		return m_nSize;
	}


	template<typename ValueType>
	inline Stream & Stream::operator >> (ValueType & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}
	inline Stream & Stream::operator >> ( const char*& val )
	{
		unsigned short usLen = -1;
		*this >> usLen;
		if ( usLen > 0)
		{
			val = new char[usLen+1];
			memcpy( (void*)val, m_pRead, usLen );
			const_cast<char*>(val)[usLen] = 0;

			m_pRead += usLen * sizeof(char);
		}
		else
		{
			val = NULL;
		}
		return * this;
	}
	inline Stream & Stream::operator >> ( std::string &val )
	{
		unsigned short usLen = -1;
		*this >> usLen;
		if ( usLen != -1)
		{
			val.assign( (char*)m_pRead, usLen );
			m_pRead += usLen * sizeof(char);
		}
		else
		{
			val = "(null)";
		}
		return * this;
	}
	inline Stream & Stream::operator >> ( std::wstring &val )
	{
		unsigned short usLen = -1;
		*this >> usLen;
		if ( usLen != -1)
		{
			val.assign( (wchar_t*)m_pRead, usLen );
			m_pRead += usLen * 2;
		}
		else
		{
			val = L"(null)";
		}
		return * this;
	}

}



#endif // !1__Stream_H__
