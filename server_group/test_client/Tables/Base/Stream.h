
#ifndef __Stream_H__
#define __Stream_H__

const unsigned int READER_BUF_SIZE		=	1024 * 16;

namespace Vek
{
	//数据流类
	class Stream
	{
	public:
		Stream();

		//!清空数据
		void Clear();

		// 获得剩余数据大小
		int getRemainingRData();

		//!获得消息里逻缉数据长度
		int getSize() const;

		// 读字符串
		void	readString(	char *buffer, int size );

		//读一定长度的数据
		void	readData( void * pData, int n );

		//写入一定长度数据
		void	writeData( const void * pData, int n );

		template<typename ValueType>
		Stream & operator >> (ValueType & val);

		Stream & operator >> (std::string &val);
		Stream & operator >> (std::wstring &val);
		Stream & operator >> ( const char*& val );

	private:
		uint m_nSize;					// 长度
		byte * m_pRead;					//读取位置
		byte * m_pWrite;				//写入位置
		byte m_pBuf[READER_BUF_SIZE];	//数据缓存
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
