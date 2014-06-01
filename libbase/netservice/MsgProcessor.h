#ifndef __MSGPROCESSOR_H__
#define __MSGPROCESSOR_H__

#include "BaseTypes.h"
#include <queue>

#define USE_STL_QUEUE	// 是否使用stl队列


namespace easygame {

#ifdef USE_STL_QUEUE
	// 消息的结构体
	struct stCmd
	{
		byte* cmd;
		int size;
	};
#else
	// 对象池
	template <class T>
	class MemPool
	{
		T memPool[2046];
	public:
		char * allocate(size_t  len)
		{
			return (char*)::malloc(len);
		}

		void deallocate(byte* ptr,size_t len)
		{
			::free(ptr);
		}
	};

	// 无锁消息队列
	typedef std::pair<uint,byte *> CmdPair;
	template <int QueueSize=102400>
	//template <int QueueSize=5120>
	class MsgQueue
	{
	public:
		//typedef std::pair<volatile bool,CmdPair > CmdQueue;
		typedef std::pair<bool,CmdPair > CmdQueue;

	public:
		MsgQueue()
			:queueRead(0),
			queueWrite(0)
		{ }

		~MsgQueue() { clear(); }

		CmdPair *front()
		{
			CmdPair *ret=NULL;
			if (cmdQueue[queueRead].first)
			{
				ret=&cmdQueue[queueRead].second;
			}
			return ret;
		}

		void pop()
		{
			mMemPool.deallocate(cmdQueue[queueRead].second.second,cmdQueue[queueRead].second.first);
			cmdQueue[queueRead].first=false;
			queueRead = (++queueRead)%QueueSize;
		}

		bool push(const void* cmd, uint cmdLen)
		{
			byte *buf = (byte*)mMemPool.allocate(cmdLen);
			if (buf)
			{
				memcpy(buf,cmd,cmdLen);
				if (!putQueueToArray() && !cmdQueue[queueWrite].first)
				{
					cmdQueue[queueWrite].second.first = cmdLen;
					cmdQueue[queueWrite].second.second = buf;
					cmdQueue[queueWrite].first=true;
					queueWrite = (++queueWrite)%QueueSize;
					return true;
				}
				else
				{ 
					queueCmd.push(std::make_pair(cmdLen,buf));
				}
				return true;
			}
			return false;
		}

	private:
		void clear()
		{
			while(putQueueToArray())
			{
				while(front())
				{
					pop();
				}
			}
			while(front())
			{
				pop();
			}
		}
		bool putQueueToArray()
		{
			bool isLeft=false;
			while(!queueCmd.empty())
			{
				if (!cmdQueue[queueWrite].first)
				{
					cmdQueue[queueWrite].second = queueCmd.front();;
					cmdQueue[queueWrite].first=true;
					queueWrite = (++queueWrite)%QueueSize;
					queueCmd.pop();
				}
				else
				{
					isLeft = true; 
					break;
				}
			}
			return isLeft;
		}
		MemPool<byte> mMemPool;
		CmdQueue cmdQueue[QueueSize];
		std::queue<CmdPair> queueCmd;
		uint queueWrite;
		uint queueRead;
	};
#endif

	//消息队列 
	class MessageHandlerQueue
	{
	public:	
		MessageHandlerQueue() {};
		virtual ~MessageHandlerQueue() {};

		size_t getMsgCount()
		{
#ifdef USE_STL_QUEUE
			return mCmdQueue.size();
#else
			return 0;
#endif

		}

		bool putMsg(const void* cmd, int cmdLen) 
		{ 
#ifdef USE_STL_QUEUE
			stCmd tCmd;
			tCmd.cmd = new byte[cmdLen];
			memcpy(tCmd.cmd, cmd, cmdLen);
			tCmd.size = cmdLen;

			MutexScopeLock scopeLock(mLock);

			mCmdQueue.push(tCmd);

			return true;
#else
			return mCmdQueue.push(cmd,cmdLen);
#endif
		}

		virtual bool doMsgQueue()
		{
#ifdef USE_STL_QUEUE
			MutexScopeLock scopeLock(mLock);
			
			while (!mCmdQueue.empty()) {
				stCmd &tCmd = mCmdQueue.front();
				handleMessage(tCmd.cmd, tCmd.size);
				delete[] tCmd.cmd;
				mCmdQueue.pop();
			}

			return true;
#else
			CmdPair* cmd = mCmdQueue.front();
			while(cmd) {
				handleMessage(cmd->second,cmd->first);
				mCmdQueue.pop();
				cmd = mCmdQueue.front();
			}

			return true;
#endif
		}

		// 消息具体的处理函数,上层逻辑使用(主线程)
		virtual bool handleMessage(const void *cmd, int cmdLen) = 0;

	private:
#ifdef USE_STL_QUEUE
		std::queue<stCmd> mCmdQueue;
		Mutex mLock;
#else
		MsgQueue<> mCmdQueue;
#endif
	};

}	// namespace

#endif