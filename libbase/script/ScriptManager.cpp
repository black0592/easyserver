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

//////////////////////////////////////////////////////////////////////////

ScriptManager::ScriptManager()
{
}

ScriptManager::~ScriptManager()
{
	while (!mScriptList.empty()) {
		delete mScriptList[mScriptList.size()-1];
		mScriptList.pop_back();
	}
}

ScriptObject* ScriptManager::createScript()
{
	ScriptObject* pScript = new ScriptObject();
	mScriptList.push_back(pScript);
	return pScript;
}

void ScriptManager::destroyScript(ScriptObject*& pScript)
{
	SAFE_DELETE(pScript);
}

ScriptObject* ScriptManager::getScript(size_t index)
{
	if (index >= mScriptList.size())
		return NULL;

	return mScriptList[index];
}

void ScriptManager::reladAllScript()
{
	// TODO
}



}	// namespace