#ifndef __TCPTASK_H__
#define __TCPTASK_H__

#include "./lance/tcpsrv.hpp"
#include "BaseTypes.h"
#include "NetPacket.h"
#include "misc/ByteBuffer.h"
#include "MsgProcessor.h"
#include "thread/Mutex.h"

namespace easygame {
	
	enum TCPTaskType
	{
		AsyncType,		// �첽���ͣ����̷߳�������
		SyncType,		// ͬ������/��Ϣ����(���̷߳���)
	};

	//enum TCPTaskState
	//{
	//	verify,		// �ս�������,�ȴ���֤
	//	okay,		// �����ȶ�����
	//	recycle,	// ��������
	//};


	// Ĭ�����ݰ���ʽ
	// ushort | BIN
	// Size | Datas
	// ǰ�����ֽڴ����С��������ָ�����ȵ�����
	class TCPTask : public lance::net::Client 
	{
	public:
		TCPTask();
		virtual ~TCPTask(void);

		// ȡ�÷����ID��
		int getID();

		// ����id
		void setID(int id);

		// �������IP��ַ
		int getDigitalIP();

		// ����ַ�IP��ַ
		const char* getIP();

		TCPTaskType getType() { return mType; }

		// �������ӵȴ���
		virtual void addConnectWaitList();

		// ������һ�ν��յ���Ϣ��ʱ��
		int64 getLastRecvTime() { return mLastRecvTime; }

		// ��¼����task��ʱ��
		void setLastDestroyTime(int64 time) { mLastDestroyTime = time; }
		int64 getLastDestroyTime() { return mLastDestroyTime; }

		// ��������(������ͣ����� ǰ�����ֽڱ�ʾ���ȣ����������)
		// �ȼ��ܣ���ѹ��
		virtual bool sendCmd(const void* cmd, ushort cmdLen);

		// ����ԭʼ���ݣ�����Ҫ���д���
		virtual bool sendRawData(const void* data, ushort dataLen);

		virtual bool isInBlacklist();

		//////////////////////////////////////////////////////////////////////////
		// ���ӽ���ʱ������
		virtual bool OnConnect();

		// ���ӶϿ�ʱ������
		virtual void OnDisconnect();

		// �������ݱ�����ʱ���ã����յ�ʵ�����ݳ���Ϊlen
		virtual int OnRecv(const void* data, int len);
		//////////////////////////////////////////////////////////////////////////

		// ����ѭ���н��и��´���
		virtual void onUpdate();

		//bool sendThreadFunc();

		//int getSendBuffSize();

		//// ��������
		//virtual void recycleTask();

		//// �ܷ��������
		//virtual bool canRecycle();

	public:
		// �ر���Ϣ����(ϵͳ�ײ�ʹ�ã���㲻Ҫ���ô˺���)

		// ������һ��״̬
		//void enterNextState();

	protected:
		// �����յ�������(���)
		void parsePacket();

		// ����
		void Encrypt(byte* data, uint dataLen);
		// ����
		void Decrypt(byte* data, uint dataLen);
		// ���µ���һ����Կ
		void nextEncryptKey(uint& index);

	private:
		std::string mIP;
		//TCPTaskState mState;
		//uint mRecycleTick;

	protected:
		// ΨһID��Ψһ��ʶ
		int mId;

		// ����
		TCPTaskType mType;

		ByteBuffer mRecvBuff;

		//// ���ջ���(Ĭ��64K)
		//stRecvBuff mRecvBuff;

		//// ���ͻ���
		//Mutex mSendMutex;
		//ByteBuffer mSendBuff;

		// �Ƿ���ѹ��
		bool mEnableZip;
		// �Ƿ������ܰ�
		bool mEnableEncrypt;
		// �����±�
		uint mEncIndex;
		// �����±�
		uint mDecIndex;

		// ���һ���յ�����ʱ��
		int64 mLastRecvTime;
		// DDS��������
		uint mDDSCheckCount;

		// ���һ��Ҫ���ٵ�ʱ��
		int64 mLastDestroyTime;
	};

	//////////////////////////////////////////////////////////////////////////

	// --- ͬ��/�첽������Ϣ��TCPTask -----
	class TCPTaskImpl : public TCPTask, public MessageHandlerQueue
	{
	public:
		TCPTaskImpl(TCPTaskType type) { mType = type; }
		// �ײ�������õĽӿ�(���߳�)
		virtual bool OnRecvCommand(const void *cmd, int cmdLen)
		{
			if (mType == SyncType) {
				// ͬ����ʽ
				return putMsg(cmd, cmdLen);
			}

			// �첽��ʽ
			return handleMessage(cmd, cmdLen); 
		};

	protected:
	private:
	};

	////////////////////////////////////////////////////////////////////////////

	//// --- �첽������Ϣ��TCPTask -----
	//class TCPTaskAsync : public TCPTask
	//{
	//public:
	//	TCPTaskAsync() { mType = AsyncType; }
	//	// �ײ�������õĽӿ�(���߳�)
	//	virtual bool OnRecvCommand(const void *cmd, int cmdLen) { return handleMessage(cmd, cmdLen); };
	//	// ��Ϣ����Ĵ�����,�ϲ��߼�ʹ��(���߳�)
	//	//virtual bool handleMessage(const void *cmd, int cmdLen) { return true; }
	//protected:
	//private:
	//};

	////////////////////////////////////////////////////////////////////////////

	//// --- ������Ϣ���з�ʽ��������Ϣ -----
	//class TCPTaskSync : public TCPTask, public MessageHandlerQueue
	//{
	//public:
	//	TCPTaskSync() { mType = SyncType; }
	//	// �ײ�������õĽӿ�(���߳�)
	//	virtual bool OnRecvCommand(const void *cmd, int cmdLen) { return putMsg(cmd, cmdLen); };

	//	// ����ӿ���MessageHandlerQueue�ж���
	//	//virtual bool handleMessage(const void *cmd, int cmdLen) { return true; }
	//protected:
	//private:
	//};


}	// namespace

#endif
