#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "Entity.h"
#include <string>
#include <map>

namespace easygame {


	template<int i>
	class EntityManagerNone
	{
	public:
		EntityManagerNone(){}
		virtual ~EntityManagerNone(){}

	protected:
		bool insert(Entity* entity){return true;}
		void remove(Entity* entity){}
		uint size(){return 0;}
		void clear(){}
	};


	class EntityManagerID
	{
	public:
		EntityManagerID(){}
		virtual ~EntityManagerID(){}

		typedef  std::map<uint,Entity*> EntityMap;
		typedef  EntityMap::iterator EntityIter;

	protected:
		bool insert(Entity* entity)
		{
			EntityIter it = m_entityMap.find(entity->getID());
			if (it == m_entityMap.end()) {
				m_entityMap[entity->getID()] = entity;
				return true;
			}

			return false;
		}

		void remove(Entity* entity)
		{
			m_entityMap.erase(entity->getID());
		}

		size_t size(){return m_entityMap.size();}

		void clear(){ m_entityMap.clear(); }

		Entity* getEntityByID(uint id)
		{
			EntityIter it = m_entityMap.find(id);
			if (it != m_entityMap.end()) {
				return it->second;
			}

			return NULL;
		}

	protected:
		EntityMap m_entityMap;
	};

	class EntityManagerLongID
	{
	public:
		EntityManagerLongID(){}
		virtual ~EntityManagerLongID(){}

		typedef  std::map<uint64,Entity*> EntityMap;
		typedef  EntityMap::iterator EntityIter;

	protected:
		bool insert(Entity* entity)
		{
			EntityIter it = m_entityMap.find(entity->getLongID());
			if (it == m_entityMap.end()) {
				m_entityMap[entity->getLongID()] = entity;
				return true;
			}

			return false;
		}

		void remove(Entity* entity)
		{
			m_entityMap.erase(entity->getLongID());
		}

		size_t size(){return m_entityMap.size();}

		void clear(){ m_entityMap.clear(); }

		Entity* getEntityByLongID(uint64 longId)
		{
			EntityIter it = m_entityMap.find(longId);
			if (it != m_entityMap.end()) {
				return it->second;
			}

			return NULL;
		}

	protected:
		EntityMap m_entityMap;
	};

	class EntityManagerName
	{
	public:
		EntityManagerName(){}
		virtual ~EntityManagerName(){}

		typedef  std::map<std::string,Entity*> EntityMap;
		typedef  EntityMap::iterator EntityIter;

	protected:
		bool insert(Entity* entity)
		{
			EntityIter it = m_entityMap.find(entity->getName());
			if (it == m_entityMap.end()) {
				m_entityMap[entity->getName()] = entity;
				return true;
			}

			return false;
		}

		void remove(Entity* entity)
		{
			m_entityMap.erase(entity->getName());
		}

		size_t size(){return m_entityMap.size();}

		void clear(){ m_entityMap.clear(); }

		Entity* getEntityByName(const std::string& name)
		{
			EntityIter it = m_entityMap.find(name);
			if (it != m_entityMap.end()) {
				return it->second;
			}

			return NULL;
		}

		Entity* getEntityByName(const char* name)
		{
			return getEntityByName(std::string(name));
		}

	protected:
		EntityMap m_entityMap;
	};


	template <class T>
	struct stCallBack
	{
		virtual bool exec(T* obj) { return true; }
	};


	// e1,e2,e3 - 管理器
	template < typename e1,typename e2 = EntityManagerNone<1>,typename e3 = EntityManagerNone<2> >
	class EntityManager : public e1, public e2, public e3
	{
	public:
		EntityManager(void){}
		virtual ~EntityManager(void)
		{
			release();
		}

		void release()
		{
			Entity* pEntity = NULL;
			while(!e1::m_entityMap.empty()) {
				pEntity = e1::m_entityMap.begin()->second;
				removeEntity(pEntity);
				delete pEntity;
			}
		}

		bool addEntity(Entity* entity)
		{
			if (entity == NULL) return false;

			if ( e1::insert(entity) ) {

				if ( e2::insert(entity) ) {

					if ( !e3::insert(entity) ) {
						e2::remove(entity);
						e1::remove(entity);
					} else {
						return true;
					}

				} else {
					e1::remove(entity);
				}

			}

			return false;
		}

		bool removeEntity(Entity* entity)
		{
			if (entity == NULL) return false;

			e1::remove(entity);
			e2::remove(entity);
			e3::remove(entity);

			return true;
		}

		size_t size()
		{
			return e1::size();
		}

		// 执行回调函数
		// 返回 true 完整执行， false 执行被打断
		template <class T>
		bool exec(stCallBack<T> &cb)
		{
			if (size() == 0)
				return true;

			for (typename e1::EntityIter it = e1::m_entityMap.begin(); it!=e1::m_entityMap.end(); it++) {
				if (!cb.exec((T*)it->second))
					return false;
			}

			return true;
		}

		// 获得所有对象
		template <class T>
		void getAllEntity(vector<T*>& list)
		{
			if (size() == 0)
				return;

			for (typename e1::EntityIter it = e1::m_entityMap.begin(); it!=e1::m_entityMap.end(); it++) {
				list.push_back((T*)it->second);
			}
		}

		// 方便迭代
		typename e1::EntityIter begin() { return e1::m_entityMap.begin(); }
		typename e1::EntityIter end() { return e1::m_entityMap.end(); }

	};


}	// namespace


#endif


