#ifndef __SCRIPT_MANAGER_H__
#define __SCRIPT_MANAGER_H__

#include "lua.hpp"
#include "luatinker/lua_tinker.h"
#include "BaseTypes.h"
#include "misc/Singleton.h"
#include <string>
#include <vector>
using namespace std;

namespace easygame {

class ScriptManager;

class ScriptObject
{
	friend class ScriptManager;

protected:
	ScriptObject();
	virtual ~ScriptObject();

public:
	lua_State* getState() { return mLuaState; }

	// ����lua����
	template<typename T>
	void set(const char* name, T object)
	{
		lua_tinker::set<T>(mLuaState, name, object);
	}

	// ���lua����
	template<typename T>
	T get(const char* name)
	{
		return lua_tinker::get<T>(mLuaState, name);
	}

	// ����C++ȫ�ֺ�����lua
	template<typename F> 
	void def(const char* name, F func)
	{ 
		lua_tinker::def(mLuaState, name, func);
	}

	bool dofile(const char* filename);
	bool dobuffer(const char* buff, size_t len);
	bool dostring(const char* script);

	void close();

protected:
private:
	string mFileName;		// �ļ���
	lua_State* mLuaState;	// lua�����
};


//////////////////////////////////////////////////////////////////////////

class ScriptManager : public Singleton<ScriptManager>
{
	typedef vector< ScriptObject* > ScriptList;

public:
	ScriptManager();
	virtual ~ScriptManager();

	ScriptObject* createScript();
	void destroyScript(ScriptObject*& pScript);

	ScriptObject* getScript(size_t index);

	void reladAllScript();

protected:
private:
	
	ScriptList  mScriptList;
};



}	// namespace

#endif