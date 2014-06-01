#include "SocketClient.h"
#include "BaseDefine.h"
#include "logger/Logger.h"
#include "debugger/Profiler.h"
#include "BaseCmd.h"
#include "zlib.h"

#define ENABLE_RBOOT		0
#define ONETHREAD_MAXCLIENT	200
//#define RECV_BUFF_SIZE		MAX_USER_PACK_SIZE

#ifdef WIN32
//#define EWOULDBLOCK	10035
#define MSG_NOSIGNAL 0
#else
#define INVALID_SOCKET -1
#endif
#define SEND_ERROR -1
#define RECV_ERROR -1

namespace easygame {

	//bool isTryAgain(int len)
	//{
	//	return (-1 == len && (EAGAIN == errno || EWOULDBLOCK == errno));
	//}

	// ��Կ
	static byte EncryptKey[] = 
	{ 
		0x82, 0xf6, 0x79, 0x71, 0x3e, 0xa3, 0x21, 0xd8, 0xb7, 0x34, 0x9f, 0x39, 0xf3, 0x43, 0xb0, 0xec, 0xba, 0x35, 0xca, 0x48, 
		0xa1, 0x49, 0xcd, 0xdd, 0xb9, 0x6a, 0x03, 0x0a, 0xce, 0x92, 0x03, 0x58, 0xd0, 0x31, 0xb4, 0x05, 0xaf, 0x01, 0xbc, 0xa5, 
		0x2a, 0xa9, 0xf6, 0x6c, 0x14, 0x7f, 0xad, 0x1a, 0xce, 0xfa, 0xba, 0x76, 0x7c, 0x78, 0x05, 0x45, 0xad, 0x06, 0x31, 0xc6, 
		0x65, 0xd8, 0xaa, 0x5b, 0xf3, 0x69, 0xcf, 0x81, 0x94, 0xd4, 0x53, 0x31, 0xc2, 0x02, 0xcf, 0x2f, 0x7a, 0x7b, 0x72, 0x14, 
		0x6f, 0x92, 0xb5, 0xac, 0x26, 0xfa, 0x0d, 0xc9, 0xfa, 0x02, 0x4b, 0x08, 0xf9, 0x90, 0x4a, 0x38, 0x29, 0xe6, 0x5b, 0x31, 
		0x48, 0xee, 0x4a, 0xc2, 0x68, 0x7a, 0x78, 0x2e, 0x2e, 0x4b, 0x15, 0x1f, 0xda, 0xda, 0xdc, 0xbc, 0xa0, 0x3d, 0x73, 0x51, 
		0x1a, 0xbb, 0x61, 0x9a, 0xdd, 0xdf, 0xd1, 0x0d, 0xc3, 0x4e, 0x94, 0x91, 0xb1, 0x51, 0x71, 0xa2, 0x2d, 0xec, 0x2f, 0x6f, 
		0x7f, 0x54, 0x26, 0x4e, 0x87, 0x9b, 0xd4, 0x08, 0xd3, 0xd3, 0x51, 0x77, 0x89, 0x77, 0xce, 0xca, 0xbb, 0x5b, 0x73, 0xe7, 
		0x2a, 0x36, 0xda, 0xb9, 0x40, 0x35, 0x06, 0x12, 0xf4, 0xc6, 0x11, 0x87, 0x8e, 0x3d, 0x78, 0xb3, 0xcf, 0x03, 0x66, 0x4c, 
		0xe5, 0x97, 0xd4, 0x12, 0xdc, 0x22, 0x62, 0x7b, 0xf5, 0xb8, 0xb6, 0xfa, 0xcd, 0xca, 0x20, 0x4b, 0x30, 0xd2, 0x8a, 0x42, 
		0x17, 0x6d, 0xda, 0x98, 0xf1, 0xd4, 0x6c, 0x80, 0x5a, 0x4d, 0x0f, 0xdf, 0xd5, 0x7f, 0xd4, 0xb3, 0xd2, 0x61, 0x0a, 0xe0, 
		0x74, 0xf8, 0x72, 0x3c, 0x69, 0xf9, 0x6d, 0x36, 0x1d, 0x07, 0xd0, 0x5c, 0xad, 0xe7, 0x82, 0x1a, 0xea, 0x4a, 0x6a, 0x93, 
		0xc6, 0x2e, 0x09, 0x86, 0x36, 0xb3, 0x3f, 0x0a, 0x2a, 0x83, 0x5b, 0xb5, 0xb2, 0x36, 0x14, 0xbe, 0xa4, 0x82, 0x73, 0xe9, 
		0x64, 0x8d, 0x24, 0x79, 0x1c, 0xc9, 0x7b, 0xf4, 0x55, 0xbe, 0x77, 0x13, 0xc4, 0x50, 0x7b, 0x71, 0xf5, 0xd6, 0x98, 0xbb, 
		0xd9, 0xae, 0xf0, 0x3a, 0xd8, 0xcc, 0xf8, 0x7a, 0x3e, 0xc3, 0xc5, 0x80, 0xe3, 0x77, 0xeb, 0xc7, 0xd7, 0xac, 0x49, 0xa1, 
		0x84, 0xb8, 0xc7, 0xd0, 0x41, 0xbb, 0x95, 0x54, 0xe1, 0x6d, 0x69, 0x6c, 0xdb, 0x9d, 0x12, 0x26, 0xae, 0x70, 0x35, 0x58, 
		0x28, 0xc9, 0xed, 0x59, 0x23, 0x1d, 0x37, 0x47, 0x62, 0x9a, 0x33, 0x17, 0x9e, 0x0a, 0x10, 0xd9, 0x25, 0x90, 0x29, 0x5b, 
		0x35, 0x7e, 0xf2, 0x99, 0x3a, 0xcd, 0x09, 0x8d, 0x78, 0xed, 0x04, 0x4d, 0x3a, 0x67, 0x1d, 0x32, 0x26, 0xda, 0x18, 0x07, 
		0x64, 0x8c, 0x3a, 0x9c, 0x1f, 0x83, 0x24, 0x13, 0x54, 0x76, 0xc9, 0x0c, 0xea, 0x12, 0x79, 0xbe, 0x25, 0x9e, 0xbc, 0x15, 
		0x08, 0x3e, 0xfa, 0x50, 0xab, 0x32, 0x77, 0x9a, 0x9d, 0x92, 0x38, 0x09, 0x98, 0x0c, 0x76, 0x2d, 0x3e, 0x58, 0x4b, 0x5b, 
		0x19, 0xce, 0x21, 0x94, 0x5a, 0x74, 0xbe, 0x6f, 0xd6, 0xe6, 0x32, 0xbb, 0xee, 0xbb, 0x16, 0x29, 0x9c, 0x66, 0xe9, 0x4c, 
		0xdf, 0x35, 0x93, 0x56, 0xbe, 0x55, 0xa4, 0x08, 0xcb, 0xda, 0x43, 0x8f, 0x94, 0xb5, 0x9d, 0x88, 0x9a, 0x52, 0xf4, 0x28, 
		0xbc, 0x64, 0x29, 0x47, 0x53, 0xca, 0xbe, 0xac, 0x50, 0x0f, 0x75, 0x92, 0xa9, 0xcb, 0x5a, 0xd6, 0xbf, 0x96, 0x84, 0x01, 
		0x30, 0xba, 0x0b, 0x69, 0x0e, 0xf5, 0x69, 0xe8, 0x77, 0x57, 0x7c, 0x37, 0x30, 0x69, 0x10, 0x87, 0xcd, 0x40, 0x71, 0xaf, 
		0x12, 0xbc, 0xa1, 0x2e, 0x8e, 0xbd, 0x19, 0x68, 0xaf, 0x7f, 0x78, 0x3f, 0x5e, 0xb8, 0xc7, 0xc7, 0x0d, 0x40, 0x8f, 0x2c, 
	};
	static const uint ENCRYPT_KEY_LEN = sizeof(EncryptKey)/sizeof(EncryptKey[0]);


