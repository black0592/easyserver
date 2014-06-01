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

	// 设置lua变量
	template<typename T>
	void set(const char* name, T object)
	{
		lua_tinker::set<T>(mLuaState, name, object);
	}

	// 获得lua变量
	template<typename T>
	T get(const char* name)
	{
		return lua_tinker::get<T>(mLuaState, name);
	}

	// 定义C++全局函数到lua
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
	string mFileName;		// 文件名
	lua_State* mLuaState;	// lua虚拟机
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