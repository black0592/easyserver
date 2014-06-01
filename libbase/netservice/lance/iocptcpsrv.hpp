#pragma once


#ifdef LANCE_WIN32
#include <stdio.h>	// printf
#include <windows.h>
#include <winsock2.h>
#include <mstcpip.h>
#include <atomic>
#include "ioevent.h"
#pragma comment(lib, "Ws2_32.lib")

namespace lance{ namespace net{
 
namespace iocp
{
	//////////////////////////////////////////////////////////////////////////

template<typename T>
class Scheduler
{
public: void start();
public: void stop();

public: void push(T * clt, IoEventBase* ioEvent);

public: int scheds;

public: HANDLE iocp;
};

template<typename T>
void Scheduler<T>::start()
{
	iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, scheds);
	if (NULL == iocp)
	{
		throw (int)::WSAGetLastError();
	}
}

template<typename T>
void Scheduler<T>::stop()
{
}

template<typename T>
void Scheduler<T>::push(T * clt, IoEventBase* ioEvent)
{
	::PostQueuedCompletionStatus(iocp, 0, (ULONG_PTR)clt, (LPOVERLAPPED)ioEvent);
}

template<typename T>
class Processor
{
public: void start();
public: void stop();

public: int threads;
public: Scheduler<T> * scheder;

public: static DWORD WINAPI run(LPVOID param);
};

template<typename T>
void Processor<T>::start()
{
	for (int i = 0;i < threads;i ++)
	{
		DWORD tid;
		HANDLE thd = ::CreateThread(NULL,
									0,
									(LPTHREAD_START_ROUTINE)run,
									this,
									0,
									&tid);
		if (NULL == thd)
		{
			throw (int)::GetLastError();
		}
		::CloseHandle(thd);
	}
}

template<typename T>
void Processor<T>::stop()
{
}

template<typename T>
DWORD WINAPI Processor<T>::run(LPVOID param)
{
	Processor<T>& procor = *(Processor<T> *)param;
	Scheduler<T>& scheder = *procor.scheder;
	HANDLE iocp = scheder.iocp;

	DWORD numOfBytesTransed = 0;
	ULONG_PTR key = NULL;
	WSAOVERLAPPED * overlap = NULL;
	while (true)
	{
		overlap = NULL;
		::GetQueuedCompletionStatus(iocp, &numOfBytesTransed, &key, (LPOVERLAPPED *)&overlap, INFINITE);
		//::GetQueuedCompletionStatus(iocp, &numOfBytesTransed, &key, (LPOVERLAPPED *)&overlap, 10 * 1000);
		if (overlap == NULL)
			continue;

		T * clt = (T *)key;
		IoEventBase* evt = (IoEventBase*)overlap;
		switch(evt->event)
		{
		case EV_RECV:
			{
				IoRecvEvent* recvEvent = (IoRecvEvent*)overlap;

				if (numOfBytesTransed == 0)
				{
					// 不采用发消息，采用设置标志位，貌似安全点
					clt->closeDelay();

					// 通过发消息来断开连接（但是有几率宕机）
					//IoDisconnectEvent* pIoEvent = IoEventPool::getInstance().getDisconnectEvent();
					//::PostQueuedCompletionStatus(iocp, 0, (ULONG_PTR)clt, pIoEvent);

					IoEventPool::getInstance().putRecvEvent(recvEvent);
					clt->decreaseEventCount();
				}
				else
				{
					clt->OnRecv(recvEvent->buffer, numOfBytesTransed);
					IoEventPool::getInstance().putRecvEvent(recvEvent);
					clt->decreaseEventCount();

					// 继续接收
					clt->recv();
				}
			}
			break;
		case EV_CONNECT:
			{
				IoConnectEvent* connectEvent = (IoConnectEvent*)overlap;

				if (NULL == ::CreateIoCompletionPort((HANDLE)clt->fd, iocp, (ULONG_PTR)clt, 0))
				{
					// [可以安全删除]还没有添加到TaskManager
					//delete clt;
					TRACE( strformat("[宕机隐患][Processor<T>::run-EV_CONNECT] connect fail! delete clt at 0x%08x\n", clt) );
				}
				else
				{
					clt->addConnectWaitList();

					// 设置心跳
					tcp_keepalive live;      
					live.keepaliveinterval=500;       
					live.keepalivetime=1000 * 10; // 重试间隔为10秒 Resend if No-Reply
					live.onoff=TRUE;      
					setsockopt(clt->fd, SOL_SOCKET, SO_KEEPALIVE, (const char*)&live, sizeof(live));
				}

				IoEventPool::getInstance().putConnectEvent(connectEvent);

				clt->decreaseEventCount();
			}
			break;
		case EV_DISCONNECT:
			{
				IoDisconnectEvent* disconnectEvent = (IoDisconnectEvent*)overlap;
				IoEventPool::getInstance().putDisconnectEvent(disconnectEvent);

				BLOGW( "[EV_DISCONNECT] clt at 0x%08x\n", clt );

				// 延时关闭
				clt->closeDelay();
			}
			break;
		case EV_SEND:
			{
				IoSendEvent* sendEvent = (IoSendEvent*)overlap;

				if (numOfBytesTransed == 0) {
					// 出错了，统一由recv处理
				} else {
					if (sendEvent->dwSentBytes + numOfBytesTransed < sendEvent->wsaBuf.len) {
						// 数据未发送全，需要继续发送
						sendEvent->dwSentBytes += numOfBytesTransed;
						const char* sendBuff = (char*)sendEvent->buffer.getBuffer() + sendEvent->dwSentBytes;
						int leftLen = sendEvent->wsaBuf.len - sendEvent->dwSentBytes;
						clt->send(sendBuff, leftLen, false);
					}
				}

				IoEventPool::getInstance().putSendEvent(sendEvent);
				clt->decreaseEventCount();
			}
			break;
		}
	}

	return 0;
}

template<typename T>
class Listener
{
public: bool start();
public: void stop();

public: int ip;
public: int port;
public: int backlogs;
public: void * service;
public: Scheduler<T> * scheder;

public: static DWORD WINAPI run(LPVOID param);

public: SOCKET fd;
};

template<typename T>
bool Listener<T>::start()
{
	SOCKET fd = INVALID_SOCKET;

	try
	{
		fd = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (INVALID_SOCKET == fd)
		{
			throw (int)WSAGetLastError();
		}

		////设置套接口发送接收缓冲，并且客户端的必须在connect之前设置
		//int recvBuffSize = 128 * 1024;
		//int sendBuffSize = 128 * 1024;
		//int retcode = 0;
		//retcode = ::setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*)&recvBuffSize, sizeof(recvBuffSize));
		//if (-1 == retcode) {
		//	return false;
		//}

		//retcode = ::setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&sendBuffSize, sizeof(sendBuffSize));
		//if (-1 == retcode) {
		//	return false;
		//}

		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = ip;
		addr.sin_port = htons(port);
		if (SOCKET_ERROR == ::bind(fd, (struct sockaddr *)&addr, sizeof(addr)))
		{
			printf("[error] bind port %u fail.\n", port);
			throw (int)::WSAGetLastError();
		}

		if (SOCKET_ERROR == ::listen(fd, backlogs))
		{
			throw (int)::WSAGetLastError();
		}


		DWORD tid;
		HANDLE thd = ::CreateThread(NULL,
									0,
									(LPTHREAD_START_ROUTINE)run,
									this,
									0,
									&tid);
		if (NULL == thd)
		{
			throw (int)::GetLastError();
		}
		::CloseHandle(thd);

		this->fd = fd;
	}
	catch(...)
	{
		if (INVALID_SOCKET != fd)
		{
			::closesocket(fd);
			return false;
		}
		//throw;
	}

	return true;
}

template<typename T>
void Listener<T>::stop()
{
	::closesocket(fd);
}

template<typename T>
DWORD WINAPI Listener<T>::run(LPVOID param)
{
	Listener<T>& listener = *(Listener<T> *)param;
	Scheduler<T>& scheder = *(Scheduler<T> *)listener.scheder;
	SOCKET lsfd = listener.fd;
	HANDLE iocp = scheder.iocp;

	SOCKET fd;
	struct sockaddr_in addr;
	int addrlen = sizeof(addr);
	while (true)
	{
		fd = ::WSAAccept(lsfd, (struct sockaddr *)&addr, &addrlen, NULL, NULL);
		if (INVALID_SOCKET == fd)
		{
			int error = ::WSAGetLastError();
			printf("[error] WSAAccept fail err=%d.\n", error);
			Platform::sleep(5000);
			continue;
		}

		// 关闭nagle算法
		int nodelay = 1;
		::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&nodelay, sizeof(nodelay));

		T * clt = new T();
		clt->service = listener.service;
		clt->isShutDown = false;
		clt->delayCloseTime = 0;
		clt->ioEventCount = 0;
		clt->iocp = iocp;
		clt->fd = fd;
		clt->ip = addr.sin_addr.s_addr;
		clt->port = htons(addr.sin_port);


		// IP黑名单检测
		if (clt->isInBlacklist()) {
			// [可以安全删除]还没有添加到TaskManager
			delete clt;
			TRACE( strformat("[宕机隐患][Listener<T>::run] delete clt at 0x%08x\n", clt) );
			continue;
		}

		clt->increaseEventCount();
		IoConnectEvent* pIoEvent = IoEventPool::getInstance().getConnectEvent();
		scheder.push(clt, pIoEvent);
	}

