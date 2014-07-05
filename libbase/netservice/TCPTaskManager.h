#ifndef __TCPTASK_MANAGER_H__
#define __TCPTASK_MANAGER_H__

#include "thread/Thread.h"
#include "misc/Singleton.h"
#include "misc/UniqueID.h"
#include "time/Timer.h"
#include "TCPTask.h"
#include <map>
#include <list>
using namespace std;

namespace easygame {


	class TCPTaskManager : public Singleton<TCPTaskManager>
	{
	public:
		TCPTaskManager(void);
		virtual ~TCPTaskManager(void);

		// 安全的关闭所有链接(Service退出前调用这个函数)
		void destroyAllTaskSafe();

		// 加入等待队列
		bool addWaitList(TCPTask* task);

		TCPTask* getTask(int id);

		bool kickTask(int id);

		bool kickAllTask();

		// 这个函数也在主循环里调用，不用加锁
		bool doMsgQueue();

		bool sendCmd(int id, void* cmd, uint cmdLen);
		bool sendRawData(int id, void* data, uint dataLen);

		void broadcastCmd(void* cmd, uint cmdLen);
		void broadcastRawData(void* data, uint dataLen);

		void broadcastCmd(vector<int> taskIdList, void* cmd, uint cmdLen);
		void broadcastRawData(vector<int> taskIdList, void* data, uint dataLen);

		uint size();
		uint getDestroyListSize();

		void onTimer();

		void printInfo();

	protected:
		// ----- 因为添加和回收链接都在主线程里，所以不用加锁

		// 处理新链接
		void processAddTask();

		// 回收死掉的链接
		void processRecycleTask();

		// 删除断开的链接
		// count - 一次调用最多能清理几个Task
		void processDestroyTask(int count);

	private:
		UniqueID16 mUniqueID;

		// 链接管理器，增加一个队列，减少线程锁
		Mutex mWaitAddMutex;
		list<TCPTask*> mWaitAddList;

		// 保存在线链接
		ElapseTimer mRecycleTimer;
		Mutex mMapMutex;
		map<int,TCPTask*> mTaskMap;
	
		// 链接管理器，增加一个删除对象队列，防止IOCP还在操作
		ElapseTimer mDestroyTimer;
		list<TCPTask*> mDestroyList;

		// 输出调试信息的时间
		ElapseTimer mPrintInfoTimer;
	};


}	// namespace



#endif