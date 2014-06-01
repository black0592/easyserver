#include "DBPool.h"
#include <stdio.h>
#include "logger/Logger.h"

#pragma warning(disable:4311)


#if	ENABLE_DATABASE

namespace easygame {


	AutoDBHandle::AutoDBHandle(DBPool& dbPool)
		: m_dbPool(dbPool)
	{
		m_handle = m_dbPool.getHandle();
	}

	AutoDBHandle::~AutoDBHandle()
	{
		m_dbPool.putHandle(m_handle);
	}

	DBHandle* AutoDBHandle::handle()
	{
		return m_handle;
	}

	//////////////////////////////////////////////////////////////////////////


	DBPool::DBPool(int maxHandle)
	{
		m_maxHandel = maxHandle;
	}

	DBPool::~DBPool(void)
	{
		close();
	}

	void DBPool::close()
	{
		//HandleMapIt it;
		//mMutexFree.lock();
		//while(!m_freeHandleMap.empty()) {
		//	it = m_freeHandleMap.begin();
		//	it->second->close();
		//	delete it->second;
		//	m_freeHandleMap.erase(it);
		//}
		//mMutexFree.unlock();

		//mMutexBusy.lock();
		//while(!m_busyHandleMap.empty()) {
		//	it = m_busyHandleMap.begin();
		//	it->second->close();
		//	delete it->second;
		//	m_busyHandleMap.erase(it);
		//}
		//mMutexBusy.unlock();
	}

	bool DBPool::connect(const char* host, const char* user, const char* passwd, const char *db, ushort port, bool bUTF8)
	{
		//size_t busyCount = m_busyHandleMap.size();
		//if (busyCount > 0) {
		//	BLOGE("%s","有数据库句柄没有回收，无法进行新的连接");
		//	return false;
		//}

		close();

		for (int i=0; i<m_maxHandel; i++) {
			DBHandle* handle = new DBHandle;
			if ( !handle->connect(host,user,passwd,db,port,bUTF8) ) {
				delete handle;
				return false;
			}

//#ifdef _DEBUG
//			mMutexFree.lock();
//			HandleMapIt itFind = m_freeHandleMap.find(id);
//			if (itFind != m_freeHandleMap.end()) {
//				BLOGE("%s","连接数据库出现错误，内存中已存在此对象");
//			}
//			mMutexFree.unlock();
//#endif
			//m_freeHandleMap[handle] = handle;

			mMutex.lock();
			mHandleList.push_back(handle);
			mMutex.unlock();
		}

		return true;
	}

	DBHandle* DBPool::getHandle()
	{
		DBHandle* handle = NULL;

		mMutex.lock();
		if (!mHandleList.empty()) {
			handle = mHandleList.front();
			mHandleList.pop_front();
		}
		mMutex.unlock();

		return handle;

//		DBHandle* handle = NULL;
//
//		mMutexFree.lock();
//
//		if (!m_freeHandleMap.empty()) {
//			HandleMapIt it = m_freeHandleMap.begin();
//			handle = it->second;
//			handle->ping();
//			m_freeHandleMap.erase(it);
//		}
//		
//		mMutexFree.unlock();
//
////#ifdef _DEBUG
////		mMutexBusy.lock();
////		HandleMapIt itFind2 = m_busyHandleMap.find(id);
////		if (itFind2 != m_busyHandleMap.end()) {
////			BLOGE("%s","获得数据库句柄出现错误，此对象已存在");
////		}
////		mMutexBusy.unlock();
////#endif
//
//		
//
//		//mMutexBusy.lock();
//		//m_busyHandleMap[id] = handle;
//		//mMutexBusy.unlock();
//
//		return handle;
	}

	bool DBPool::putHandle(DBHandle* handle)
	{
		mMutex.lock();
		mHandleList.push_back(handle);
		mMutex.unlock();

		//mMutexFree.lock();
		//m_freeHandleMap[handle] = handle;
		//mMutexFree.unlock();

//		ulong id = (ulong)handle;
//		HandleMapIt itFind = m_busyHandleMap.find(id);
//		if (itFind == m_busyHandleMap.end())
//			return false;
//
//#ifdef _DEBUG
//		mMutexFree.lock();
//		HandleMapIt itFind2 = m_freeHandleMap.find(id);
//		if (itFind2 != m_freeHandleMap.end()) {
//			BLOGE("%s","回收数据库句柄出现错误，此对象已存在");
//		}
//		mMutexFree.unlock();
//#endif
//
//		mMutexBusy.lock();
//		m_busyHandleMap.erase(itFind);
//		m_freeHandleMap[id] = handle;
//		mMutexBusy.unlock();

		return true;
	}

	void DBPool::printDebugInfo()
	{
		//printf("[DBPool] free: %u, busy: %u\n", m_freeHandleMap.size(), m_busyHandleMap.size());
	}


}	// namespace


#endif // ENABLE_DATABASE