#ifndef __SUPER_LOGGER_H__
#define __SUPER_LOGGER_H__


class SuperLogger : 
	public Logger,
	public Singleton<SuperLogger>
{
public:
	SuperLogger() : Logger("SuperLogger") {}
	virtual ~SuperLogger(){}

protected:
private:
};


#define LOGD	SuperLogger::getInstance().debug
#define LOGI	SuperLogger::getInstance().info
#define LOGW	SuperLogger::getInstance().warn
#define LOGE	SuperLogger::getInstance().error



#endif