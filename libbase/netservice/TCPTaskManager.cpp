#include "TCPTaskManager.h"
#include <assert.h>
#include "debugger/Profiler.h"
#include "logger/Logger.h"
#include "BaseMisc.h"


namespace easygame {

	TCPTaskManager::TCPTaskManager(void)
		: mRecycleTimer(200)
		, mDestroyTimer(500)
		, mPrintInfoTimer(1000)
	{
	}

	TCPTaskManager::~TCPTaskManager(void)
	{
	}

	void TCPTaskManager::destroyAllTaskSafe()
	{
		// 直到所有链接正常退出，才能释放TaskManager
		printf("Server is exiting! Waitting!\n");
		while (!mTaskMap.empty() 
			|| !mWaitAddList.empty() 
			|| !mDestroyList.empty())
		{
			if (mPrintInfoTimer.isTimeout()) {
				// 踢掉所有链接
				kickAllTask();

				// 快速清理（可以快速退出服务器）
				processDestroyTask(mDestroyList.size());

				// 打印信息
				printf("Task %d, WaitAddTask %d, DestroyTask %d\n",
					mTaskMap.size(), mWaitAddList.size(), mDestroyList.size());
			}
			
			onTimer();

			Platform::sleep(1);
		}
	}

	bool TCPTaskManager::addWaitList(TCPTask* task)
	{
		mWaitAddMutex.lock();
		mWaitAddList.push_back(task);
		mWaitAddMutex.unlock();

		return true;
	}

	TCPTask* TCPTaskManager::getTask(int id)
	{
		MutexScopeLock lock(mMapMutex);

		TCPTask* task = NULL;
		auto itFind = mTaskMap.find(id);
		if (itFind != mTaskMap.end()) {
			task = itFind->second;
		}

		return task;
	}

	void TCPTaskManager::processAddTask()
	{
		TCPTask* task = NULL;
		mWaitAddMutex.lock();
		if (!mWaitAddList.empty()) {
			task = mWaitAddList.front();
			mWaitAddList.pop_front();
		}
		mWaitAddMutex.unlock();

		// 当前没有新链接
		if (task == NULL)
			return;

		do 
		{
			// 创建唯一id
			ushort uid = mUniqueID.get();
			if (uid == mUniqueID.invalid()) {
				BLOGE("人数已满，不能在加入task管理器 uid=%d", uid);
				break;
			}

			// 设置唯一id
			task->setID(uid);

			// 检查是否已存在
			if (mTaskMap.find(uid) != mTaskMap.end()) {
				// 肯定id重复了，理论上不应该出现滴
				//assert(false);
				BLOGE("uid重复了，理论上不应该出现滴 uid=%d", uid);
				//TRACE( strformat("[id重复] clt at 0x%08x\n", task) );
				break;
			}

			mMapMutex.lock();
			mTaskMap[uid] = task;
			mMapMutex.unlock();

			// 开始接受数据
			task->OnConnect();

			// 加入成功
			return;
		} while (0);

		// 删除task
		//TRACE( strformat("[processAddTask] clt at 0x%08x\n", task) );
		BLOGW( "[processAddTask] clt at 0x%08x\n", task );
		delete task;
	}

	void TCPTaskManager::processRecycleTask()
	{
		int64 curTick = Platform::tick();
		vector<ushort> taskIdList;
		TCPTask* task = NULL;
		for (auto it=mTaskMap.begin(); it!=mTaskMap.end(); it++) {
			task = it->second;

			if (task->delayCloseTime > 0 && task->isEventEmpty()) {
				// 删除进入关闭倒计时的链接
				if (curTick > task->delayCloseTime) {
					taskIdList.push_back(task->getID());
				}
			} else {
#ifndef _DEBUG	// 调试模式下，不开此功能
				// 关闭很久没发消息的链接(5分钟内都没反应的话，就踢掉)
				if (curTick - task->getLastRecvTime() > 5 * 60 * 1000) {
					task->closeDelay();
					BLOGW("task在规定时间内，没有发消息，踢掉！！！ taskid=%u, ip=%s", task->getID(), task->getIP());
				}
#endif
			}
		}


		// 删除已经超时的链接对象
		for (size_t i=0; i<taskIdList.size(); i++) {
			ushort taskId = taskIdList[i];
			auto itFind = mTaskMap.find(taskId);
			if (itFind != mTaskMap.end()) {
				task = itFind->second;
				task->OnDisconnect();
				mUniqueID.put(taskId);	// 回收id

				mMapMutex.lock();
				mTaskMap.erase(itFind);
				mMapMutex.unlock();

				// 加入销毁队列
				task->setLastDestroyTime(curTick);
				mDestroyList.push_back(task);
			}
		}
	}

	void TCPTaskManager::processDestroyTask(int count)
	{
		if (mDestroyList.empty())
			return;

		for (int i=0; i<count; i++) {
			int64 curTick = Platform::tick();
			TCPTask* task = mDestroyList.front();
			if (curTick - task->getLastDestroyTime() > 10 * 1000) {
				//BLOGW( "[processDestroyTask] clt at 0x%08x\n", task );
				delete task;
				mDestroyList.pop_front();
			}
		}
	}


