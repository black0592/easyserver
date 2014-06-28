#pragma once

class ServerLogger : 
	public Logger,
	public Singleton<ServerLogger>
{
public:
	ServerLogger() : Logger("ServerLogger") {}
	virtual ~ServerLogger(){}

protected:
private:
};


#define LOGD	ServerLogger::getInstance().debug
#define LOGI	ServerLogger::getInstance().info
#define LOGW	ServerLogger::getInstance().warn
#define LOGE	ServerLogger::getInstance().error

