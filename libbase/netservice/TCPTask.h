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
		AsyncType,		// 异步类型（多线程服务网）
		SyncType,		// 同步类型/消息队列(单线程服务)
	};

	//enum TCPTaskState
	//{
	//	verify,		// 刚建立连接,等待验证
	//	okay,		// 建立稳定连接
	//	recycle,	// 回收连接
	//};


	// 默认数据包格式
	// ushort | BIN
	// Size | Datas
	// 前两个字节代表大小，后面是指定长度的数据
	class TCPTask : public lance::net::Client 
	{
	public:
		TCPTask();
		virtual ~TCPTask(void);

		// 取得分配的ID号
		int getID();

		// 设置id
		void setID(int id);

		// 获得数字IP地址
		int getDigitalIP();

		// 获得字符IP地址
		const char* getIP();

		TCPTaskType getType() { return mType; }

		// 加入链接等待池
		virtual void addConnectWaitList();

		// 获得最后一次接收到消息的时刻
		int64 getLastRecvTime() { return mLastRecvTime; }

		// 记录销毁task的时间
		void setLastDestroyTime(int64 time) { mLastDestroyTime = time; }
		int64 getLastDestroyTime() { return mLastDestroyTime; }

		// 发送数据(处理后发送，处理： 前两个字节表示长度，后面跟数据)
		// 先加密，后压缩
		virtual bool sendCmd(const void* cmd, ushort cmdLen);

		// 发送原始数据，不需要进行处理
		virtual bool sendRawData(const void* data, ushort dataLen);

		virtual bool isInBlacklist();

		//////////////////////////////////////////////////////////////////////////
		// 连接建立时被调动
		virtual bool OnConnect();

		// 连接断开时被调用
		virtual void OnDisconnect();

		// 当有数据被接收时调用，接收的实际数据长度为len
		virtual int OnRecv(const void* data, int len);
		//////////////////////////////////////////////////////////////////////////

		// 在主循环中进行更新处理
		virtual void onUpdate();

		//bool sendThreadFunc();

		//int getSendBuffSize();

		//// 回收连接
		//virtual void recycleTask();

		//// 能否回收连接
		//virtual bool canRecycle();

	public:
		// 关闭消息发送(系统底层使用，外层不要调用此函数)

		// 进入下一个状态
		//void enterNextState();

	protected:
		// 解析收到的数据(拆包)
		void parsePacket();

		// 加密
		void Encrypt(byte* data, uint dataLen);
		// 解密
		void Decrypt(byte* data, uint dataLen);
		// 更新到下一个密钥
		void nextEncryptKey(uint& index);

	private:
		std::string mIP;
		//TCPTaskState mState;
		//uint mRecycleTick;

	protected:
		// 唯一ID，唯一标识
		int mId;

		// 类型
		TCPTaskType mType;

		ByteBuffer mRecvBuff;

		//// 接收缓存(默认64K)
		//stRecvBuff mRecvBuff;

		//// 发送缓存
		//Mutex mSendMutex;
		//ByteBuffer mSendBuff;

		// 是否开启压缩
		bool mEnableZip;
		// 是否开启加密啊
		bool mEnableEncrypt;
		// 加密下标
		uint mEncIndex;
		// 解密下标
		uint mDecIndex;

		// 最后一次收到包的时间
		int64 mLastRecvTime;
		// DDS攻击次数
		uint mDDSCheckCount;

		// 最后一次要销毁的时间
		int64 mLastDestroyTime;
	};

	//////////////////////////////////////////////////////////////////////////

	// --- 同步/异步处理消息的TCPTask -----
	class TCPTaskImpl : public TCPTask, public MessageHandlerQueue
	{
	public:
		TCPTaskImpl(TCPTaskType type) { mType = type; }
		// 底层拆包后调用的接口(多线程)
		virtual bool OnRecvCommand(const void *cmd, int cmdLen)
		{
			if (mType == SyncType) {
				// 同步方式
				return putMsg(cmd, cmdLen);
			}

			// 异步方式
			return handleMessage(cmd, cmdLen); 
		};

	protected:
	private:
	};

	////////////////////////////////////////////////////////////////////////////

	//// --- 异步处理消息的TCPTask -----
	//class TCPTaskAsync : public TCPTask
	//{
	//public:
	//	TCPTaskAsync() { mType = AsyncType; }
	//	// 底层拆包后调用的接口(多线程)
	//	virtual bool OnRecvCommand(const void *cmd, int cmdLen) { return handleMessage(cmd, cmdLen); };
	//	// 消息具体的处理函数,上层逻辑使用(多线程)
	//	//virtual bool handleMessage(const void *cmd, int cmdLen) { return true; }
	//protected:
	//private:
	//};

	////////////////////////////////////////////////////////////////////////////

	//// --- 采用消息队列方式来处理消息 -----
	//class TCPTaskSync : public TCPTask, public MessageHandlerQueue
	//{
	//public:
	//	TCPTaskSync() { mType = SyncType; }
	//	// 底层拆包后调用的接口(多线程)
	//	virtual bool OnRecvCommand(const void *cmd, int cmdLen) { return putMsg(cmd, cmdLen); };

	//	// 这个接口在MessageHandlerQueue中定义
	//	//virtual bool handleMessage(const void *cmd, int cmdLen) { return true; }
	//protected:
	//private:
	//};


}	// namespace

#endif