	SocketClient::SocketClient(const string& name)
		: mSendBuff(MAX_USER_SEND_SIZE, 500*1024*1024)
		, mRecvBuff(MAX_USER_PACK_SIZE)
		, mName(name)
		, mIsDeleteMe(false)
	{
		//mIsConnected = false;
		//mIsRunning = true;
		//mIP = "127.0.0.1";
		//mPort = 6800;

#ifdef WIN32
		WSADATA WSAData;
		if ( WSAStartup(WINSOCK_VERSION,&WSAData) ) {
			BLOGE("��ʼ��WSAStartupʧ��");
		}
#endif

		//m_threadSend = CreateThread(NULL, 0, SendThread, this, 0, NULL);
		//if( m_threadSend == NULL) {
		//	printf("CreateSendThread failure!\n");
		//}

		//m_threadRecv = CreateThread(NULL, 0, RecvThread, this, 0, NULL);
		//if(m_threadRecv == NULL) {
		//	printf(" CreateRecvThread failure!\n");
		//}


		// �����
		//printf("\n׼����� %s Client\n", getName());

		bool success = TCPClientThreadPool::getInstance().addTCPClient(this);
		if (!success)
			BLOGE("SocketClient %s �����̳߳�ʧ��", getName());

		//printf("�ɹ���� %s Client\n\n", getName());

		resetEncrypt();
	}

