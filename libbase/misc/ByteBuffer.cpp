#include "BaseLib.h"
#include "ByteBuffer.h"
#include <assert.h>
#include <string.h>

namespace easygame {

	ByteBuffer::ByteBuffer()
	{
		uint capacity = 1024;
		init(capacity, capacity * 100);
	}

	ByteBuffer::ByteBuffer(uint capacity)
	{
		init(capacity, capacity * 100);
	}

	ByteBuffer::ByteBuffer(uint capacity, uint safeCapacity)
	{
		init(capacity, safeCapacity);
	}

	ByteBuffer::~ByteBuffer(void)
	{
		mCurSize = 0;
#ifdef USE_STL_BYTE
		m_buffer.clear();
#else
		SAFE_DELETE_ARRAY(m_buffer);
		mCapacity = 0;
		mSafeCapacity = 0;
#endif
	}

	void ByteBuffer::init(uint capacity, uint safeCapacity)
	{
		assert(capacity != 0);

#ifdef USE_STL_BYTE
		m_buffer.resize(capacity);
#else
		m_buffer = new byte[capacity];
		memset(m_buffer, 0, capacity);
#endif
		mCurSize = 0;
		mCapacity = capacity;
		mSafeCapacity = safeCapacity;
	}

	void ByteBuffer::resize(uint capacity)
	{
#ifdef THREAD_SAFE
		RWLockScopeWrite lock(mRWLock);
#endif

#ifdef USE_STL_BYTE
		m_buffer.clear();
		m_buffer.resize(capacity);
#else
		SAFE_DELETE_ARRAY(m_buffer);
		m_buffer = new byte[capacity];
		memset(m_buffer, 0, capacity);
#endif
		mCurSize = 0;
		mCapacity = capacity;
	}

	void ByteBuffer::addCapacity(uint capacity)
	{
#ifdef THREAD_SAFE
		RWLockScopeWrite lock(mRWLock);
#endif

		writeReserve(capacity);
	}

	size_t ByteBuffer::size()
	{
		return mCurSize;
	}

	void ByteBuffer::dump()
	{
#ifdef THREAD_SAFE
		RWLockScopeRead lock(mRWLock);
#endif

		printf("---------Begin Dump-----------\n");
		for (uint i=0; i<mCurSize; i++) {
			printf("%c", m_buffer[i]);	
		}
		printf("\n---------End Dump-----------\n");
	}

	void ByteBuffer::writeReserve(uint size)
	{
		// 需要的内存大小
		uint size2 = mCurSize + size;

#ifdef USE_STL_BYTE
		if (size2 >= m_buffer.size()) {
			m_buffer.resize(size2 * 2);
		}
#else
		if (mCurSize + size >= mCapacity) {
			mCapacity = size2 * 2;
			byte* pNewBuff = new byte[mCapacity];
			memset(pNewBuff, 0, mCapacity);
			if (mCurSize > 0) {
				memcpy(pNewBuff, m_buffer, mCurSize);
			}
			SAFE_DELETE_ARRAY(m_buffer);
			m_buffer = pNewBuff;
		}
#endif

	}

	bool ByteBuffer::write(const void* data, uint size)
	{
#ifdef THREAD_SAFE
		RWLockScopeWrite lock(mRWLock);
#endif

		if (mCurSize + size > mSafeCapacity) {
			printf("[error] !!!超过安全容量，写入失败\n");
			return false;
		}

		writeReserve(size);
		memcpy(&m_buffer[mCurSize],data,size);
		mCurSize += size;

		return true;
	}

	bool ByteBuffer::read(void* data, uint size)
	{
#ifdef THREAD_SAFE
		RWLockScopeWrite lock(mRWLock);
#endif

		if (mCurSize == 0)
			return false;

		if (size > mCurSize)
			return false;

		memcpy(data, &m_buffer[0], size);
		if (mCurSize != size) {
			memmove(&m_buffer[0], &m_buffer[size], mCurSize-size);
		}
		mCurSize -= size;

		return true;
	}

	bool ByteBuffer::remove(uint size)
	{
#ifdef THREAD_SAFE
		RWLockScopeWrite lock(mRWLock);
#endif

		if (mCurSize == 0)
			return false;

		if (size > mCurSize)
			return false;

		uint size2 = mCurSize - size;
		if (size2 > 0) {
			memmove(&m_buffer[0], &m_buffer[size], size2);
		}

		mCurSize -= size;
		return true;
	}

	bool ByteBuffer::get(void* data, uint size, uint offset)
	{
#ifdef THREAD_SAFE
		RWLockScopeRead lock(mRWLock);
#endif

		if (mCurSize == 0)
			return false;

		if (offset + size > mCurSize)
			return false;

		memcpy(data,&m_buffer[offset], size);

		return true;
	}

	const byte* ByteBuffer::getBuffer()
	{
#ifdef THREAD_SAFE
		RWLockScopeRead lock(mRWLock);
#endif

		return m_buffer;
	}

	void ByteBuffer::clearBuffer()
	{
#ifdef THREAD_SAFE
		RWLockScopeWrite lock(mRWLock);
#endif

		mCurSize = 0;
	}


	byte* ByteBuffer::getWriteBufferPtr()
	{
		return &m_buffer[mCurSize];
	}


	size_t ByteBuffer::getFreeSize()
	{
		return mCapacity - mCurSize;
	}

	bool ByteBuffer::offsetCurPos(uint size)
	{
		if (mCurSize + size > mSafeCapacity) {
			printf("[error] !!!超过安全容量，移动游标失败\n");
			return false;
		}

		mCurSize += size;

		return true;
	}


}	// namespace



