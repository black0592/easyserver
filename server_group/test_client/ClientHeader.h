#ifndef __CLIENT_HEADER_H__
#define __CLIENT_HEADER_H__

// 系统库
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <map>
using namespace std;

// 网络基础库
#include "BaseLib.h"
using namespace easygame;

// TestClient的头文件
#include "ClientLogger.h"
#include "TestClient.h"
#include "MsgDefine.h"

// 随机重连的概率
extern int rand_reconnect;

#endif