	SocketClient::~SocketClient()
	{
		shutoff();

		// �����Զ��ͷ�
		// // �ͷ�ʱɾ��
		// //TCPClientThreadPool::getInstance().removeTCPClient(this);

#ifdef WIN32
		WSACleanup();
#endif
	}

	const char* SocketClient::getName()
	{
		return mName.c_str();
	}

	// �رշ���
	void SocketClient::shutoff()
	{
		BLOGE("�رշ�����ɶϿ�����");

		//TCPClientThreadPool::getInstance().removeTCPClient(this);

		//Thread::stop();
		close();
		//mIsRunning = false;
		//WaitForSingleObject(m_threadSend, 1000);
		//WaitForSingleObject(m_threadRecv, 1000);
	}

	// ���ӷ�����
	bool SocketClient::connect(const char* ip, ushort port)
	{
		//printf("׼�� %s SocketClient::connect\n", getName());

		if (mSocket.connected()) {
			//printf("�ظ����� %s SocketClient::connect\n", getName());
			return false;
		}

		bool success = mSocket.connect(ip, port);

		//setBlock(false);
		//mIsConnected = true;
		if (success) {
			OnConnect();
			resetEncrypt();
		}

		//// ���Ƴ�
		//BLOGW("׼���Ƴ�Client");
		////TCPClientThreadPool::getInstance().removeTCPClient(this);

		//// �����
		//BLOGW("׼�����Client");
		//success &= TCPClientThreadPool::getInstance().addTCPClient(this);

		//BLOGW("�ɹ����Client");

		//printf("�ɹ� %s SocketClient::connect\n", getName());

		return success;
	}

//	void SocketClient::setBlock(bool block)
//	{
////		//��socket����Ϊ�Ƕ���ģʽ
////#ifdef WIN32
////		unsigned long flag = 1;
////		if (block) flag = 0;
////		ioctlsocket(mSocket, FIONBIO, &flag);
////#else
////		int flag;  
////		flag = fcntl(mSocket,F_GETFL,0);
////		if (block) flag = flag & (~O_NONBLOCK);
////		fcntl(mSocket,F_SETFL,flag|O_NONBLOCK);  
////#endif
//	}

	// ping������
	bool SocketClient::ping()
	{
		return true;
	}

	// �Ͽ��ͻ���
	bool SocketClient::close()
	{
		// ��崻�
		//TCPClientThreadPool::getInstance().removeTCPClient(this);

		mSocket.close();
		OnDisconnect();

		return true;
	}


	// ��������
	bool SocketClient::reconnect()
	{
		//BLOGE("��������ɶϿ�����");
		bool success = mSocket.reconnect();

		if (success) {
			OnConnect();
			resetEncrypt();
		}

		return success;
	}

	//�ж��Ƿ�ͷ�������������״̬
	bool SocketClient::isConnected()
	{
		return mSocket.connected();
	}


	int SocketClient::send(const char * buff, int len, bool writesize)
	{
		if ( !isConnected() )
			return 0;

		FUNC_PF_EXT("���ͳ�ʱ", 10);

		int realSize = len;

		if (len == 0)
			return 0;

		if (mEnableEncrypt) {
			// ��������
			stBaseCmd* pCmd = (stBaseCmd*)buff;
			pCmd->flag |= FLAG_ENCRYPT;
			byte* pData = (byte*)pCmd + sizeof(pCmd->flag);
			uint dataLen = len - sizeof(pCmd->flag);
			Encrypt(pData, dataLen);
		}

		mSendMutex.lock();

		if (writesize) {
			mSendBuff.write(&len, HEADER_SIZE);
			realSize += HEADER_SIZE;
		}

		mSendBuff.write(buff, len);

		mSendMutex.unlock();

		return realSize;
	}

