#pragma once

/*linux epoll implement----------------------------------------------------------------------------------------------------*/

#ifdef LANCE_LINUX

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/time.h> 
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "ioevent.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
using namespace std;
using namespace easygame;

#define EPOLL_MAX_NFDS			10000		// max sockets queried by epoll.
#define EPOLL_MAX_EVENTS		100			// max events queried by epoll.
#define EPOLL_MAX_QUEUE			2048	// max events in cache queue.

namespace lance{ namespace net{

#if 1
	__inline int socket_set_keepalive(int fd)
	{
		int ret, error, flag, alive, idle, cnt, intv;

		/* Set: use keepalive on fd */    
		alive = 1;    
		if ( setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &alive,sizeof(alive)) != 0 ) {    
			printf("Set keepalive error: %s.\n", strerror (errno));    
			return -1;    
		}    

		/* 10���������ݣ�����������ƣ����ͱ���� */    
		idle = 10;    
		if ( setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, &idle, sizeof idle) != 0 ) {    
			printf("Set keepalive idle error: %s.\n", strerror (errno));    
			return -1;    
		}    

		/* ���û���յ���Ӧ����5���Ӻ��ط������ */    
		intv = 5;    
		if (setsockopt (fd, SOL_TCP, TCP_KEEPINTVL, &intv, sizeof intv) != 0) {    
			printf ("Set keepalive intv error: %s.\n", strerror (errno));    
			return -1;    
		}    

		/* ����3��û�յ����������Ϊ����ʧЧ */    
		cnt = 3;    
		if ( setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &cnt, sizeof cnt) != 0 ) {    
			printf ("Set keepalive cnt error: %s.\n", strerror (errno));    
			return -1;    
		}    

		return 0;    
	}  
#endif