	return 0;
}

}

class Client : public IClient
{
public: virtual ~Client()
		{
			//printf("关闭socket fd=%d\n", fd);
			this->shutdown();
			::closesocket(fd);
		}

public: int send(const char * buff, int len, bool writesize)
		{
			using namespace iocp;

			if (delayCloseTime > 0)
				return SEND_ERROR;

			if (len == 0)
				return SEND_ERROR;

			if (len > MAX_USER_PACK_SIZE) {
				printf("send 发送的消息太长了，发送失败!!!\n");
				return SEND_ERROR;
			}

			int realSize = len;

			IoSendEvent* pIoSend = IoEventPool::getInstance().getSendEvent();

			if (writesize) {
				// 写入消息长度
				if ( !pIoSend->put(&len, HEADER_SIZE) ) {
					IoEventPool::getInstance().putSendEvent(pIoSend);
					closeDelay();
					return SEND_ERROR;
				}

				realSize += HEADER_SIZE;
			}

			// 写入消息内容
			if ( !pIoSend->put(buff, len) ) {
				IoEventPool::getInstance().putSendEvent(pIoSend);
				closeDelay();
				return SEND_ERROR;
			}

			DWORD ready = 0;
			if ( ::WSASend(fd, &pIoSend->wsaBuf, 1, &ready, 0, pIoSend, NULL) == SOCKET_ERROR ) {
				int lastError = WSAGetLastError();
				if (lastError != WSA_IO_PENDING) {
#ifdef _DEBUG
					printf("【错误】socket send error fd=%08x,error=%d\n", fd, lastError);
#endif
					IoEventPool::getInstance().putSendEvent(pIoSend);
					closeDelay();
					return SEND_ERROR;
				}
			}

			this->increaseEventCount();
			return realSize;
		}

public: int recv()
		{
			using namespace iocp;

			if (delayCloseTime > 0)
				return 0;

			IoRecvEvent* pIoRecv = IoEventPool::getInstance().getRecvEvent();
			DWORD ready = 0;
			DWORD flags = 0;
			if ( ::WSARecv(fd, &pIoRecv->wsaBuf, 1, &ready, &flags, pIoRecv, NULL) == SOCKET_ERROR ) {
				int lastError = WSAGetLastError();
				if (lastError != WSA_IO_PENDING) {
#ifdef _DEBUG
					printf("【错误】socket recv error fd=%08x,error=%d\n", fd, lastError);
#endif
					IoEventPool::getInstance().putRecvEvent(pIoRecv);
					closeDelay();
					return 0;
				}
			}

			this->increaseEventCount();
			return 0;
		}

