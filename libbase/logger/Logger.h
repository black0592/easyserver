#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <queue>
#include <fstream>
#include "thread/Thread.h"

#define MAX_LOG_MSG_SIZE	4096

namespace easygame {

	enum LogType
	{
		LOG_DEBUG,		// 白色调试级别，发布版不打印
		LOG_INFO,		// 绿色信息
		LOG_WARN,		// 黄色警告信息
		LOG_ERROR,		// 红色错误信息
		LOG_MAX,		// 无效
	};

	struct stLogItem
	{
		LogType type;
		string strLog;
		bool isOutputConsole;

		stLogItem()
		{
			type = LOG_DEBUG;
			strLog = "";
			isOutputConsole = true;
		}
	};

	class Logger : public Thread
	{
	public:
		Logger(const string& name);
		virtual ~Logger(void);

		// 开启日志系统
		// filename 和 logname 不允许空指针
		bool start(const char* filename, const char* logname, bool isOutputConsole = false);

		// 关闭日志统计
		bool stop(void);

		//  开启控制台输出(系统默认是关闭)
		void openOutputConsole();

		//  关闭控制台输出
		void closeOutputConsole();

		// debug级别的记录
		bool debug(const char* strFormat, ...);

		// info级别的记录
		bool info(const char* strFormat, ...);
		bool infoEx(bool isOutputConsole, const char* strFormat, ...);

		// warn级别的记录
		bool warn(const char* strFormat, ...);

		// eror级别的记录
		bool error(const char* strFormat, ...);

	private:
		// 判断文件是否存在
		bool isFileExist(const char* filename);

		// 取得当前时间字符串 Y-M-D-H
		string getHourTime();

		// 取得时间戳字符串 Y-M-D H:M:S
		string getTimeStamp();

		// 处理文件创建(会根据时间创建按小时分割的日志)
		bool processFileCreate();

		// 取得类型字符串
		const char* getTypeString(LogType logType);

		// 写日志
		bool writeToFile();

		// 写日志工作线程
		virtual bool run();

	private:
		// 是否开启控制台输出
		bool m_isOutputConsole;
		// 不带时间的文件名
		string m_filename;
		// 包含时间的文件名
		string m_timeFileName;
		// 日志名
		string m_logname;
		// 文件流
		ofstream m_fstream;
		// 运行标志
		// 线程锁
		Mutex m_lock;
		// 日志队列
		queue<stLogItem> m_logItemQueue;
		// 字符串格式化缓存
		char m_logMsg[MAX_LOG_MSG_SIZE];
	};



	extern Logger g_Logger;
	#define BLOGD	g_Logger.debug
	#define BLOGI	g_Logger.info
	#define BLOGW	g_Logger.warn
	#define BLOGE	g_Logger.error



}	// namespace

#endif