namespace epoll
{
template<typename T>
class Queue
{
public: Queue()
	{
		len = 0;
		offset =  0;
	}

public: void in()
	{
		lock.lock();
	}
public: void out()
	{
		lock.unlock();
	}
public: void fullNotify()
	{
		fullSignal.notify_one();
	}
public: void fullWait()
	{
		fullSignal.wait(lock);
	}
public: void fullBroadcast()
	{
		fullSignal.notify_all();
	}
public: void emptyNotify()
	{
		emptySignal.notify_one();
	}
public: void emptyWait()
	{
		emptySignal.wait(lock);
	}
public: void emptyBroadcast()
	{
		emptySignal.notify_all();
	}


public: bool full()
	{
		return (EPOLL_MAX_QUEUE == len);
	}
public: bool empty()
	{
		return (0 == len);
	}
public: T * pop()
	{
		T * clt = clts[offset];
		len --;
		offset = (offset+1 == EPOLL_MAX_QUEUE)?0:(offset+1);
		return clt;
	}
public: void push(T * clt)
	{
		clts[(offset+len)%EPOLL_MAX_QUEUE] = clt;
		len ++;
	}

public: T* clts[EPOLL_MAX_QUEUE+1];
public: int len;
public: int offset;
public: std::mutex lock;
public: std::condition_variable_any emptySignal;
public: ::condition_variable_any fullSignal;
};

template<typename T>
class Epoller
{
public: void start();
public: void stop();

public: static void* run(void* param);

public: Queue<IoEventBase>* queue;

public: int epfd;
};

template<typename T>
void Epoller<T>::start()
{
	epfd = -1;
	try
	{
		epfd = epoll_create(EPOLL_MAX_NFDS);
		if (-1 == epfd)
		{
			TRACE( "Epoller - epoll_create fail" );
			throw errno;
		}

		std::thread epoller_thread( std::bind(&Epoller::run,this) );
		epoller_thread.detach();

		this->epfd = epfd;
	}
	catch(...)
	{
		TRACE( "Epoller - start fail" );

		if (-1 != epfd)
		{
			::close(epfd);
		}
		//throw;
	}
}

template<typename T>
void Epoller<T>::stop()
{
	//printf("Epoller stop\n");
}

template<typename T>
void * Epoller<T>::run(void * param)
{
	//printf("Epoller begin\n");

	Epoller<T>& eper = *(Epoller<T> *)param;
	Queue<IoEventBase>& queue = *eper.queue;
	int epfd = eper.epfd;

	try
	{
		int nfds;
		int capcity;
		struct epoll_event events[EPOLL_MAX_EVENTS];
		struct epoll_event ev;
		while (true)
		{
			//printf("Epoller epoll_wait begin\n");
			nfds = epoll_wait(epfd, events, EPOLL_MAX_EVENTS, 10 * 1000);
			for (int i = 0;i < nfds;i ++)
			{
				epoll_ctl(epfd, EPOLL_CTL_DEL, ((T *)events[i].data.ptr)->fd, &ev);
				//printf("Epoller epoll_delete fd=%d\n", clt->fd);
			}

			for (int i = 0;i < nfds;)
			{
				queue.in();
				while (queue.full())
				{
					queue.fullWait();
				}

				// ����ʣ������
				capcity = EPOLL_MAX_QUEUE - queue.len;
				if (capcity < nfds) {
					queue.out();
					//printf("����̫С�����������¼�������capcity=%d, nfds=%d\n", capcity, nfds);
					//Platform::sleep(5000);
					continue;
				}

				// �������ʣ���������ڴ����¼���Ŀ�������¼���Ŀ,�������������С������(�Ƿ������в����¼�����������)
				capcity = (capcity > nfds)?nfds:capcity;
				if (queue.empty())
				{
					if (capcity > 1)
					{
						queue.emptyBroadcast();
					}
					else
					{
						queue.emptyNotify();
					}
				}

				while (capcity > 0)
				{
					if (i >= EPOLL_MAX_EVENTS) {
						printf("�������¼�����Խ���ˣ�����, i=%d, nfds=%d, capcity=%d\n", i, nfds, capcity);
						//Platform::sleep(5000);
					}

					T* clt = (T*)events[i].data.ptr;
					if (clt == NULL) {
						int fd = clt->fd;
						printf("������ָ������쳣������\n");
						Platform::sleep(5000);
						break;
					}

					IoEventBase* pIoEvent = NULL;

					//��ʱע��
					if (events[i].events&EPOLLIN)
					{
						pIoEvent = IoEventPool::getInstance().getRecvEvent();
						pIoEvent->clt = clt;
						//printf("��������¼� event=%d\n", ioevt->event);
					}
					else
					{
						pIoEvent = IoEventPool::getInstance().getDisconnectEvent();
						pIoEvent->clt = clt;
						//printf("��������¼� clt=%d\n", clt->fd);
					}

					queue.push(pIoEvent);
					capcity --;	// ��������������
					i ++;		// ������һ���¼�
				}

				queue.out();
			}
		}
	}
	catch(...)
	{
		TRACE( "Epoller<T>::run error" );
		//throw;
	}

	//printf("Epoller end\n");

	return NULL;
}

template<typename T>
class Scheduler
{
public: void start();
public: void stop();
public: void push(IoEventBase* ioEvent);

public: int scheds;

public: Queue<IoEventBase> queue;
public: Epoller<T> * epers;
public: int epoff;
};

template<typename T>
void Scheduler<T>::start()
{
	try
	{
		this->epoff = 0;
		this->epers = new Epoller<T>[scheds];
		for (int i = 0;i < scheds;i ++)
		{
			this->epers[i].queue = &queue;
			this->epers[i].start();
		}
	}
	catch(...)
	{
		// ����ֹͣ��
		TRACE( "Scheduler - start fail" );
		stop();
		//throw;
	}
}

template<typename T>
void Scheduler<T>::stop()
{
	// epoll�ͷ�
	if (epers != NULL) {
		for (int i=0; i<scheds; i++) {
			epers[i].stop();
		}

		delete[] epers;
		epers = NULL;
	}

	// socket�ͷ�(ò�������⣬�Ժ��Ż�)
	/*printf("Scheduler<T>::stop1111\n");
	queue.in();
	while (!queue.empty())
	{
		printf("delete queue.pop()\n");
		T * clt = queue.pop();
		printf("�����˳����Ͽ�����\n");
		clt->OnDisconnect();
		::close(clt->fd);
		delete clt;
	}
	queue.out();
	printf("Scheduler<T>::stop222\n");*/
}

template<typename T>
void Scheduler<T>::push(IoEventBase* ioEvent)
{
	//printf("epoff: %d\n", epoff);
	T* clt = (T*)ioEvent->clt;
	clt->epfd = epers[epoff].epfd;
	epoff = (epoff+1 == scheds)?0:(epoff+1);
	queue.in();
	while (queue.full())
	{
		queue.fullWait();
	}
	if (queue.empty())
	{
		queue.emptyNotify();
	}
	queue.push(ioEvent);
	queue.out();
}

template<typename T>
class Processor
{
public: void start();
public: void stop();

public: static void * run(void * param);

public: int threads;
public: Scheduler<T> * scheder;
};

template<typename T>
void Processor<T>::start()
{
	for (int i = 0;i < threads;i ++)
	{
		std::thread processor_thread( std::bind(&Processor::run,this) );
		processor_thread.detach();
	}
}

template<typename T>
void Processor<T>::stop()
{
}

template<typename T>
void * Processor<T>::run(void * param)
{
	Processor<T>& procor = *(Processor<T> *)param;
	Queue<IoEventBase>& queue = procor.scheder->queue;

	IoEventBase* ioevt = NULL;
	int len = 0;
	while(true)
	{
		queue.in();
		while(queue.empty())
		{
			queue.emptyWait();
		}
		if (queue.full())
		{
			queue.fullNotify();
		}
		ioevt = queue.pop();
		T * clt = (T*)ioevt->clt;
		queue.out();

		switch(ioevt->event)
		{
		case EV_RECV:
			{
				IoRecvEvent* recvEvent = (IoRecvEvent*)ioevt;

				//printf("�յ������¼�!!!\n");

#if 1
				//////////////////////////////////////////////////////////////////////////
				len = ::recv(clt->fd, recvEvent->wsaBuf.buf, recvEvent->wsaBuf.len, MSG_NOSIGNAL);
				if (-1 == len && (EAGAIN == errno || EWOULDBLOCK == errno)) {	// EINTR
					printf("�������ݷ���0[errno=%d]���ȴ�����\n",errno);
					break;	//should retry
				}
				if (len > 0) {
					//clt->OnRecv(NULL, len);
					clt->OnRecv(recvEvent->wsaBuf.buf, len);
				} else if (len <= 0) {
					//printf("�������ݷ���0[errno=%d]���Ͽ�����\n",errno);

					// ѹ������¼�
					//IoEventBase* pIoEvent = IoEventPool::getInstance().getDisconnectEvent();
					//pIoEvent->clt = clt;
					//queue.push(pIoEvent);

					// ��ʱ�ر�
					clt->closeDelay();
					
					IoEventPool::getInstance().putRecvEvent(recvEvent);
					break;

					//clt->OnDisconnect();
					//::close(clt->fd);
					//delete clt;
					//break; //EOF
				}
				//printf("!@!@!@!@!@!@\n");
#else
				////////////////////////////////////////////////////////////////////////////
				//len = ::recv(clt->fd, clt->EPOLL_buff, clt->EPOLL_len, MSG_NOSIGNAL);
				//if (len <= 0)
				//{
				//	//printf("�������ݷ���0[errno=%d]���Ͽ�����\n",errno);
				//	clt->OnDisconnect();
				//	delete clt;
				//}
				//else
				//{
				//	clt->OnRecv(NULL, len);
				//}
#endif

				IoEventPool::getInstance().putRecvEvent(recvEvent);

				// ��������
				clt->recv();
			}
			break;
		case EV_CONNECT:
			{
				IoConnectEvent* connectEvent = (IoConnectEvent*)ioevt;

				clt->addConnectWaitList();

				//// ��������
				//tcp_keepalive live;      
				//live.keepaliveinterval=500;       
				//live.keepalivetime=1000 * 10; // ���Լ��Ϊ10�� Resend if No-Reply
				//live.onoff=TRUE;      
				//setsockopt(clt->fd, SOL_SOCKET, SO_KEEPALIVE, (const char*)&live, sizeof(live));

				// ��������
				socket_set_keepalive(clt->fd);

				IoEventPool::getInstance().putConnectEvent(connectEvent);
			}
			break;
		case EV_DISCONNECT:
			{
				//printf("�յ��Ͽ���������\n");
				IoDisconnectEvent* disconnectEvent = (IoDisconnectEvent*)ioevt;
				IoEventPool::getInstance().putDisconnectEvent(disconnectEvent);

				// ��ʱ�ر�
				clt->closeDelay();
			}
			break;

		case EV_SEND:
			{
				printf("�յ��������ݵ�����\n");
			}
			break;

		default:
			{
				printf("�յ�δע������� %d\n", ioevt->event);
			}
			break;
		}
	}

	return NULL;
}

template<typename T>
class Listener
{
public: bool start();
public: void stop();

public: static void * run(void * param);

public: int ip;
public: int port;
public: int backlogs;
public: void * service;
public: Scheduler<T> * scheder;
public: int fd;
};

template<typename T>
bool Listener<T>::start()
{
	fd = -1;
	try
	{
		fd = socket(AF_INET, SOCK_STREAM, 0);
		if (-1 == fd)
		{
			TRACE("create socket fail");
			throw errno;
		}

		//�����ַ����
		int on = 1;
		int ret = setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

		struct sockaddr_in addr;
		bzero(&addr, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = ip;
		addr.sin_port = htons(port);

		if (-1 == bind(fd, (struct sockaddr *)&addr, sizeof(addr)))
		{
			TRACE( format("bind port %d fail", port) );
			throw errno;
		}
		if (-1 == listen(fd, backlogs))
		{
			TRACE( format("listen port %d fail", port) );
			throw errno;
		}

		std::thread listener_thread( std::bind(&Listener::run,this) );
		listener_thread.detach();

		this->fd = fd;
	}
	catch(...)
	{
		if (-1 != fd)
		{
			::close(fd);
		}
		//throw;
		return false;
	}

	return true;
}

template<typename T>
void Listener<T>::stop()
{
	::close(fd);
}

template<typename T>
void * Listener<T>::run(void * param)
{
	Listener<T>& listener = *(Listener<T> *)param;
	Scheduler<T>& scheder = *listener.scheder;

	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	while (true)
	{
		int fd = accept(listener.fd, (struct sockaddr *)&addr, &addrlen);
		if (-1 == fd)
		{
			printf("���������ļ������������ƣ���������ulimit -n\n");
			break;
		}

		// �ر�nagle�㷨
		int nodelay = 1;
		::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&nodelay, sizeof(nodelay));
		
		T * clt = new T();
		clt->service = listener.service;
		clt->delayCloseTime = 0;
		clt->fd = fd;
		clt->ip = addr.sin_addr.s_addr;
		clt->port = htons(addr.sin_port);
		
		// IP���������
		if (clt->isInBlacklist()) {
			// [���԰�ȫɾ��]��û����ӵ�TaskManager
			delete clt;	// �϶��ǰ�ȫ�ģ���Ϊ��û����EPOLL�󶨣�δ�յ�������Ϣ
			//TRACE(strformat("[run] delete clt at 0x%08x\n", clt));
			continue;
		}

		//printf("��������index=%d\n");
		IoEventBase* pIoEvent = IoEventPool::getInstance().getConnectEvent();
		pIoEvent->clt = clt;
		scheder.push(pIoEvent);
	}

	printf("!!!exit the Listener thread!!!\n");

	return NULL;
}

}

