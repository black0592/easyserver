#ifndef __NET_SERVICE_H__
#define __NET_SERVICE_H__

#include "BaseTypes.h"
#include "thread/Thread.h"
#include "TCPTask.h"
#include "TCPTaskManager.h"
#include "client/SocketClient.h"
#include "NetDefense.h"
#include "time/Timer.h"
#include "debugger/Profiler.h"
#include "logger/Logger.h"
#include "ServiceConfig.h"


namespace easygame {

	// ������Ϣ���еĶ�ʱ��
	class TimerForNetService : public TimerEvent
	{
	public:
		TimerForNetService(uint elapse) : TimerEvent(elapse, true) {}
		virtual ~TimerForNetService(void) {}

	protected:
		virtual bool onTimer()
		{
			//FUNC_PF_EXT("��Ϣ��ѭ��", 5);
			// ����������յ�����Ϣ����
			TCPTaskManager::getInstance().onTimer();
			NetDefense::getInstance().onTimer();
			
			return true;
		}
	};


	//////////////////////////////////////////////////////////////////////////
	// ���������������ܷ�װ
	template<typename TTCPTask>
	class NetService
	{
	public:
		NetService(const string& name)
			: mName(name)
			, mMainTimer("MainTimer")
		{
#ifndef WIN32
			// linux�£������̨����
			daemon(1, 0);
#endif

#ifdef _DEBUG
			mIsDebug = true;
#else
			mIsDebug = false;
#endif

			// �����̳߳��߳���
			mSvrPtr.threads = 1;

			// ��ʼ��minidump
			MiniDump::InitMiniDump("./crashlog/", mName.c_str());

			// ��ʼ��ȫ����־
			g_Logger.start("./log/serverglobal/serverglobal", mName.c_str(), true);

			mMainTimer.addTimer(new TimerForNetService(0));
		}

		virtual ~NetService(void)
		{
			shutdown();
			ShutdownProtobufLibrary();

			stop();
		}

		// ���붨ʱ���¼�
		void addTimer(TimerEvent* event)
		{
			mMainTimer.addTimer(event);
		}

		// ɾ����ʱ���¼�
		void deleteTimer(TimerEvent*& event)
		{
			mMainTimer.deleteTimer(event);
		}

		virtual bool start(ushort port)
		{
			//BLOGE("�����˿� %u", port);

			mSvrPtr.listener.service = this;

			// ���ü����׽��ְ󶨵�IP
			// 0Ϊ�����б�������IP��ַ
			mSvrPtr.ip = 0;

			// �����˿�
			mSvrPtr.port = port;

			// �����׽������Ӷ��г���
			mSvrPtr.backlogs = 10;

			// �����̳߳��߳���
			//mSvrPtr.threads = Platform::getNumberOfProcessors() * 2;
			//printf("WorkThreads: %d\n", mSvrPtr.threads);

			// �������߳�����ͨ���Ǳ���CPU����2��
			// 0��ʾ�Զ�ѡ��
#ifdef WIN32
			mSvrPtr.scheds = Platform::getNumberOfProcessors() * 2;
#else
			mSvrPtr.scheds = 0;
#endif

			// �����������
			if (!mSvrPtr.start())
				return false;

			// �������̼߳�ʱ��
			mMainTimer.start();

			return true;
		}

		virtual void onInputCmd(const string& cmd) {}

		// ������ѭ��
		virtual void main()
		{
			// ��ʼ��
			initialise();

			string cmd;
			while (true)
			{
#ifdef WIN32
				cout << "������ָ��(exit - �˳�������)��\n";
				cout << ">>";
				getline(cin, cmd);
				if (cmd == "exit") {
					break;
				} else if (cmd == "crash") {
					char* p = NULL;
					*p = 'a';
				} else if (cmd == "info") {
					printf("task: %d\n", TCPTaskManager::getInstance().size());
				} else {
					onInputCmd(cmd);
				}
#else
				Platform::sleep(5000);
#endif
			}

			// �����˳�
			shutdown();
		}

		// ��ȡ�����Լ��Ļ�������
		bool loadConfigBase()
		{

		}

		// ��ʼ��������߼�
		virtual bool initialise() {return true;}

		// �˳�������߼�
		virtual bool shutdown() {return true;}

		virtual void stop()
		{
			ThreadManager::getInstance().stopAllThread();
			//getSingletonRleaser().manualRelease();
			//ThreadManager::destroy();
			mSvrPtr.stop();
		}

		// debug��ʶ������һЩ���Թ��ܵļ��
		bool isDebug() { return mIsDebug; }
		void setDebugFlag(bool debug) { mIsDebug = debug; }

	protected:
		virtual bool onTimer()
		{
			
			return true;
		}

	protected:
		string mName;
		lance::net::TCPSrv<TTCPTask> mSvrPtr;
		Timer mMainTimer;		// ���̼߳�ʱ��
		//bool mIsInitialise;
		bool mIsDebug;			// �Ƿ�debugģʽ
	};



}	// namespace



#endif