#ifndef __XSOCKET_H__
#define __XSOCKET_H__

#include <BaseTypes.h>
#include <string>

#ifdef WIN32
#include <winsock2.h>
#include <Windows.h>
#else
typedef int SOCKET;
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <errno.h>
#endif

using namespace std;
using namespace easygame;


namespace xsocket
{
	class Socket
	{
	public:
		Socket();
		virtual ~Socket();

		bool create(ushort port = 0, int sendBuffSize = 128 * 1024, int recvBuffSize = 128 * 1024);

		int send(const void* buf, const int& size);

		int recv(void* buf, const int& size);

		void setBlock(bool block);

		// 关闭nagle算法(默认是开启的)
		void setNoDelay();

		// 获得数字IP地址
		uint getDigitalIP();

		// 获得字符IP地址
		const char* getIP();

		ushort getPort();

		bool connect(const char* ip, ushort port);

		bool reconnect();

		bool close();

		bool connected();

	protected:

	private:
		SOCKET	mSocket;
		uint	mIP;
		ushort	mPort;
		string	mStrIP;
		bool	mIsConnected;
	};





}	// namespace


#endif