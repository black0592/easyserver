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

		// ���ӷ�����
		virtual bool connect(const char* ip, ushort port) = 0;

		// ping������
		virtual bool ping() = 0;

		// �Ͽ��������������
		virtual bool close() = 0;

		// �رշ���
		virtual void shutoff() = 0;

		// ��������
		virtual bool reconnect() = 0;

		//�ж��Ƿ�ͷ���������
		virtual bool isConnected() = 0;

		// �Ƿ�����������
		//virtual bool IsWaitConnect() = 0;

		// ��������(�����С������)
		virtual bool sendRawData(const void* data, int len) = 0;

		// ������Ϣ(�����������)
		virtual bool sendCmd(const void* cmd, ushort cmdLen) = 0;

	public:
		// ��ӡ������Ϣ
		virtual void printInfo() = 0;

	protected:
		//////////////////////////////////////////////////////////////////////////
		// ���ӽ���ʱ������
		virtual void OnConnect() = 0;

		// ���ӶϿ�ʱ������
		virtual void OnDisconnect() = 0;
	};


}	// namespace


#endif