	// ��������
	bool SocketClient::sendRawData(const void* data, int len)
	{
		this->send((const char*)data, len, false);
		return true;
	}

	// ������Ϣ(�����������)
	bool SocketClient::sendCmd(const void* cmd, ushort cmdLen)
	{
		if ( !isConnected() )
			return false;

		if (cmdLen > MAX_USER_CMD_SIZE) {
			BLOGE("���͵���Ϣ̫���ˣ�����ʧ��!!!");
			return false;
		}

		if (cmd == NULL || cmdLen == 0)
			return false;

		int error = this->send((const char*)cmd, cmdLen, true);
		if (error != SEND_ERROR && error != cmdLen+HEADER_SIZE) {
			BLOGE("������SocketClient::sendCmd����δ����ȫ %d\n", error);
		}

		return true;
	}

	const char* SocketClient::getIP()
	{
		return mSocket.getIP();
	}

	ushort SocketClient::getPort()
	{
		return mSocket.getPort();
	}

	bool SocketClient::recvThreadFunc()
	{
		FUNC_PF_EXT("���ճ�ʱ", 100);

		if (!mSocket.connected()) {
			return true;
		}

#if 1		
		// �Ż��汾
		// ��100��Ϊ�˷�ֹ���������������߽�����
		int byteReceived = mSocket.recv(mRecvBuff.getWriteBufferPtr(), (int)mRecvBuff.getFreeSize() - 100);
		if (byteReceived > 0) {
			mRecvBuff.offsetCurPos(byteReceived);
			parsePacket();
		} else {
			bool isCloseError = false;
#ifdef WIN32
			int nErr = GetLastError();
			isCloseError = (nErr != WSAEWOULDBLOCK && nErr != WSAENOTCONN);
			if (isCloseError)
				printf("�յ��쳣error=%d\n", nErr);
			if(isCloseError) {
				close();
			}
#else
			isCloseError = (errno != EINTR && errno != EWOULDBLOCK && errno != EAGAIN);
			if (isCloseError)
				printf("�յ��쳣error=%d\n", errno);
			if (byteReceived == 0 || isCloseError) {	// EINTR
				//printf("�Է��Ͽ�����error=%d\n", errno);
				close();
			}
#endif
		}


#else	// aaaaaaaaaaaa
		// �ϵİ汾
		char tmpRecvBuff[MAX_USER_RECV_SIZE];
		int byteReceived = mSocket.recv(tmpRecvBuff, MAX_USER_RECV_SIZE);
		if (byteReceived > 0) {
			mRecvBuff.write(tmpRecvBuff, byteReceived);
			parsePacket();
		} else {
			bool isCloseError = false;
#ifdef WIN32
			int nErr = GetLastError();
			isCloseError = (nErr != WSAEWOULDBLOCK && nErr != WSAENOTCONN);
			if (isCloseError)
				printf("�յ��쳣error=%d\n", nErr);
			if(isCloseError) {
				close();
			}
#else
			isCloseError = (errno != EINTR && errno != EWOULDBLOCK && errno != EAGAIN);
			if (isCloseError)
				printf("�յ��쳣error=%d\n", errno);
			if (byteReceived == 0 || isCloseError) {	// EINTR
				//printf("�Է��Ͽ�����error=%d\n", errno);
				close();
			}
#endif
		}


#endif	// aaaaaaaaaaaa

		return true;
	}

