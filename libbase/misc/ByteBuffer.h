#ifndef __BYTE_BUFFER_H__
#define __BYTE_BUFFER_H__

#include "BaseTypes.h"

namespace easygame {

	//#define USE_STL_BYTE	// ʹ��stl
	//#define THREAD_SAFE

	// capacity - ��ʼ����
	// safecapacity - ��ȫ����(������ȫ����ʱ���޷�д������)
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

		// �ӻ����ĩβд������(�������ݻ�����)
		bool write(const void* data, uint size);

		// �ӻ����ͷ����ȡ����(�������ݻ����)
		bool read(void* data, uint size);

		// �ӻ���ͷ����ʼ���Ƴ�ָ����С������(�������ݻ����)
		bool remove(uint size);

		// ��ָ��λ��ȡָ����С�����������ݣ������ı仺������
		bool get(void* data, uint size, uint offset = 0);

		// ��û����׵�ַ
		const byte* getBuffer();

		// �������
		void clearBuffer(); 

		// ȡ�û������Ĵ�С(�����ݵ��ֽ���)
		size_t size();

		// ���·����ڴ�(ע�⣺���д��ڵ����ݶ�������������ã�)
		void resize(uint capacity);

		// ��������,֮ǰ�����ݲ��ᱻ���
		void addCapacity(uint capacity);

		// ----------------- �����÷�(������ʹ��) ---------------------
		// ��û��浱ǰ��д��ĵ�ַ
		byte* getWriteBufferPtr();

		// ��û�������ʣ��Ĵ�С����������-��ǰռ������
		size_t getFreeSize();

		// ƫ�Ƶ�ǰλ��
		bool offsetCurPos(uint size);
		// -------------------------------------------------------------

	protected:
		void init(uint capacity, uint safeCapacity);
		// Ԥ�ȼ��㻺���С���������������ڴ�
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


