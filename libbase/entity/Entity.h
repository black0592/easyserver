#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "BaseTypes.h"
#include <string>

namespace easygame {

	// 同时以ID和Name为索引的对象
	class Entity
	{
	public:
		Entity() : m_id(0),m_longId(0),m_name("") {}
		virtual ~Entity(void) {}

		virtual void setID(uint id) { m_id = id; }
		virtual uint getID() const { return m_id; }

		virtual void setLongID(uint64 longId) { m_longId = longId; }
		virtual uint64 getLongID() const{ return m_longId; }

		virtual void setName(const char* name) { m_name = std::string(name); }
		virtual void setName(const std::string& name) { m_name = name; }
		virtual const char* getName() const { return m_name.c_str(); }
		//virtual const string& getName() { return m_name; }

	protected:
		uint m_id;
		uint64 m_longId;
		std::string m_name;
	};


}	// namespace

#endif


