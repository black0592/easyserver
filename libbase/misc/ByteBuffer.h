#ifndef __BYTE_BUFFER_H__
#define __BYTE_BUFFER_H__

#include "BaseTypes.h"

namespace easygame {

	//#define USE_STL_BYTE	// 使用stl
	//#define THREAD_SAFE

	// capacity - 初始容量
	// safecapacity - 安全容量(超过安全容量时，无法写入数据)
	class ByteBuffer
	{
	private:
		ByteBuffer();

	public:
		ByteBuffer(uint capacity);
		ByteBuffer(uint capacity, uint safeCapacity);
		virtual ~ByteBuffer(void);

		// debug function
		void dump();

		// 从缓存的末尾写入数据(缓存数据会增加)
		bool write(const void* data, uint size);

		// 从缓存的头部读取数据(缓存数据会减少)
		bool read(void* data, uint size);

		// 从缓存头部开始，移除指定大小的数据(缓存数据会减少)
		bool remove(uint size);

		// 从指定位置取指定大小的连续的数据，但不改变缓存数据
		bool get(void* data, uint size, uint offset = 0);

		// 获得缓存首地址
		const byte* getBuffer();

		// 清除缓存
		void clearBuffer(); 

		// 取得缓存区的大小(即数据的字节数)
		size_t size();

		// 重新分配内存(注意：所有存在的数据都将被清除，慎用！)
		void resize(uint capacity);

		// 增加容量,之前的数据不会被清除
		void addCapacity(uint capacity);

		// ----------------- 特殊用法(不建议使用) ---------------------
		// 获得缓存当前可写入的地址
		byte* getWriteBufferPtr();

		// 获得缓存区，剩余的大小，即总容量-当前占用容量
		size_t getFreeSize();

		// 偏移当前位置
		bool offsetCurPos(uint size);
		// -------------------------------------------------------------

	protected:
		void init(uint capacity, uint safeCapacity);
		// 预先计算缓存大小，不够就申请新内存
		void writeReserve(uint size);

	private:
#ifdef USE_STL_BYTE
		vector<byte> m_buffer;
#else
		byte* m_buffer;
#endif
		volatile uint mCurSize;
		volatile uint mCapacity;
		volatile uint mSafeCapacity;
#ifdef THREAD_SAFE
		RWLock mRWLock;
#endif
	};


}	// namespace

#endif