class Client: public IClient
{
public: virtual ~Client()
		{
			//printf("�ر�socket fd=%d\n", fd);
			this->shutdown();
			::close(fd);
		}

public: int send(const char * buff, int len, bool writesize)
		{
#if 1
			//printf("������Ϣ");

			if (delayCloseTime > 0)
				return SEND_ERROR;

			if (len == 0)
				return SEND_ERROR;

			if (len > MAX_USER_PACK_SIZE) {
				printf("send ���͵���Ϣ̫���ˣ�����ʧ��!!!\n");
				return SEND_ERROR;
			}

			int realSize = len;

			IoSendEvent* pIoSend = IoEventPool::getInstance().getSendEvent();

			if (writesize) {
				// д����Ϣ����
				if ( !pIoSend->put(&len, HEADER_SIZE) ) {
					IoEventPool::getInstance().putSendEvent(pIoSend);
					closeDelay();
					return SEND_ERROR;
				}

				realSize += HEADER_SIZE;
			}

			// д����Ϣ����
			if ( !pIoSend->put(buff, len) ) {
				IoEventPool::getInstance().putSendEvent(pIoSend);
				closeDelay();
				return SEND_ERROR;
			}

			//FUNC_PF_EXT("���ͳ�ʱ", 10);

			int sendSize = 0;
			while (sendSize<realSize) {
				int retcode = ::send(fd, pIoSend->wsaBuf.buf, realSize, MSG_NOSIGNAL);
				if (retcode == -1) {
					// �����ʹ���
					int errorNum = 0;
					bool isCloseError = false;
					errorNum = errno;
					isCloseError = (errno != EINTR && errno != EWOULDBLOCK && errno != EAGAIN);
					if (!isCloseError) {
						// ���Լ�������
						continue;
					}

					printf("����ʧ��,error=%d\n", errorNum);
					closeDelay();
					return SEND_ERROR;
				}

				if (retcode != realSize) {
					printf("����δ����ȫ\n");
					Platform::sleep(60*1000);	// ������ʱ��ô�õ�
					return retcode;
				}

				sendSize += retcode;
			}

			IoEventPool::getInstance().putSendEvent(pIoSend);

			return sendSize;

#else
			return 0;
#endif

			//return ::send(fd, buff, len, MSG_NOSIGNAL);
		}
public: int recv()
		{
			if (delayCloseTime > 0)
				return 0;

			//printf("�����������fd=%d\n", this->fd);

			struct epoll_event ev;
			ev.data.ptr = this;
			ev.events = EPOLLIN;
			int result = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
			if (result != 0) {
				printf("recv()��������ʧ�� result=%d\n", result);
				closeDelay();
			}

			//printf("ѹ������¼�\n");
		}

