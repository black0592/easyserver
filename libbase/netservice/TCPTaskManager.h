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

		// ��ȫ�Ĺر���������(Service�˳�ǰ�����������)
		void destroyAllTaskSafe();

		// ����ȴ�����
		bool addWaitList(TCPTask* task);

		TCPTask* getTask(int id);

		bool kickTask(int id);

		bool kickAllTask();

		// �������Ҳ����ѭ������ã����ü���
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
		// ----- ��Ϊ��Ӻͻ������Ӷ������߳�����Բ��ü���

		// ����������
		void processAddTask();

		// ��������������
		void processRecycleTask();

		// ɾ���Ͽ�������
		// count - һ�ε��������������Task
		void processDestroyTask(int count);

	private:
		UniqueID16 mUniqueID;

		// ���ӹ�����������һ�����У������߳���
		Mutex mWaitAddMutex;
		list<TCPTask*> mWaitAddList;

		// ������������
		ElapseTimer mRecycleTimer;
		Mutex mMapMutex;
		map<int,TCPTask*> mTaskMap;
	
		// ���ӹ�����������һ��ɾ��������У���ֹIOCP���ڲ���
		ElapseTimer mDestroyTimer;
		list<TCPTask*> mDestroyList;

		// ���������Ϣ��ʱ��
		ElapseTimer mPrintInfoTimer;
	};


}	// namespace



#endif