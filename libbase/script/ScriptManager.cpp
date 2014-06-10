#include "ScriptManager.h"
#include "BaseLib.h"
#include "ScriptLoader.h"

namespace easygame {

ScriptObject::ScriptObject()
{
	mLuaState = luaL_newstate();
	luaL_openlibs(mLuaState);
	lua_tinker::init(mLuaState);	// 支持64位

	// 注册自定义加载函数
	initScriptLoader(mLuaState);
}

ScriptObject::~ScriptObject()
{
	close();
}


bool ScriptObject::dofile(const char* filename)
{
	File file;
	if (!file.open(filename)) {
		BLOGE("加载脚本文件 %s 失败！！", filename);
		return false;
	}

	return dobuffer((const char*)file.getBufferPtr(), file.getLength());
}

bool ScriptObject::dobuffer(const char* buff, size_t len)
{
	lua_tinker::dobuffer(mLuaState, buff, len);
	return true;
}

bool ScriptObject::dostring(const char* script)
{
	lua_tinker::dostring(mLuaState, script);
	return true;
}

void ScriptObject::close()
{
	if (mLuaState) {
		lua_close(mLuaState);
	}
}

void ScriptObject::runGC()
{
	lua_gc(mLuaState, LUA_GCCOLLECT, 0);
}

int ScriptObject::getMemory()
{
	return lua_gc(mLuaState, LUA_GCCOUNT, 0);
}

//////////////////////////////////////////////////////////////////////////

ScriptManager::ScriptManager()
{
}

ScriptManager::~ScriptManager()
{
	for (auto it=mScriptList.begin(); it!=mScriptList.end(); it++) {
		delete it->second;
	}
	mScriptList.clear();
}

ScriptObject* ScriptManager::createScript()
{
	ScriptObject* pScript = new ScriptObject();
	mScriptList[pScript] = pScript;
	return pScript;
}

void ScriptManager::destroyScript(ScriptObject*& pScript)
{
	auto it = mScriptList.find(pScript);
	if (it == mScriptList.end()) {
		BLOGE("not find pScript");
		return;
	}

	SAFE_DELETE(pScript);
	mScriptList.erase(it);
}

//ScriptObject* ScriptManager::getScript(size_t index)
//{
//	if (index >= mScriptList.size())
//		return NULL;
//
//	return mScriptList[index];
//}

void ScriptManager::reladAllScript()
{
	// TODO
}

int ScriptManager::getTotalMemory()
{
	int totalMemory = 0;
	for (auto it=mScriptList.begin(); it!=mScriptList.end(); it++) {
		totalMemory += it->second->getMemory();
	}

	return totalMemory;
}

void ScriptManager::printInfo()
{
	printf("script memory: %dK\n", getTotalMemory());
}

}	// namespace