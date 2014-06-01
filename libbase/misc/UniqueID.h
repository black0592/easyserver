#ifndef __UNIQUEID_H__
#define __UNIQUEID_H__

#include "Noncopyable.h"
#include "thread/Mutex.h"
#include "Rand.h"
#include <set>

namespace easygame {

	//UniqueIDģ��
	//��ģ��ʵ����ΨһID������������֤�̰߳�ȫ�������ø��ֳ��ȵ��޷���������ΪID

	template <class T>
	class UniqueID : private Noncopyable
	{
	private:
		Mutex mutex;
		std::set<T>	ids;	//���ڴ�Ż��յĵ�ID
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

		//��ȡ�����ЧID
		T invalid()
		{
			return maxID;
		}

		//����һ��ID�Ƿ��Ѿ��������ȥ
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

		//��ȡһ��ΨһID��������ص�ID��invalid()��ȣ���ʾ���е�ID���Ѿ������ȥ�ˣ��޷��ٷ���
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


		//��ID�Ż�ID�أ��Ա��´����·���
		//�Żص�ID����������get������ȡ�ģ����Ҳ���֤�Żص�ID��û�б������߳�ʹ��
		//�û���Ҫ�Լ���֤����ʹ�õ�ID���ᱻ�Żأ��������ID�ظ�
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

		//��ȡ���������ID���������ֵ��invalid()��ȣ���ʾ���е�ID���Ѿ������ȥ�ˣ��޷��ٷ���
		//param size	��Ҫ��ȡ��ID����
		//param count	ʵ�ʻ�õ�ID����
		//return		����������ID�У���С���Ǹ�
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


	typedef UniqueID<ushort>	UniqueID16;		//16λ��ID������
	typedef UniqueID<uint>		UniqueID32;		//32λ��ID������
	//typedef UniqueID<uint64>	UniqueID64;		//64λ��ID������



}	// namespace


#endif