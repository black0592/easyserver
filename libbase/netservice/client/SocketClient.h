#ifndef __SOCKET_CLIENT_H__
#define __SOCKET_CLIENT_H__


#include "TCPClient.h"
#include <stdio.h>  
#include <map>
#include <vector>
#include <list>
#include "netservice/xsocket/xsocket.h"
#include "netservice/NetPacket.h"
#include "misc/ByteBuffer.h"
#include "misc/ObjectPool.h"
using namespace xsocket;

namespace easygame {

	class SocketClient : public TCPClient
	{
		friend class TCPClientThread;

	public:
		SocketClient(const string& name);

		const char* getName();

		// ���ӷ�����
		virtual bool connect(const char* ip, ushort port);

		// ping������
		virtual bool ping();

		// �Ͽ��������������
		virtual bool close();

		// �رշ���
		virtual void shutoff();

		// ��������
		virtual bool reconnect();

		//�ж��Ƿ�ͷ���������
		virtual bool isConnected();

		// �Ƿ�����������
		//virtual bool IsWaitConnect();

		void deleteMe() { mIsDeleteMe = true; }
		bool isCanDeleteMe() { return mIsDeleteMe; }


		// ��������(�����С������)
		virtual bool sendRawData(const void* data, int len);

		// ������Ϣ(�����������)
		virtual bool sendCmd(const void* cmd, ushort cmdLen);

		const char* getIP();
		ushort getPort();

	public:
		// ��ӡ������Ϣ
		virtual void printInfo();

	protected:
		virtual ~SocketClient();

		// ��Ϣ����Ĵ�����,�ϲ��߼�ʹ��
		//virtual bool handleMessage(const void *cmd, int cmdLen) { return true; }

	protected:
		bool recvThreadFunc();
		bool sendThreadFunc();

		//void setBlock(bool block);
		int send(const char * buff, int len, bool writesize);
		//int recv(char * buff, int len);

		// �����յ�������(���)
		void parsePacket();

		// ��ʼ�����ܲ���
		void resetEncrypt();
		// ����
		void Encrypt(byte* data, uint dataLen);
		// ����
		void Decrypt(byte* data, uint dataLen);
		// ���µ���һ����Կ
		void nextEncryptKey(uint& index);

		//// ���ӽ���ʱ������
		virtual void OnConnect() {}

		//// ���ӶϿ�ʱ������
		virtual void OnDisconnect() {}

		// �������ݱ�����ʱ���ã����յ�ʵ�����ݳ���Ϊlen
		//virtual int OnRecv(int len);

	public:
		//void Send();
		//void recvThread();

	//protected:
	//	static DWORD WINAPI SendThread(void* param);
	//	static DWORD WINAPI RecvThread(void* param);

	protected:
		//volatile bool mIsRunning;
		//bool mIsConnected;
		//string mIP;
		//ushort mPort;
		string mName;
		Socket mSocket;

		// �Ƿ�ɾ��
		bool mIsDeleteMe;

		Mutex mSendMutex;
		ByteBuffer mSendBuff;

		ByteBuffer mRecvBuff;

		// �Ƿ������ܰ�
		bool mEnableEncrypt;
		// �����±�
		uint mEncIndex;
		// �����±�
		uint mDecIndex;

		//HANDLE 	m_threadSend;
		//HANDLE	m_threadRecv;

		//Mutex m_sendLock;
		//ByteBuffer m_sendBuff;

		//SyncLock m_recvLock;
		//ByteBuffer m_recvBuff;

		//Mutex m_cmdQueLock;
		//queue<ByteBuffer*> m_cmdQueue;
	};


	//////////////////////////////////////////////////////////////////////////


	// �ͻ������Ӳ����̳߳�
	class TCPClientThread : public Thread
	{
		friend class TCPClientThreadPool;

	public:
		typedef std::map<SocketClient*,SocketClient*>	TCPClientMap;
		typedef TCPClientMap::iterator					TCPClientMapIter;

	protected:
		TCPClientThread();
		virtual ~TCPClientThread();
	
		bool isFull();
		//bool addTCPClient(SocketClient* client);
		//bool removeTCPClient(SocketClient* client);

		virtual bool run();

		// ����ȴ�����
		bool addWaitList(SocketClient* pClient);

		// ����������
		void processAddClient();

	protected:
		TCPClientMap mClientMap;
		Cond mCond;
		Mutex mMutex;

		// ���ӹ�����������һ�����У������߳���
		Mutex mWaitAddMutex;
		list<SocketClient*> mWaitAddList;
	};

	//// �����͵��߳�
	//class TCPClientSendThread : public TCPClientThread
	//{
	//public:
	//	friend class TCPClientThreadPool;

	//protected:
	//	TCPClientSendThread();
	//	virtual ~TCPClientSendThread();
	//
	//	virtual bool run();
	//};

	//// ������յ��߳�
	//class TCPClientRecvThread : public TCPClientThread
	//{
	//public:
	//	friend class TCPClientThreadPool;

	//protected:
	//	TCPClientRecvThread();
	//	virtual ~TCPClientRecvThread();

	//	virtual bool run();
	//};


	//////////////////////////////////////////////////////////////////////////

	// �ͻ������Ӳ����̳߳�
	class TCPClientThreadPool : public Singleton<TCPClientThreadPool>
	{
	public:
		TCPClientThreadPool();
		virtual ~TCPClientThreadPool();

		bool addTCPClient(SocketClient* client);
		//bool removeTCPClient(SocketClient* client);

		// ֹͣ�����߳�
		void stopAllThread();

	protected:
		//Mutex mMutex;	// ͳһ����
		// �����͵��̳߳�
		//std::list<TCPClientSendThread*> mClientSendThreads;
		// ������յ��̳߳�
		//std::list<TCPClientRecvThread*> mClientRecvThreads;

		// ���ͺͽ���
		std::list<TCPClientThread*> mClientThreads; 

	private:
	};



}	// namespace


#endif