	void SocketClient::parsePacket()
	{
		while(true) {
			ByteBuffer* revBuff = &mRecvBuff;

			// �����ǰ����С��2���ֽڣ�˵�������ݻ�����
			if (revBuff->size() < HEADER_SIZE)
				break;

			// ȡ��������Ϣ
			ushort cmdLen = 0;
			revBuff->get(&cmdLen, sizeof(cmdLen));
			if (cmdLen < HEADER_SIZE) {
				printf("[ϵͳ������]SocketClient��Ч����Ϣ��ʽ\n");
				revBuff->clearBuffer();
				break;
			}

			if (cmdLen > MAX_USER_CMD_SIZE) {
				printf("[ϵͳ������]SocketClient�û��������������ƣ������⵽������\n");
				revBuff->clearBuffer();
				break;
			}

			// ��������ݲ�����������������
			if ((ushort)revBuff->size() < (cmdLen + HEADER_SIZE)) {
				break;
			}

			const byte* pCmdBuff = (const byte*)(revBuff->getBuffer() + HEADER_SIZE);
			stBaseCmd* pCmd = (stBaseCmd*)pCmdBuff;

			// �Ƚ���
			if (pCmd->flag & FLAG_ENCRYPT) {
				Decrypt((byte*)pCmdBuff+CMD_FLAG_SIZE, cmdLen-CMD_FLAG_SIZE);
			}
			
			if (pCmd->flag & FLAG_ZIP) {
				// �ٽ�ѹ
				ulong destLen = -1;
				byte unzipBuffer[MAX_USER_CMD_SIZE*2];
				stBaseCmd* pUnzipCmd = (stBaseCmd*)unzipBuffer;
				uint flagSize = sizeof(pCmd->flag);
				pUnzipCmd->flag = pCmd->flag;
				int ret = uncompress(unzipBuffer+flagSize, &destLen, (const byte*)(pCmdBuff+flagSize), cmdLen-flagSize);
				if(ret != Z_OK) {
					printf("[ϵͳ������]SocketClient��ѹ������ʧ��!!\n");
					break;
				}
				uint unzipCmdSize = destLen + flagSize;

				// �ѽ�����������Ϣ��������Ϣ����
				putMsg(pUnzipCmd, unzipCmdSize);
			} else {

				// �ѽ�����������Ϣ��������Ϣ����
				putMsg(pCmdBuff, cmdLen);
			}

			// ɾ�������������
			revBuff->remove(cmdLen + HEADER_SIZE);
		}
	}

	bool SocketClient::sendThreadFunc()
	{
		if (!mSocket.connected()) {
			return true;
		}

		if (mSendBuff.size() == 0)
			return true;

		mSendMutex.lock();
		int buffSize = (int)mSendBuff.size();
		if (mSocket.connected() && buffSize > 0) {
			int byteSent = mSocket.send(mSendBuff.getBuffer(), buffSize);
			if (byteSent != -1 && byteSent > 0) {
				if (!mSendBuff.remove(byteSent))
					BLOGE("SocketClient::sendThreadFunc���ͻ������");
			}
		}
		mSendMutex.unlock();

		return true;
	}

	void SocketClient::resetEncrypt()
	{
		mEnableEncrypt = false;
		mEncIndex = 0;
		mDecIndex = 0;
	}

	// ����
	void SocketClient::Encrypt(byte* data, uint dataLen)
	{
		uint lastIndex = mEncIndex;
		for(uint i=0; i<dataLen; i++) {
			if(lastIndex >= ENCRYPT_KEY_LEN) {
				lastIndex = 0;
			}

			data[i] ^= EncryptKey[lastIndex];
			lastIndex++;
		}

		nextEncryptKey(mEncIndex);
	}

	// ����
	void SocketClient::Decrypt(byte* data, uint dataLen)
	{
		uint lastIndex = mDecIndex;
		for(uint i=0; i<dataLen; i++) {
			if(lastIndex >= ENCRYPT_KEY_LEN) {
				lastIndex = 0;
			}

			data[i] ^= EncryptKey[lastIndex];
			lastIndex++;
		}

		nextEncryptKey(mDecIndex);

		// �����Ҫ������Ϣ��˵��������Ҳ�����˼��ܹ���
		mEnableEncrypt = true;
	}

	void SocketClient::nextEncryptKey(uint& index)
	{
		index++;
		if (index >= ENCRYPT_KEY_LEN)
			index = 0;
	}


	void SocketClient::printInfo()
	{
		char info[] = 
			"---------------------------------\n"
			"          Socket Client		  \n"
			"���ն����� %d					  \n"
			"���Ͷ����� %d					  \n"
			"��Ϣ������ %d					  \n"
			"---------------------------------\n";
		//printf(info, m_recvBuff.size(), m_sendBuff.size(), m_cmdQueue.size());
	}


