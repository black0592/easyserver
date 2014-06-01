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

		// 连接服务器
		virtual bool connect(const char* ip, ushort port);

		// ping服务器
		virtual bool ping();

		// 断开与服务器的链接
		virtual bool close();

		// 关闭服务
		virtual void shutoff();

		// 重新连接
		virtual bool reconnect();

		//判断是否和服务器连接
		virtual bool isConnected();

		// 是否正在连接中
		//virtual bool IsWaitConnect();

		void deleteMe() { mIsDeleteMe = true; }
		bool isCanDeleteMe() { return mIsDeleteMe; }


		// 发送数据(任意大小的数据)
		virtual bool sendRawData(const void* data, int len);

		// 发送消息(进行组包操作)
		virtual bool sendCmd(const void* cmd, ushort cmdLen);

		const char* getIP();
		ushort getPort();

	public:
		// 打印运行信息
		virtual void printInfo();

	protected:
		virtual ~SocketClient();

		// 消息具体的处理函数,上层逻辑使用
		//virtual bool handleMessage(const void *cmd, int cmdLen) { return true; }

	protected:
		bool recvThreadFunc();
		bool sendThreadFunc();

		//void setBlock(bool block);
		int send(const char * buff, int len, bool writesize);
		//int recv(char * buff, int len);

		// 解析收到的数据(拆包)
		void parsePacket();

		// 初始化加密参数
		void resetEncrypt();
		// 加密
		void Encrypt(byte* data, uint dataLen);
		// 解密
		void Decrypt(byte* data, uint dataLen);
		// 更新到下一个密钥
		void nextEncryptKey(uint& index);

		//// 连接建立时被调动
		virtual void OnConnect() {}

		//// 连接断开时被调用
		virtual void OnDisconnect() {}

		// 当有数据被接收时调用，接收的实际数据长度为len
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

		// 是否删除
		bool mIsDeleteMe;

		Mutex mSendMutex;
		ByteBuffer mSendBuff;

		ByteBuffer mRecvBuff;

		// 是否开启加密啊
		bool mEnableEncrypt;
		// 加密下标
		uint mEncIndex;
		// 解密下标
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


	// 客户端链接采用线程池
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

		// 加入等待队列
		bool addWaitList(SocketClient* pClient);

		// 处理新链接
		void processAddClient();

	protected:
		TCPClientMap mClientMap;
		Cond mCond;
		Mutex mMutex;

		// 链接管理器，增加一个队列，减少线程锁
		Mutex mWaitAddMutex;
		list<SocketClient*> mWaitAddList;
	};

	//// 处理发送的线程
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

	//// 处理接收的线程
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

	// 客户端链接采用线程池
	class TCPClientThreadPool : public Singleton<TCPClientThreadPool>
	{
	public:
		TCPClientThreadPool();
		virtual ~TCPClientThreadPool();

		bool addTCPClient(SocketClient* client);
		//bool removeTCPClient(SocketClient* client);

		// 停止所有线程
		void stopAllThread();

	protected:
		//Mutex mMutex;	// 统一管理
		// 处理发送的线程池
		//std::list<TCPClientSendThread*> mClientSendThreads;
		// 处理接收的线程池
		//std::list<TCPClientRecvThread*> mClientRecvThreads;

		// 发送和接收
		std::list<TCPClientThread*> mClientThreads; 

	private:
	};



}	// namespace


#endif
