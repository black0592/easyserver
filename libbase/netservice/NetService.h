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

	// 处理消息队列的定时器
	class TimerForNetService : public TimerEvent
	{
	public:
		TimerForNetService(uint elapse) : TimerEvent(elapse, true) {}
		virtual ~TimerForNetService(void) {}

	protected:
		virtual bool onTimer()
		{
			//FUNC_PF_EXT("消息主循环", 5);
			// 处理服务器收到的消息队列
			TCPTaskManager::getInstance().onTimer();
			NetDefense::getInstance().onTimer();
			
			return true;
		}
	};


	//////////////////////////////////////////////////////////////////////////
	// 网络服务器基础框架封装
	template<typename TTCPTask>
	class NetService
	{
	public:
		NetService(const string& name)
			: mName(name)
			, mMainTimer("MainTimer")
		{
#ifndef WIN32
			// linux下，进入后台运行
			daemon(1, 0);
#endif

#ifdef _DEBUG
			mIsDebug = true;
#else
			mIsDebug = false;
#endif

			// 处理线程池线程数
			mSvrPtr.threads = 1;

			// 初始化minidump
			MiniDump::InitMiniDump("./crashlog/", mName.c_str());

			// 初始化全局日志
			g_Logger.start("./log/serverglobal/serverglobal", mName.c_str(), true);

			mMainTimer.addTimer(new TimerForNetService(0));
		}

		virtual ~NetService(void)
		{
			shutdown();
			ShutdownProtobufLibrary();

			stop();
		}

		// 加入定时器事件
		void addTimer(TimerEvent* event)
		{
			mMainTimer.addTimer(event);
		}

		// 删除定时器事件
		void deleteTimer(TimerEvent*& event)
		{
			mMainTimer.deleteTimer(event);
		}

		virtual bool start(ushort port)
		{
			//BLOGE("监听端口 %u", port);

			mSvrPtr.listener.service = this;

			// 设置监听套接字绑定的IP
			// 0为绑定所有本机可用IP地址
			mSvrPtr.ip = 0;

			// 监听端口
			mSvrPtr.port = port;

			// 监听套接字连接队列长度
			mSvrPtr.backlogs = 10;

			// 处理线程池线程数
			//mSvrPtr.threads = Platform::getNumberOfProcessors() * 2;
			//printf("WorkThreads: %d\n", mSvrPtr.threads);

			// 调度器线程数，通常是本机CPU数的2倍
			// 0表示自动选择
#ifdef WIN32
			mSvrPtr.scheds = Platform::getNumberOfProcessors() * 2;
#else
			mSvrPtr.scheds = 0;
#endif

			// 启动网络服务
			if (!mSvrPtr.start())
				return false;

			// 开启主线程计时器
			mMainTimer.start();

			return true;
		}

		virtual void onInputCmd(const string& cmd) {}

		// 启动主循环
		virtual void main()
		{
			// 初始化
			initialise();

			string cmd;
			while (true)
			{
#ifdef WIN32
				cout << "请输入指令(exit - 退出服务器)：\n";
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

			// 正常退出
			shutdown();
		}

		// 读取服务自己的基础配置
		bool loadConfigBase()
		{

		}

		// 初始化服务的逻辑
		virtual bool initialise() {return true;}

		// 退出服务的逻辑
		virtual bool shutdown() {return true;}

		virtual void stop()
		{
			ThreadManager::getInstance().stopAllThread();
			//getSingletonRleaser().manualRelease();
			//ThreadManager::destroy();
			mSvrPtr.stop();
		}

		// debug标识，用于一些测试功能的检测
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
		Timer mMainTimer;		// 主线程计时器
		//bool mIsInitialise;
		bool mIsDebug;			// 是否debug模式
	};



}	// namespace



#endif