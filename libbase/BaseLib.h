#ifndef __BASELIB_H__
#define __BASELIB_H__

// stl头文件
#include <map>
//#include <hash_map>
#include <stack>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;


// 系统标准库
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>


// 常用库头文件
#include "BaseDefine.h"
#include "BaseTypes.h"
#include "BaseMisc.h"
#include "platform/Platform.h"
#include "debugger/MemoryLeak.h"
#include "debugger/Profiler.h"
#include "thread/Thread.h"
#include "logger/Logger.h"
#include "data/Properties.h"
#include "misc/Noncopyable.h"
#include "misc/Singleton.h"
#include "misc/ByteBuffer.h"
#include "misc/Rand.h"
#include "misc/UniqueID.h"
#include "time/CTime.h"
#include "time/Timer.h"
#include "entity/EntityManager.h"
#include "netservice/client/SocketClient.h"
#include "netservice/NetService.h"
#include "dump/MiniDump.h"
#include "pathfinder/PathFinder.h"
#include "pugixml/pugixml.hpp"
#include "script/ScriptManager.h"
#include "zlib/zlib.h"
#include "filesystem/FileSystem.h"
#include "protohandler/ProtoDataFill.h"
#include "protohandler/ProtoTask.h"
#include "protohandler/ProtoClient.h"
#include "objectsystem/GameObject.h"

#define SOCKET_CLIENT	SocketClient



#endif
