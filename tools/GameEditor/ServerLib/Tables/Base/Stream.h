
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

#if 0
		Stream & operator >> (bool & val);
		Stream & operator >> (char & val);
		Stream & operator >> (unsigned char & val);
		Stream & operator >> (short & val);
		Stream & operator >> (unsigned short & val);
		Stream & operator >> (int & val);
		Stream & operator >> (unsigned int & val);
		Stream & operator >> (long & val);
		Stream & operator >> (unsigned long & val);
		Stream & operator >> (long long & val);
		Stream & operator >> (unsigned long long & val);
		Stream & operator >> (float & val);
		Stream & operator >> (double & val);
		Stream & operator >> (std::wstring &val);
#else
		template<typename ValueType>
		Stream & operator >> (ValueType & val);

		Stream & operator >> (std::string &val);
		Stream & operator >> (std::wstring &val);
		Stream & operator >> ( const char*& val );
#endif
	private:
		WORD m_nSize;					// ����
		BYTE * m_pRead;					//��ȡλ��
		BYTE * m_pWrite;				//д��λ��
		BYTE m_pBuf[READER_BUF_SIZE];	//���ݻ���
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

#if 0
	inline Stream & Stream::operator >> (bool & val)
	{
		readData(&val,1);
		return *this;
	}

	inline Stream & Stream::operator >> (char & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline Stream & Stream::operator >> (unsigned char & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline Stream & Stream::operator >> (short & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline Stream & Stream::operator >> (unsigned short & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline Stream & Stream::operator >> (int & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline Stream & Stream::operator >> (unsigned int & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline Stream & Stream::operator >> (long & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline Stream & Stream::operator >> (unsigned long & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline Stream & Stream::operator >> (long long & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline Stream & Stream::operator >> (unsigned long long & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline Stream & Stream::operator >> (float & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline Stream & Stream::operator >> (double & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}
	inline Stream & Stream::operator >> ( std::wstring &val )
	{
		unsigned short usLen = -1;
		*this >> usLen;
		if ( usLen != -1)
		{
			val.assign( (wchar_t*)m_pRead, usLen );
			m_pRead += usLen * sizeof(wchar_t);
		}
		else
		{
			val = L"(null)";
		}
		return * this;
	}
#else
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
			m_pRead += usLen * sizeof(wchar_t);
		}
		else
		{
			val = L"(null)";
		}
		return * this;
	}
#endif

}



#endif // !1__Stream_H__