	bool TCPTaskManager::kickTask(int id)
	{
		MutexScopeLock lock(mMapMutex);

		TCPTask* task = NULL;
		auto itFind = mTaskMap.find(id);
		if (itFind != mTaskMap.end()) {
			task = itFind->second;
			task->closeDelay();
			return true;
		}

		return false;
	}

	bool TCPTaskManager::kickAllTask()
	{
		MutexScopeLock lock(mMapMutex);

		TCPTask* task = NULL;
		for (auto it=mTaskMap.begin(); it!=mTaskMap.end(); it++) {
			task = it->second;
			task->closeDelay();
		}

		return false;
	}

	bool TCPTaskManager::doMsgQueue()
	{
		FUNC_PF_EXT("TCPTaskManager::doMsgQueue", 100);

		if (mTaskMap.empty())
			return false;

		// 先检测，节省循环的开销(doMsgQueue只在消息队列任务中使用)
		auto it = mTaskMap.begin();
		TCPTask* task = it->second;
		if (task->getType() != SyncType)
			return false;
		
		// 如果是消息队列任务，才执行
		MessageHandlerQueue* msgQueue;
		for (it=mTaskMap.begin(); it!=mTaskMap.end(); it++) {
			//msgQueue = (MessageHandlerQueue*)it->second;
			msgQueue = dynamic_cast<MessageHandlerQueue *>(it->second);
#ifdef _DEBUG 
			MessageHandlerQueue* msgQueueCheck = dynamic_cast<MessageHandlerQueue *>(it->second);
			assert(msgQueueCheck != NULL);
			msgQueueCheck = msgQueueCheck;
#endif
			msgQueue->doMsgQueue();
		}

		return false;
	}

	bool TCPTaskManager::sendCmd(int id, void* cmd, uint cmdLen)
	{
		MutexScopeLock lock(mMapMutex);

		TCPTask* task = NULL;
		auto itFind = mTaskMap.find(id);
		if (itFind != mTaskMap.end()) {
			task = itFind->second;
			task->sendCmd(cmd, cmdLen);
			return true;
		}

		return false;
	}

	bool TCPTaskManager::sendRawData(int id, void* data, uint dataLen)
	{
		MutexScopeLock lock(mMapMutex);

		TCPTask* task = NULL;
		auto itFind = mTaskMap.find(id);
		if (itFind != mTaskMap.end()) {
			task = itFind->second;
			task->sendRawData(data, dataLen);
			return true;
		}

		return false;
	}

	void TCPTaskManager::broadcastCmd(void* cmd, uint cmdLen)
	{
		MutexScopeLock lock(mMapMutex);

		TCPTask* task = NULL;
		for (auto it=mTaskMap.begin(); it!=mTaskMap.end(); it++) {
			task = it->second;
			task->sendCmd(cmd, cmdLen);
		}
	}

	void TCPTaskManager::broadcastRawData(void* data, uint dataLen)
	{
		MutexScopeLock lock(mMapMutex);

		TCPTask* task = NULL;
		for (auto it=mTaskMap.begin(); it!=mTaskMap.end(); it++) {
			task = it->second;
			task->sendRawData(data, dataLen);
		}
	}

	void TCPTaskManager::broadcastCmd(vector<int> taskIdList, void* cmd, uint cmdLen)
	{
		MutexScopeLock lock(mMapMutex);

		TCPTask* task = NULL;
		for (size_t i=0; i<taskIdList.size(); i++) {
			auto itFind = mTaskMap.find(taskIdList[i]);
			if (itFind != mTaskMap.end()) {
				task = itFind->second;
				task->sendCmd(cmd, cmdLen);
			}
		}
	}

	void TCPTaskManager::broadcastRawData(vector<int> taskIdList, void* data, uint dataLen)
	{
		MutexScopeLock lock(mMapMutex);

		TCPTask* task = NULL;
		for (size_t i=0; i<taskIdList.size(); i++) {
			auto itFind = mTaskMap.find(taskIdList[i]);
			if (itFind != mTaskMap.end()) {
				task = itFind->second;
				task->sendRawData(data, dataLen);
			}
		}
	}

	uint TCPTaskManager::size()
	{
		MutexScopeLock lock(mMapMutex);

		return (uint)mTaskMap.size();
	}

	uint TCPTaskManager::getDestroyListSize()
	{
		return (uint)mDestroyList.size();
	}

	void TCPTaskManager::printInfo()
	{
		//MutexScopeLock lock(mMapMutex);

		//int totalSize = 0;
		//TCPTask* task = NULL;
		//for (auto it=mTaskMap.begin(); it!=mTaskMap.end(); it++) {
		//	task = it->second;
		//	totalSize += 0;//task->getSendBuffSize();
		//}

		//char info[] = 
		//	"---------------------------------\n"
		//	"          TCPTaskManager		  \n"
		//	"连接数 %d						  \n"
		//	"总发送数据长度 %d K			  \n"
		//	"---------------------------------\n";
		//printf(info, mTaskMap.size(), totalSize/1024);
	}

	void TCPTaskManager::onTimer()
	{
		// 删除过期的任务
		if (mRecycleTimer.isTimeout()) {
			processRecycleTask();
		}

		// 处理新链接
		processAddTask();

		// 再处理消息
		doMsgQueue();

		// 删除过期的task，释放内存
		if (mDestroyTimer.isTimeout()) {
			processDestroyTask(1);
		}
	}


}	// namespace
