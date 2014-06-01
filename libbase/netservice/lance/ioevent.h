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

// io�¼�����
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

// io�����¼�
struct IoConnectEvent : public IoEventBase
{
	IoConnectEvent()
	{
		event = EV_CONNECT;
	}
};

// io�Ͽ������¼�
struct IoDisconnectEvent : public IoEventBase
{
	IoDisconnectEvent()
	{
		event = EV_DISCONNECT;
	}
};

struct IoRecvEvent : public IoEventBase
{
	WSABUF	wsaBuf;			// ����ʱ��
	char buffer[MAX_USER_RECV_SIZE];	// ����/���ͻ���

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
	uint dwSentBytes;		// �Ѿ����͵��ֽ���
	WSABUF	wsaBuf;			// ����ʱ��
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

	// �����¼�
	IoConnectEvent* getConnectEvent()
	{
		return mConnEventCache.newObject();
	}
	void putConnectEvent(IoConnectEvent* pEvent)
	{
		mConnEventCache.freeObject(pEvent);
	}

	// �Ͽ������¼�
	IoDisconnectEvent* getDisconnectEvent()
	{
		return mDisconnEventCache.newObject();
	}
	void putDisconnectEvent(IoDisconnectEvent* pEvent)
	{
		mDisconnEventCache.freeObject(pEvent);
	}

	// �����¼�
	IoRecvEvent* getRecvEvent()
	{
		return mRecvEventCache.newObject();
	}
	void putRecvEvent(IoRecvEvent* pEvent)
	{
		pEvent->init();
		mRecvEventCache.freeObject(pEvent);
	}

	// �����¼�
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

