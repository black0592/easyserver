#ifndef __UNIQUEID_H__
#define __UNIQUEID_H__

#include "Noncopyable.h"
#include "thread/Mutex.h"
#include "Rand.h"
#include <set>

namespace easygame {

	//UniqueID模板
	//本模板实现了唯一ID生成器，并保证线程安全，可以用各种长度的无符号整数作为ID

	template <class T>
	class UniqueID : private Noncopyable
	{
	private:
		Mutex mutex;
		std::set<T>	ids;	//用于存放回收的的ID
		T minID;
		T maxID;	
		T curMaxID;

	public:
		UniqueID()
		{
			init(1, (T)-1);
		}

		UniqueID(T startID)
		{
			init(startID, (T)-1);
		}

		UniqueID(T startID, T endID)
		{
			init(startID, endID);
		}

		virtual ~UniqueID()
		{
			mutex.lock();
			ids.clear();
			mutex.unlock();
		}

		void init(T min, T max)
		{
			minID = min;
			maxID = max;
			curMaxID = minID;
		}

		//获取最大无效ID
		T invalid()
		{
			return maxID;
		}

		//测试一个ID是否已经被分配出去
		bool hasAssigned(T testid)
		{
			mutex.lock();

			if (testid < curMaxID && testid >= minID) {
				typename std::set<T >::iterator iter = ids.find(testid);
				if (iter != ids.end()) {
					mutex.unlock();
					return false;
				}
				mutex.unlock();
				return true;
			}

			mutex.unlock();
			return false;
		}

		//获取一个唯一ID，如果返回的ID与invalid()相等，表示所有的ID都已经分配出去了，无法再分配
		T get()
		{
			T ret;
			mutex.lock();
			if (maxID > curMaxID) {
				ret = curMaxID;
				++curMaxID;
			} else
				ret = maxID;

			if (ret == maxID && !ids.empty()) {
				uint index = (uint)Rand::randRange(0, (int)ids.size() - 1);
				typename std::set<T >::iterator iter = ids.begin();
				std::advance(iter, index);
				ret = *iter;
				ids.erase(iter);
			}

			mutex.unlock();
			return ret;
		}


		//将ID放回ID池，以便下次重新分配
		//放回的ID，必须是由get函数获取的，并且不保证放回的ID，没有被其他线程使用
		//用户需要自己保证还在使用的ID不会被放回，以免出现ID重复
		void put(T id)
		{
			mutex.lock();
			if (id < maxID && id >= minID) {
				typename std::set<T >::iterator iter = ids.find(id);
				if (iter == ids.end())
					ids.insert(id);
			}
			mutex.unlock();
		}

		//获取多个连续的ID，如果返回值与invalid()相等，表示所有的ID都已经分配出去了，无法再分配
		//param size	需要获取的ID数量
		//param count	实际获得的ID数量
		//return		返回连续的ID中，最小的那个
		T get(int size, int& count)
		{
			T ret;
			mutex.lock();
			if (maxID > curMaxID) {
				count = (maxID - curMaxID) > size ? size : (maxID - curMaxID);
				ret = curMaxID;
				curMaxID += count;
			} else {
				count = 0;
				ret = maxID;
			}
			mutex.unlock();
			return ret;
		}

	};


	typedef UniqueID<ushort>	UniqueID16;		//16位的ID生成器
	typedef UniqueID<uint>		UniqueID32;		//32位的ID生成器
	//typedef UniqueID<uint64>	UniqueID64;		//64位的ID生成器



}	// namespace


#endif