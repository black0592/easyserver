#include "TCPTaskManager.h"
#include <assert.h>
#include "debugger/Profiler.h"
#include "logger/Logger.h"
#include "BaseMisc.h"


namespace easygame {

	TCPTaskManager::TCPTaskManager(void)
		: mRecycleTimer(200)
		, mDestroyTimer(500)
	{
	}

	TCPTaskManager::~TCPTaskManager(void)
	{

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
		TaskMapItr itFind = mTaskMap.find(id);
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

		// ��ǰû��������
		if (task == NULL)
			return;

		do 
		{
			// ����Ψһid
			ushort uid = mUniqueID.get();
			if (uid == mUniqueID.invalid()) {
				BLOGE("���������������ڼ���task������ uid=%d", uid);
				break;
			}

			// ����Ψһid
			task->setID(uid);

			// ����Ƿ��Ѵ���
			if (mTaskMap.find(uid) != mTaskMap.end()) {
				// �϶�id�ظ��ˣ������ϲ�Ӧ�ó��ֵ�
				//assert(false);
				BLOGE("uid�ظ��ˣ������ϲ�Ӧ�ó��ֵ� uid=%d", uid);
				//TRACE( strformat("[id�ظ�] clt at 0x%08x\n", task) );
				break;
			}

			mMapMutex.lock();
			mTaskMap[uid] = task;
			mMapMutex.unlock();

			// ��ʼ��������
			task->OnConnect();

			// ����ɹ�
			return;
		} while (0);

		// ɾ��task
		//TRACE( strformat("[processAddTask] clt at 0x%08x\n", task) );
		BLOGW( "[processAddTask] clt at 0x%08x\n", task );
		delete task;
	}

	void TCPTaskManager::processRecycleTask()
	{
		int64 curTick = Platform::tick();
		vector<ushort> taskIdList;
		TCPTask* task = NULL;
		for (TaskMapItr it=mTaskMap.begin(); it!=mTaskMap.end(); it++) {
			task = it->second;

			if (task->delayCloseTime > 0 && task->isEventEmpty()) {
				// ɾ������رյ���ʱ������
				if (curTick > task->delayCloseTime) {
					taskIdList.push_back(task->getID());
				}
			} else {
#ifndef _DEBUG	// ����ģʽ�£������˹���
				// �رպܾ�û����Ϣ������(5�����ڶ�û��Ӧ�Ļ������ߵ�)
				if (curTick - task->getLastRecvTime() > 5 * 60 * 1000) {
					task->closeDelay();
					BLOGW("task�ڹ涨ʱ���ڣ�û�з���Ϣ���ߵ������� taskid=%u, ip=%s", task->getID(), task->getIP());
				}
#endif
			}
		}


		// ɾ���Ѿ���ʱ�����Ӷ���
		for (size_t i=0; i<taskIdList.size(); i++) {
			ushort taskId = taskIdList[i];
			auto itFind = mTaskMap.find(taskId);
			if (itFind != mTaskMap.end()) {
				task = itFind->second;
				task->OnDisconnect();
				mUniqueID.put(taskId);	// ����id

				mMapMutex.lock();
				mTaskMap.erase(itFind);
				mMapMutex.unlock();

				// �������ٶ���
				task->setLastDestroyTime(curTick);
				mDestroyList.push_back(task);
			}
		}
	}

	void TCPTaskManager::processDestroyTask()
	{
		if (mDestroyList.empty())
			return;

		int64 curTick = Platform::tick();
		TCPTask* task = mDestroyList.front();
		if (curTick - task->getLastDestroyTime() > 10 * 1000) {
			//TRACE( strformat("[processDestroyTask] clt at 0x%08x\n", task) );
			BLOGW( "[processDestroyTask] clt at 0x%08x\n", task );
			delete task;
			mDestroyList.pop_front();
		}
	}


	bool TCPTaskManager::kickTask(int id)
	{
		MutexScopeLock lock(mMapMutex);

		TCPTask* task = NULL;
		TaskMapItr itFind = mTaskMap.find(id);
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
		for (TaskMapItr it=mTaskMap.begin(); it!=mTaskMap.end(); it++) {
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

		// �ȼ�⣬��ʡѭ���Ŀ���(doMsgQueueֻ����Ϣ����������ʹ��)
		TaskMapItr it = mTaskMap.begin();
		TCPTask* task = it->second;
		if (task->getType() != SyncType)
			return false;
		
		// �������Ϣ�������񣬲�ִ��
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
		TaskMapItr itFind = mTaskMap.find(id);
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
		TaskMapItr itFind = mTaskMap.find(id);
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
		for (TaskMapItr it=mTaskMap.begin(); it!=mTaskMap.end(); it++) {
			task = it->second;
			task->sendCmd(cmd, cmdLen);
		}
	}

	void TCPTaskManager::broadcastRawData(void* data, uint dataLen)
	{
		MutexScopeLock lock(mMapMutex);

		TCPTask* task = NULL;
		for (TaskMapItr it=mTaskMap.begin(); it!=mTaskMap.end(); it++) {
			task = it->second;
			task->sendRawData(data, dataLen);
		}
	}

	void TCPTaskManager::broadcastCmd(vector<int> taskIdList, void* cmd, uint cmdLen)
	{
		MutexScopeLock lock(mMapMutex);

		TCPTask* task = NULL;
		for (size_t i=0; i<taskIdList.size(); i++) {
			TaskMapItr itFind = mTaskMap.find(taskIdList[i]);
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
			TaskMapItr itFind = mTaskMap.find(taskIdList[i]);
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
		//for (TaskMapItr it=mTaskMap.begin(); it!=mTaskMap.end(); it++) {
		//	task = it->second;
		//	totalSize += 0;//task->getSendBuffSize();
		//}

		//char info[] = 
		//	"---------------------------------\n"
		//	"          TCPTaskManager		  \n"
		//	"������ %d						  \n"
		//	"�ܷ������ݳ��� %d K			  \n"
		//	"---------------------------------\n";
		//printf(info, mTaskMap.size(), totalSize/1024);
	}

	void TCPTaskManager::onTimer()
	{
		// ɾ�����ڵ�����
		if (mRecycleTimer.isTimeout()) {
			processRecycleTask();
		}

		// ����������
		processAddTask();

		// �ٴ�����Ϣ
		doMsgQueue();

		// ɾ�����ڵ�task���ͷ��ڴ�
		if (mDestroyTimer.isTimeout()) {
			processDestroyTask();
		}
	}


}	// namespace
