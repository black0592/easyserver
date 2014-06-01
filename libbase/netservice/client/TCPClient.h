#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__

#include "BaseTypes.h"
#include "thread/Thread.h"
#include "netservice/MsgProcessor.h"


namespace easygame {

	class TCPClient : public MessageHandlerQueue
	{
	public:
		virtual ~TCPClient(void){}

		// 连接服务器
		virtual bool connect(const char* ip, ushort port) = 0;

		// ping服务器
		virtual bool ping() = 0;

		// 断开与服务器的链接
		virtual bool close() = 0;

		// 关闭服务
		virtual void shutoff() = 0;

		// 重新连接
		virtual bool reconnect() = 0;

		//判断是否和服务器连接
		virtual bool isConnected() = 0;

		// 是否正在连接中
		//virtual bool IsWaitConnect() = 0;

		// 发送数据(任意大小的数据)
		virtual bool sendRawData(const void* data, int len) = 0;

		// 发送消息(进行组包操作)
		virtual bool sendCmd(const void* cmd, ushort cmdLen) = 0;

	public:
		// 打印运行信息
		virtual void printInfo() = 0;

	protected:
		//////////////////////////////////////////////////////////////////////////
		// 连接建立时被调动
		virtual void OnConnect() = 0;

		// 连接断开时被调用
		virtual void OnDisconnect() = 0;
	};


}	// namespace


#endif