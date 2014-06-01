#include "ClientHeader.h"
#include "Stream.h"

using namespace Vek;

Stream::Stream()
{
	Clear();
}

void Stream::readData(void * pData, int n)
{
	if( m_pRead + n > m_pBuf + READER_BUF_SIZE)
	{
		return;
	}
	if( m_pRead + n > m_pBuf + getSize())
	{
		return;
	}

	memcpy(pData, m_pRead, n);
	m_pRead += n;
}

void Stream::writeData(const void * pData, int n)
{
	if( m_pWrite + n > m_pBuf + READER_BUF_SIZE )
	{
		return;
	}

	memcpy(m_pWrite, pData, n);
	m_pWrite += n;
	m_nSize += n;
}






