#include "ScriptLoader.h"
#include "BaseLib.h"
#include "luatinker/lua_tinker.h"


typedef int (*manual_load_file_func)(lua_State* L, const char* filename);  
typedef int (*readable_func)(const char* filename);  


static manual_load_file_func manul_load_file = NULL;  
static readable_func readablefunc = NULL;  

static void loaderror(lua_State *L, const char *filename)  
{  
	luaL_error(L, "error loading module " LUA_QS " from file " LUA_QS ":\n\t%s",  
		lua_tostring(L, 1), filename, lua_tostring(L, -1));  
}  

static const char *pushnexttemplate(lua_State *L, const char *path)  
{  
	const char *l;  
	while (*path == *LUA_PATHSEP) path++;  /* skip separators */  
	if (*path == '\0') return NULL;  /* no more templates */  
	l = strchr(path, *LUA_PATHSEP);  /* find next separator */  
	if (l == NULL) l = path + strlen(path);  
	lua_pushlstring(L, path, (size_t)(l - path));  /* template */  
	return l;  
}  

static const char *searchpath (lua_State *L, const char *name,  
							   const char *path, const char *sep,  
							   const char *dirsep)  
{  
	luaL_Buffer msg;  /* to build error message */  
	luaL_buffinit(L, &msg);  
	if (*sep != '\0')  /* non-empty separator? */  
		name = luaL_gsub(L, name, sep, dirsep);  /* replace it by 'dirsep' */  
	while ((path = pushnexttemplate(L, path)) != NULL) {  
		const char *filename = luaL_gsub(L, lua_tostring(L, -1),  
			LUA_PATH_MARK, name);  
		lua_remove(L, -2);  /* remove path template */  
		if (readablefunc && readablefunc(filename))  /* does file exist and is readable? */  
			return filename;  /* return that file name */  
		lua_pushfstring(L, "\n\tno file " LUA_QS, filename);  
		lua_remove(L, -2);  /* remove file name */  
		luaL_addvalue(&msg);  /* concatenate error msg. entry */  
	}  
	luaL_pushresult(&msg);  /* create error message */  
	return NULL;  /* not found */  
}  

static const char *findfile(lua_State *L, const char *name,  
							const char *pname)  
{  
	const char *path;  
	lua_getfield(L, LUA_ENVIRONINDEX, pname);  
	path = lua_tostring(L, -1);  
	if (path == NULL)  
		luaL_error(L, LUA_QL("package.%s") " must be a string", pname);  
	return searchpath(L, name, path, ".", LUA_DIRSEP);  
}  

static int lua_file_from_manual_func(lua_State *L)  
{  
	const char *filename;  
	const char *name = luaL_checkstring(L, 1);  
	filename = findfile(L, name, "path");  
	if (filename == NULL) return 1;  /* library not found in this path */  
	if (manul_load_file && manul_load_file(L, filename) != 0)  
		loaderror(L, filename);  
	return 1;  /* library loaded successfully */  
}  

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 自定义加载函数

static const char *getF (lua_State *L, void *ud, size_t *size) {
	File* pFile = (File*)ud;

	if (pFile->isEnd())
		return NULL;

	*size = pFile->getLength();
	pFile->setPosition(pFile->getLength());

	return (const char*)pFile->getBufferPtr();
}

static int errfile (lua_State *L, const char *what, int fnameindex) {
	const char *serr = strerror(errno);
	const char *filename = lua_tostring(L, fnameindex) + 1;
	lua_pushfstring(L, "cannot %s %s: %s", what, filename, serr);
	lua_remove(L, fnameindex);
	return LUA_ERRFILE;
}

static int my_load_file(lua_State* L, const char* filename)
{
	File file;
	int status;
	int fnameindex = lua_gettop(L) + 1;  /* index of filename on the stack */
	if (filename == NULL) {
		lua_pushliteral(L, "=stdin");
	}
	else {
		lua_pushfstring(L, "@%s", filename);
		if ( !file.open(filename) ) {
			BLOGE("my_load_file 加载脚本文件 %s 失败！！", filename);
			return errfile(L, "open", fnameindex);
		}
	}

	status = lua_load(L, getF, &file, lua_tostring(L, -1));

	lua_remove(L, fnameindex);
	return status;
}

static int my_readable(const char* filename)
{
	File file;
	if (!file.open(filename))
		return 0;

	return 1;
}

void initScriptLoader(lua_State *luaState)
{  
	manul_load_file = my_load_file;  
	readablefunc = my_readable;  

	lua_getglobal(luaState,"package");  
	lua_getfield(luaState,-1,"loaders");//package.loaders  
	lua_pushnumber(luaState, 2);  
	lua_gettable(luaState, -2);//package.loaders[2]  
	lua_pushnumber(luaState,2);//先把索引压栈，呆会用来设置C函数到这个索引位置  
	lua_pushcfunction(luaState,lua_file_from_manual_func);//压入自己的C函数  
	lua_getfenv(luaState,-3);//获取旧函数的环境表  
	int ret=lua_setfenv(luaState,-2);//设置到新函数  
	lua_settable(luaState,-4);//替换旧函数  
	lua_pop(luaState,3);//清理堆栈  
}  