	//////////////////////////////////////////////////////////////////////////

	TCPClientThread::TCPClientThread()
		:Thread("TCPClientThread")
	{

	}

	TCPClientThread::~TCPClientThread()
	{

	}

	// ����ȴ�����
	bool TCPClientThread::addWaitList(SocketClient* pClient)
	{
		mWaitAddMutex.lock();
		mWaitAddList.push_back(pClient);
		mWaitAddMutex.unlock();

		return true;
	}

	void TCPClientThread::processAddClient()
	{
		SocketClient* pClient = NULL;
		mWaitAddMutex.lock();
		if (!mWaitAddList.empty()) {
			pClient = mWaitAddList.front();
			mWaitAddList.pop_front();
		}
		mWaitAddMutex.unlock();

		if (pClient == NULL)
			return;

		mClientMap[pClient] = pClient;
	}

	bool TCPClientThread::isFull()
	{
		return (mClientMap.size() >= ONETHREAD_MAXCLIENT);
	}

	//bool TCPClientThread::addTCPClient(SocketClient* client)
	//{
	//	//MutexScopeLock lock(mMutex);
	//	//mCond.timedwait(mMutex, 2000);

	//	if (isFull())
	//		return false;

	//	mClientMap[client] = client;
	//	return true;
	//}

	//bool TCPClientThread::removeTCPClient(SocketClient* client)
	//{
	//	//MutexScopeLock lock(mMutex);
	//	//mCond.timedwait(mMutex, 2000);

	//	TCPClientMapIter it = mClientMap.find(client);
	//	if (it == mClientMap.end())
	//		return false;

	//	mClientMap.erase(it);
	//	return true;
	//}

	bool TCPClientThread::run()
	{
		//mMutex.lock();

		// �ȴ�������¼�
		processAddClient();

		// Ȼ����
		vector<SocketClient*> vecDel;
		for (auto it = mClientMap.begin(); it != mClientMap.end(); it++) {
			SocketClient* client = it->second;

			if (client->isCanDeleteMe()) {
				// ����ɾ������
				vecDel.push_back(client);
			} else {
				// ������������
				client->recvThreadFunc();

				// ������������
				client->sendThreadFunc();
			}
		}

		// ɾ������Ҫ�Ķ���
		for (auto pClient : vecDel) {
			mClientMap.erase(pClient);
			delete pClient;
			BLOGD("delete pClient=0x%08x", pClient);
		}

		//mMutex.unlock();

		if (mClientMap.size() >= 100) {
			// ����̫��ʱ����ͣ�ᣬһ���ڻ�����ѹ��ʱ���Ż����
			sleep(1);
		}

		return true;
	}

	/*
	// ------ ���͵��߳� -----------
	TCPClientSendThread::TCPClientSendThread()
	{

	}

	TCPClientSendThread::~TCPClientSendThread()
	{

	}

	bool TCPClientSendThread::run()
	{
		//mMutex.lock();

		for (TCPClientThread::TCPClientMapIter it = mClientMap.begin(); it != mClientMap.end(); it++) {
			SocketClient* client = it->second;
			if (client == NULL)
				continue;

			client->sendThreadFunc();
		}
	
		//mMutex.unlock();


		if (mClientMap.size() >= 100) {
			// ����̫��ʱ����ͣ�ᣬһ���ڻ�����ѹ��ʱ���Ż����
			sleep(1);
		}

		return true;
	}

	// ------ ���յ��߳� -----------
	TCPClientRecvThread::TCPClientRecvThread()
	{

	}

	TCPClientRecvThread::~TCPClientRecvThread()
	{

	}

	bool TCPClientRecvThread::run()
	{
		//mMutex.lock();

		for (TCPClientThread::TCPClientMapIter it = mClientMap.begin(); it != mClientMap.end(); it++) {
			SocketClient* client = it->second;
			if (client == NULL)
				continue;

			client->recvThreadFunc();
		}

		//mMutex.unlock();

		if (mClientMap.size() >= 100) {
			// ����̫��ʱ����ͣ�ᣬһ���ڻ�����ѹ��ʱ���Ż����
			sleep(1);
		}

		return true;
	}
	*/


