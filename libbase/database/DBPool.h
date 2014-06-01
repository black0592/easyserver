#ifndef __DBPOOL_H__
#define __DBPOOL_H__

#include "BaseConfig.h"
#include "BaseLib.h"

#if ENABLE_DATABASE

#include "DBHandle.h"
#include <map>
#include <list>
using namespace std;

namespace easygame {

	class DBPool; 


	class AutoDBHandle
	{
	public:
		AutoDBHandle(DBPool& dbPool);
		virtual ~AutoDBHandle();

		DBHandle* handle();

	private:
		DBPool& m_dbPool;
		DBHandle* m_handle;
	};



	class DBPool
	{
	public:
		typedef map<DBHandle*,DBHandle*> HandleMap;
		typedef map<DBHandle*,DBHandle*>::iterator HandleMapIt;

		typedef list<DBHandle*> HandleList;
		typedef list<DBHandle*>::iterator HandleListIt;

	public:
		DBPool(int maxHandle = 10);
		virtual ~DBPool(void);

		void close();

		bool connect(const char* host, const char* user, const char* passwd, const char *db, ushort port = 3306, bool bUTF8 = true);

		DBHandle* getHandle();

		bool putHandle(DBHandle* handle);

		// 打印调试信息
		void printDebugInfo();


	private:

		//easygame::Mutex mMutexFree;
		//HandleMap m_freeHandleMap;		// 不使用的连接

		//easygame::Mutex mMutexBusy;
		//HandleMap m_busyHandleMap;		// 使用中的连接

		easygame::Mutex mMutex;
		HandleList mHandleList;


		int m_maxHandel;
	};



}	//namespace


#endif	// ENABLE_DATABASE


#endif

