#include "xsocket.h"
#include "BaseLib.h"

#ifdef WIN32
//#define EWOULDBLOCK	10035
#define MSG_NOSIGNAL 0
#else
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif

namespace xsocket
{

	Socket::Socket()
	{
		mSocket = INVALID_SOCKET;
		mIP = -1;
		mPort = -1;
		mIsConnected = false;
	}

	Socket::~Socket()
	{
	}

	bool Socket::create(ushort port, int sendBuffSize, int recvBuffSize)
	{
		if (mSocket != INVALID_SOCKET)
			return false;

		mSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (INVALID_SOCKET == mSocket)
			return false;

		//设置套接口发送接收缓冲，并且客户端的必须在connect之前设置
		int retcode;
		retcode = ::setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&recvBuffSize, sizeof(recvBuffSize));
		if (-1 == retcode) {
			return false;
		}
		retcode = ::setsockopt(mSocket, SOL_SOCKET, SO_SNDBUF, (const char*)&sendBuffSize, sizeof(sendBuffSize));
		if (-1 == retcode) {
			return false;
		}

		if (port > 0) {
			sockaddr_in sockAddr;
			memset(&sockAddr,0,sizeof(sockAddr));
			sockAddr.sin_family = AF_INET;
			sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			sockAddr.sin_port = htons(port);
			if (SOCKET_ERROR == ::bind(mSocket,(sockaddr*)&sockAddr,sizeof(sockAddr)))
				return false;
		}
		
		return true;
	}

	bool Socket::connect(const char* ip, ushort port)
	{
		if (!create())
			return false;

		sockaddr_in sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_addr.s_addr = inet_addr(ip);
		sockAddr.sin_port = htons(port);

		if (INADDR_NONE == sockAddr.sin_addr.s_addr) {
			struct hostent* lphost;
			lphost = gethostbyname(ip);
			if (!lphost) {
				close();
				return false;
			}
			sockAddr.sin_addr.s_addr = ((struct in_addr*)lphost->h_addr)->s_addr;
		}

		if (-1 == ::connect(mSocket, (sockaddr*)&sockAddr, sizeof(sockAddr))) {
			BLOGE("connect function failed !\n");
			this->close();
			// 连接失败，也记录下IP和Port，方便下次重练
			mIP = sockAddr.sin_addr.s_addr;
			mPort = port;
			mStrIP = ::getStrIP(mIP);
			return false;
		}

		//将socket设置为非堵塞模式
		setBlock(false);

		// 关闭nagle算法
		setNoDelay();

		mIP = sockAddr.sin_addr.s_addr;
		mPort = port;
		mStrIP = ::getStrIP(mIP);

		// 链接成功
		mIsConnected = true;

		return true;
	}

	bool Socket::reconnect()
	{
		close();
		return connect(mStrIP.c_str(), mPort);
	}

	bool Socket::close()
	{
		if (-1 != mSocket) {
			shutdown(mSocket, 0);	//SD_BOTH
#ifdef WIN32
			::closesocket(mSocket);
#else
			::close(mSocket);
#endif
			mSocket = INVALID_SOCKET;

			mIsConnected = false;

			return true;
		}
		
		return false;
	}

	int Socket::send(const void* buf, const int& size)
	{
		return ::send(mSocket, (const char*)buf, size, MSG_NOSIGNAL);
	}

	int Socket::recv(void* buf, const int& size)
	{
		return ::recv(mSocket, (char*)buf, size, MSG_NOSIGNAL);
	}

	void Socket::setBlock(bool block)
	{
		//将socket设置为非堵塞模式
#ifdef WIN32
		unsigned long flag = 1;
		if (block) flag = 0;
		int retcode = ioctlsocket(mSocket, FIONBIO, &flag);
#else
		int flag;  
		flag = fcntl(mSocket,F_GETFL,0);
		if (block) flag = flag & (~O_NONBLOCK);
		int retcode = fcntl(mSocket,F_SETFL,flag|O_NONBLOCK);  
#endif

		retcode = retcode;
	}

	void Socket::setNoDelay()
	{
		int nodelay = 1;
		::setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&nodelay, sizeof(nodelay));
	}

	// 获得数字IP地址
	uint Socket::getDigitalIP()
	{
		return mIP;
	}

	// 获得字符IP地址
	const char* Socket::getIP()
	{
		return mStrIP.c_str();
	}

	ushort Socket::getPort()
	{ 
		return mPort;
	}

	bool Socket::connected()
	{
		return mIsConnected;
	}




}	// namespace