	//////////////////////////////////////////////////////////////////////////
	TCPClientThreadPool::TCPClientThreadPool()
	{

	}

	TCPClientThreadPool::~TCPClientThreadPool()
	{
		while (!mClientThreads.empty()) {
			delete mClientThreads.front();
			mClientThreads.pop_front();
		}

		//while (!mClientSendThreads.empty()) {
		//	delete mClientSendThreads.front();
		//	mClientSendThreads.pop_front();
		//}

		//while (!mClientRecvThreads.empty()) {
		//	delete mClientRecvThreads.front();
		//	mClientRecvThreads.pop_front();
		//}
	}

	bool TCPClientThreadPool::addTCPClient(SocketClient* client)
	{
		bool isAdded = false;
		for (auto it=mClientThreads.begin(); it!=mClientThreads.end(); it++) {
			if ( !(*it)->isFull() ) {
				(*it)->addWaitList(client);
				isAdded = true;
				break;
			}
		}

		if (!isAdded) {
			// �������ʧ�ܣ��ʹ���һ�����߳�������
			TCPClientThread* clientThread = new TCPClientThread;
			mClientThreads.push_back(clientThread);
			isAdded = clientThread->addWaitList(client);
			clientThread->start();
		}

		return isAdded;

		/*
		//MutexScopeLock lock(mMutex);
		
		// ----- ��������̳߳� ----------
		bool isRecvAdded = false;
		for (auto it=mClientRecvThreads.begin(); it!=mClientRecvThreads.end(); it++) {
			if ((*it)->addTCPClient(client)) {
				isRecvAdded = true;
				break;
			}
		}

		if (!isRecvAdded) {
			// �������ʧ�ܣ��ʹ���һ�����߳�������
			TCPClientRecvThread* clientThread = new TCPClientRecvThread;
			mClientRecvThreads.push_back(clientThread);
			clientThread->start();
			isRecvAdded = clientThread->addTCPClient(client);
		}


		// ----- �������̳߳� ----------
		bool isSendAdded = false;
		for (auto it=mClientSendThreads.begin(); it!=mClientSendThreads.end(); it++) {
			if ((*it)->addTCPClient(client)) {
				isSendAdded = true;
				break;
			}
		}

		if (!isSendAdded) {
			// �������ʧ�ܣ��ʹ���һ�����߳�������
			TCPClientSendThread* clientThread = new TCPClientSendThread;
			mClientSendThreads.push_back(clientThread);
			clientThread->start();
			isSendAdded = clientThread->addTCPClient(client);
		}

		return isSendAdded && isRecvAdded;
		*/
	}		

	//bool TCPClientThreadPool::removeTCPClient(SocketClient* client)
	//{
	//	bool removed = false;
	//	for (auto it=mClientThreads.begin(); it!=mClientThreads.end(); it++) {
	//		if ((*it)->removeTCPClient(client)) {
	//			removed = true;
	//			break;
	//		}
	//	}

	//	return removed;

	//	/*
	//	bool isRemovedRecv = false;
	//	bool isRemovedSend = false;

	//	// �Ƴ����ն����е�socket
	//	for (auto it=mClientRecvThreads.begin(); it!=mClientRecvThreads.end(); it++) {
	//		if ((*it)->removeTCPClient(client)) {
	//			isRemovedRecv = true;
	//			break;
	//		}
	//	}


	//	// �Ƴ����Ͷ����е�socket
	//	for (auto it=mClientSendThreads.begin(); it!=mClientSendThreads.end(); it++) {
	//		if ((*it)->removeTCPClient(client)) {
	//			isRemovedSend = true;
	//			break;
	//		}
	//	}

	//	return isRemovedRecv && isRemovedSend;
	//	*/
	//}


	void TCPClientThreadPool::stopAllThread()
	{
		//mMutex.lock();

		for (auto it=mClientThreads.begin(); it!=mClientThreads.end(); it++) {
			(*it)->stop();
		}

		//for (auto it=mClientRecvThreads.begin(); it!=mClientRecvThreads.end(); it++) {
		//	(*it)->stop();
		//}

		//for (auto it=mClientSendThreads.begin(); it!=mClientSendThreads.end(); it++) {
		//	(*it)->stop();
		//}

		//mMutex.unlock();
	}

}	// namespace

