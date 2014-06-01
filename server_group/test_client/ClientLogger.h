#ifndef __CLIENT_LOGGER_H__
#define __CLIENT_LOGGER_H__


class ClientLogger : public Logger, public Singleton<ClientLogger>
{
public:
	ClientLogger() : Logger("ClientLogger") {}
	virtual ~ClientLogger(){}

protected:
private:
};



#define LOGD	ClientLogger::getInstance().debug
#define LOGI	ClientLogger::getInstance().info
#define LOGW	ClientLogger::getInstance().warn
#define LOGE	ClientLogger::getInstance().error



#endif