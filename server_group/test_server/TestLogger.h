#ifndef __SUPER_LOGGER_H__
#define __SUPER_LOGGER_H__
#include "logger/Logger.h"
#include "misc/Singleton.h"
using namespace easygame;

class TestLogger : 
	public Logger,
	public Singleton<TestLogger>
{
public:
	TestLogger() : Logger("TestLogger") {}
	virtual ~TestLogger(){}

protected:
private:
};


#define LOGD	TestLogger::getInstance().debug
#define LOGI	TestLogger::getInstance().info
#define LOGW	TestLogger::getInstance().warn
#define LOGE	TestLogger::getInstance().error



#endif