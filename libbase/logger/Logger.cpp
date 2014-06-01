#include "Logger.h"
#include "platform/Platform.h"
#include <stdarg.h>
#include <string.h>
#ifdef WIN32
#include <Windows.h>
#include <WinCon.h>
#else
#endif

namespace easygame {

	//////////////////////////////////////////////////////////////////////////
	

#ifdef WIN32
	// FOREGROUND_INTENSITY ����
	const static ushort cs_sColor[] = 
	{
		FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE,		// ��ɫ
		FOREGROUND_GREEN,										// ��ɫ
		FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_RED,	// ��ɫ
		FOREGROUND_INTENSITY|FOREGROUND_RED,					// ��ɫ
		FOREGROUND_BLUE|FOREGROUND_GREEN,						// ����ɫ
		FOREGROUND_RED|FOREGROUND_BLUE
	};
#else

	const static char cs_strColor[][50] = { 
		"\e[0m",
		"\e[32m", //green
		"\e[31m", //red
		"\e[33m", //yellow
		"\e[34m\e[1m",//hight blue
		"\e[35m"};
#endif

		void showColorText(const char *text, int level)
		{
#ifdef WIN32
			//static Mutex s_mutex;
			//MutexScopeLock scopeLock(s_mutex);
#endif

			if (level >= 0 && level < LOG_MAX) {
#ifdef WIN32
				HANDLE hStd = ::GetStdHandle(STD_OUTPUT_HANDLE);
				if (hStd == INVALID_HANDLE_VALUE) return;

				SetConsoleTextAttribute(hStd, cs_sColor[level]);
				printf("%s", text);
				SetConsoleTextAttribute(hStd, cs_sColor[0]);
#else
				printf("%s%s\e[0m", cs_strColor[level], text);
				printf("\e[0m");
#endif
			}
		}


	//////////////////////////////////////////////////////////////////////////

	// ȫ����־ϵͳ
	Logger g_Logger("GlobalLogger");

#define VarMessage(msg,format) \
	va_list args;	\
	va_start(args, format);	\
	vsnprintf(msg,sizeof(msg)-1,format,args);	\
	va_end(args); 

	Logger::Logger(const string& name)
		:Thread(name)
	{
		m_timeFileName = "";
		m_filename = "";
		m_isOutputConsole = false;
		memset(m_logMsg,0,sizeof(m_logMsg));
	}

	Logger::~Logger(void)
	{
		stop();
	}

	bool Logger::isFileExist(const char* filename)
	{
		FILE* file = fopen(filename,"r+b");
		if (file == NULL)
			return false;
		fclose(file);
		return true;
	}

	const char* Logger::getTypeString(LogType logType)
	{
		switch (logType)
		{
		case LOG_DEBUG:
			{
				return "DEBUG";
			}
			break;
		case LOG_INFO:
			{
				return " INFO";
			}
			break;
		case LOG_WARN:
			{
				return " WARN";
			}
			break;
		case LOG_ERROR:
			{
				return "ERROR";
			}
			break;
		default:
			{ }
			break;
		}

		return " NONE";
	}

	bool Logger::processFileCreate()
	{
		if (m_logItemQueue.empty()) {
			return false;
		}

		string strHourTime = getHourTime();
#if 0
		// �ļ���.log.ʱ���
		string strTimeFileName = m_filename;
		strTimeFileName += ".";
		strTimeFileName += strHourTime;
#else	
		// �ļ���_ʱ���.log
		string strTimeFileName = m_filename;
		strTimeFileName += "_";
		strTimeFileName += strHourTime;
		strTimeFileName += ".log";
#endif
		if (strTimeFileName != m_timeFileName) {
			if (m_fstream.is_open()) {
				// �ȹر�֮ǰ�򿪵��ļ�
				m_fstream.flush();
				m_fstream.close();
			}
			// ����Ŀ¼
			string logPath = strTimeFileName.substr(0, strTimeFileName.find_last_of('/'));
			Platform::createDirectorys(logPath.c_str());
			m_fstream.open(strTimeFileName.c_str(), ios_base::app);
			if (!m_fstream.is_open()) {
				printf("���ļ�%sʧ��\n", strTimeFileName.c_str());
				return false;
			}
			m_timeFileName = strTimeFileName;
		}

		return true;
	}