		/*
public: int recv(char * buff, int len)
		{
			if (delayCloseTime > 0)
				return 0;

			if (len == 0) {
				printf("!!!!��������Ϊ0�������ܵ�\n");
				return 0;
			}

			this->EPOLL_buff = buff;
			this->EPOLL_len = len;

			struct epoll_event ev;
			ev.data.ptr = this;
			ev.events = EPOLLIN;
			epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

			return 0;
		}
		*/

public: void shutdown()
		{
			::shutdown(fd, SHUT_RDWR);
		}

public: void closeDelay(int64 second = 0)
		{
			shutdown();
			delayCloseTime = Platform::tick() + second * 1000;	// n���ر�����
		}

public: int epfd;
public: int fd;
public: int64 delayCloseTime;	// ��ʱ�ر�
//public: char * EPOLL_buff;
//public: int EPOLL_len;
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

public: epoll::Listener<T> listener;
public: epoll::Scheduler<T> scheder;
public: epoll::Processor<T> procor;
};

template<typename T>
bool TCPSrv<T>::start()
{
	////! linux����Ҫ���ε�һЩ�ź�
	//signal( SIGHUP, SIG_IGN );
	//signal( SIGALRM, SIG_IGN ); 
	//signal( SIGPIPE, SIG_IGN );
	//signal( SIGXCPU, SIG_IGN );
	//signal( SIGXFSZ, SIG_IGN );
	//signal( SIGPROF, SIG_IGN ); 
	//signal( SIGVTALRM, SIG_IGN );
	//signal( SIGQUIT, SIG_IGN );
	//signal( SIGCHLD, SIG_IGN);

	if (backlogs <= 0)
	{
		backlogs = 10;
	}
	if (threads <= 0)
	{
		threads = 1;
	}
	if (scheds <= 0)
	{
		scheds = 1;
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
	listener.stop();
	procor.stop();
	scheder.stop();
}


}}

#endif