		// 事件计数器增加
public: void increaseEventCount()
		{
			++ioEventCount;
			//printf("eventCount: fd=%d count=%d\n", fd, ioEventCount);
		}

		// 事件计数器减少
public: void decreaseEventCount()
		{
			--ioEventCount;
			//printf("eventCount: fd=%d count=%d\n", fd, ioEventCount);
		}

		// 事件是否为空了
public: bool isEventEmpty()
		{
			return ioEventCount == 0;
		}

public: void shutdown()
		{
			if (!isShutDown) {
				::shutdown(fd, SD_BOTH);
				isShutDown = true;
			}
		}

public: void closeDelay(int64 second = 0)
		{
			// 先关闭发送和接收
			shutdown();
			delayCloseTime = Platform::tick() + second * 1000;	// n秒后关闭链接
		}

public: HANDLE iocp;
public: SOCKET fd;
public: int64 delayCloseTime;	// 延时关闭
public: bool isShutDown;		// 是否已经关闭了
public: atomic<int> ioEventCount;		// io事件计数器
};

template<typename T>
class TCPSrv : public ITCPSrv
{
public: TCPSrv()
	{
		ip = 0;
		port = 1234;
		backlogs = 10;
		threads = 1;
		scheds = 1;
	}

public: bool start();
public: void stop();

public: iocp::Scheduler<T> scheder;
public: iocp::Processor<T> procor;
public: iocp::Listener<T> listener;
};

template<typename T>
bool TCPSrv<T>::start()
{
	static WSADATA wsadata;
	static int wsaversion = WSAStartup(MAKEWORD(2,2), &wsadata);

	if (backlogs <= 0)
	{
		backlogs = 10;
	}
	if (threads <= 0)
	{
		threads = 1;
	}

	listener.ip = ip;
	listener.port = port;
	listener.backlogs = backlogs;
	listener.scheder = &scheder;
	if (!listener.start())
		return false;

	scheder.scheds = scheds;
	scheder.start();

	procor.threads = threads;
	procor.scheder = &scheder;
	procor.start();

	return true;
}

template<typename T>
void TCPSrv<T>::stop()
{
}

}}

#endif




