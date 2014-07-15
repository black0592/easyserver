#ifndef __SCRIPT_MANAGER_H__
#define __SCRIPT_MANAGER_H__

#include "lua.hpp"
#include "luatinker/lua_tinker.h"
#include "BaseTypes.h"
#include "misc/Singleton.h"
#include <string>
#include <map>
using namespace std;

namespace easygame {

class ScriptManager;

enum class ScriptType
{
	NewState,		// �µ������
	ThreadState,	// Э��
};

class ScriptObject
{
	friend class ScriptManager;

protected:
	ScriptObject(lua_State* mainState);
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

	// ����һ��GC����
	void runGC();

	// ��õ�ǰ�ű�ռ���ڴ��С(��λK�ֽ�)
	int getMemory();

	void close();

protected:

private:
	string mFileName;		// �ļ���
	lua_State* mLuaState;	// lua�����
	ScriptType mScriptType;
};


//////////////////////////////////////////////////////////////////////////

class ScriptManager : public Singleton<ScriptManager>
{
public:
	ScriptManager();
	virtual ~ScriptManager();

	// ���ýű��ĸ�Ŀ¼
	void setSciptRootPath(const char* path);
	const char* getSciptRootPath();

	ScriptObject* createScript(ScriptType type = ScriptType::NewState);
	void destroyScript(ScriptObject*& pScript);

	//ScriptObject* getScript(size_t index);

	void reladAllScript();

	// ������нű�ռ���ڴ��С(��λK�ֽ�)
	int getTotalMemory();

	void printInfo();

protected:

private:
	string mScriptRootPath;
	lua_State* mLuaState;	// �������
	map<ScriptObject*,ScriptObject*> mScriptList;
};



}	// namespace

#endif