#pragma once
#include <BaseDefine.h>
#include <BaseMisc.h>
#include <misc/ByteBuffer.h>
#include <misc/ObjectPool.h>
#include <misc/Singleton.h>
using namespace easygame;

#define EV_INVALID		0
#define EV_CONNECT		1
#define EV_DISCONNECT	2
#define EV_RECV			3
#define EV_SEND			4
#define EV_EXIT			5

#ifndef WIN32
struct WSABUF 
{
	ulong len;	// the length of the buffer
	void *buf;
};
#endif

// io事件基类
#ifdef WIN32
struct IoEventBase : public WSAOVERLAPPED
#else
struct IoEventBase
#endif
{
	void* clt;
	int	event;

	IoEventBase()
	{
		clt = NULL;
		event = EV_INVALID;
	}
};

// io连接事件
struct IoConnectEvent : public IoEventBase
{
	IoConnectEvent()
	{
		event = EV_CONNECT;
	}
};

// io断开连接事件
struct IoDisconnectEvent : public IoEventBase
{
	IoDisconnectEvent()
	{
		event = EV_DISCONNECT;
	}
};

struct IoRecvEvent : public IoEventBase
{
	WSABUF	wsaBuf;			// 发送时用
	char buffer[MAX_USER_RECV_SIZE];	// 接收/发送缓存

	IoRecvEvent()
	{
		init();
	}

	void init()
	{
		memset(this, 0, sizeof(IoEventBase));
		wsaBuf.buf=buffer;
		wsaBuf.len=MAX_USER_RECV_SIZE;

		event = EV_RECV;
	}
};

struct IoSendEvent : public IoEventBase
{
	uint dwSentBytes;		// 已经发送的字节数
	WSABUF	wsaBuf;			// 发送时用
	ByteBuffer buffer;

	IoSendEvent()
		:buffer(MAX_USER_SEND_SIZE)
	{
		init();
	}

	void init()
	{
		memset(this, 0, sizeof(IoEventBase));
		buffer.clearBuffer();
		wsaBuf.buf = (char*)buffer.getBuffer();
		wsaBuf.len = 0;
		dwSentBytes = 0;

		event = EV_SEND;
	}

	bool put(const void* data, int len)
	{
		if ( !buffer.write(data, len) )
			return false;

		wsaBuf.buf = (char*)buffer.getBuffer();
		wsaBuf.len = (ulong)buffer.size();
		return true;
	}
};


class IoEventPool : public Singleton<IoEventPool>
{
public:
	IoEventPool()
		: mConnEventCache("ConnEvent", 99999999)
		, mDisconnEventCache("DisconnEvent", 99999999)
		, mRecvEventCache("RecvEvent", 99999999)
		, mSendEventCache("SendEvent", 99999999)
	{

	}
	virtual ~IoEventPool(){}

public:

	// 链接事件
	IoConnectEvent* getConnectEvent()
	{
		return mConnEventCache.newObject();
	}
	void putConnectEvent(IoConnectEvent* pEvent)
	{
		mConnEventCache.freeObject(pEvent);
	}

	// 断开链接事件
	IoDisconnectEvent* getDisconnectEvent()
	{
		return mDisconnEventCache.newObject();
	}
	void putDisconnectEvent(IoDisconnectEvent* pEvent)
	{
		mDisconnEventCache.freeObject(pEvent);
	}

	// 接收事件
	IoRecvEvent* getRecvEvent()
	{
		return mRecvEventCache.newObject();
	}
	void putRecvEvent(IoRecvEvent* pEvent)
	{
		pEvent->init();
		mRecvEventCache.freeObject(pEvent);
	}

	// 发送事件
	IoSendEvent* getSendEvent()
	{
		return mSendEventCache.newObject();
	}
	void putSendEvent(IoSendEvent* pEvent)
	{
		pEvent->init();
		mSendEventCache.freeObject(pEvent);
	}

private:
	ObjectCache<IoConnectEvent> mConnEventCache;
	ObjectCache<IoDisconnectEvent> mDisconnEventCache;
	ObjectCache<IoRecvEvent> mRecvEventCache;
	ObjectCache<IoSendEvent> mSendEventCache;
};