	bool Logger::writeToFile()
	{
		stLogItem logItem;

		{
			// ȡ����־
			MutexScopeLock scopeLock(m_lock);
			if (m_logItemQueue.empty()) {
				return false;
			}
			logItem = m_logItemQueue.front();
			m_logItemQueue.pop();
		}

		string strTemp = "";
		strTemp += "[";
		strTemp += getTypeString(logItem.type);
		strTemp += "] <";
		strTemp += m_logname;
		strTemp += "> ";
		strTemp += getTimeStamp();
		strTemp += " - ";
		strTemp += logItem.strLog;
		strTemp += "\n";

		m_fstream << strTemp.c_str();

#ifdef WIN32
		// linux��ֻ���ļ���־�����ô�ӡ������̨
		if (m_isOutputConsole && logItem.isOutputConsole) {
			showColorText(strTemp.c_str(), logItem.type);
		}
#endif

		m_fstream.flush();

		return true;
	}

	string Logger::getHourTime()
	{
		// %04d-%02d-%02d-%02d
		T_TIME nowTime;
		Platform::now(nowTime);
		char temp[1024];
		sprintf(temp, "%04d-%02d-%02d-%02d", 
			nowTime.tm_year, nowTime.tm_mon, nowTime.tm_mday, nowTime.tm_hour);
		string strTime = temp;
		return strTime;
	}

	string Logger::getTimeStamp()
	{
		// 04d-%02d-%02d %02d:%02d:%02d
		T_TIME nowTime;
		Platform::now(nowTime);
		char temp[1024];
		Platform::format(temp, nowTime);
		string strTime = temp;
		return strTime;
	}

	//  ��������̨���(ϵͳĬ���ǿ���)
	void Logger::openOutputConsole()
	{
		m_isOutputConsole = true;
	}

	//  �رտ���̨���
	void Logger::closeOutputConsole()
	{
		m_isOutputConsole = false;
	}

	bool Logger::run()
	{
		
		while(isRunning()) {
			while(true) {
				if (!processFileCreate()) {
					break;
				}
		
				if (!writeToFile()) {
					break;
				}
			}

			yield();
		}

		return true;
	}

	// ������־ϵͳ
	bool Logger::start(const char* filename, const char* logname, bool isOutputConsole)
	{
		if (isRunning()) {
			printf("��־ϵͳ�Ѿ����������ȹرա�name=%s\n", logname);
			return false;
		}

		if (filename == NULL || logname == NULL) {
			return false;
		}

		m_isOutputConsole = isOutputConsole;
		m_filename = filename;
		m_logname = logname;

		return Thread::start();
	}

	// �ر���־ͳ��
	bool Logger::stop(void)
	{
		if (!isRunning())
			return false;

		if (m_fstream.is_open())
			m_fstream.close();

		Thread::stop();
		return true;
	}

	// debug����ļ�¼
	bool Logger::debug(const char* strFormat, ...)
	{
#ifdef _DEBUG
		if (!isRunning())
			return false;

		m_lock.lock();
		VarMessage(m_logMsg, strFormat);
		stLogItem logItem;
		logItem.type = LOG_DEBUG;
		logItem.strLog = m_logMsg;
		m_logItemQueue.push(logItem);
		m_lock.unlock();

		//printf("%s\n", m_logMsg);
#endif

		return true;
	}

	// info����ļ�¼
	bool Logger::info(const char* strFormat, ...)
	{
		if (!isRunning())
			return false;

		m_lock.lock();
		VarMessage(m_logMsg, strFormat);
		stLogItem logItem;
		logItem.type = LOG_INFO;
		logItem.strLog = m_logMsg;
		m_logItemQueue.push(logItem);
		m_lock.unlock();

		return true;
	}

	bool Logger::infoEx(bool isOutputConsole, const char* strFormat, ...)
	{
		if (!isRunning())
			return false;

		m_lock.lock();
		VarMessage(m_logMsg, strFormat);
		stLogItem logItem;
		logItem.type = LOG_INFO;
		logItem.strLog = m_logMsg;
		logItem.isOutputConsole = isOutputConsole;
		m_logItemQueue.push(logItem);
		m_lock.unlock();

		return true;
	}

	// warn����ļ�¼
	bool Logger::warn(const char* strFormat, ...)
	{
//#ifdef _DEBUG
		if (!isRunning())
			return false;

		m_lock.lock();
		VarMessage(m_logMsg, strFormat);
		stLogItem logItem;
		logItem.type = LOG_WARN;
		logItem.strLog = m_logMsg;
		m_logItemQueue.push(logItem);
		m_lock.unlock();

		//printf("%s\n", m_logMsg);
//#endif

		return true;
	}

	// error����ļ�¼
	bool Logger::error(const char* strFormat, ...)
	{
		if (!isRunning())
			return false;

		m_lock.lock();
		VarMessage(m_logMsg, strFormat);
		stLogItem logItem;
		logItem.type = LOG_ERROR;
		logItem.strLog = m_logMsg;
		m_logItemQueue.push(logItem);
		m_lock.unlock();

		return true;
	}



}	// namespace