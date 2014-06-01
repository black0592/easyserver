#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <queue>
#include <fstream>
#include "thread/Thread.h"

#define MAX_LOG_MSG_SIZE	4096

namespace easygame {

	enum LogType
	{
		LOG_DEBUG,		// ��ɫ���Լ��𣬷����治��ӡ
		LOG_INFO,		// ��ɫ��Ϣ
		LOG_WARN,		// ��ɫ������Ϣ
		LOG_ERROR,		// ��ɫ������Ϣ
		LOG_MAX,		// ��Ч
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

		// ������־ϵͳ
		// filename �� logname �������ָ��
		bool start(const char* filename, const char* logname, bool isOutputConsole = false);

		// �ر���־ͳ��
		bool stop(void);

		//  ��������̨���(ϵͳĬ���ǹر�)
		void openOutputConsole();

		//  �رտ���̨���
		void closeOutputConsole();

		// debug����ļ�¼
		bool debug(const char* strFormat, ...);

		// info����ļ�¼
		bool info(const char* strFormat, ...);
		bool infoEx(bool isOutputConsole, const char* strFormat, ...);

		// warn����ļ�¼
		bool warn(const char* strFormat, ...);

		// eror����ļ�¼
		bool error(const char* strFormat, ...);

	private:
		// �ж��ļ��Ƿ����
		bool isFileExist(const char* filename);

		// ȡ�õ�ǰʱ���ַ��� Y-M-D-H
		string getHourTime();

		// ȡ��ʱ����ַ��� Y-M-D H:M:S
		string getTimeStamp();

		// �����ļ�����(�����ʱ�䴴����Сʱ�ָ����־)
		bool processFileCreate();

		// ȡ�������ַ���
		const char* getTypeString(LogType logType);

		// д��־
		bool writeToFile();

		// д��־�����߳�
		virtual bool run();

	private:
		// �Ƿ�������̨���
		bool m_isOutputConsole;
		// ����ʱ����ļ���
		string m_filename;
		// ����ʱ����ļ���
		string m_timeFileName;
		// ��־��
		string m_logname;
		// �ļ���
		ofstream m_fstream;
		// ���б�־
		// �߳���
		Mutex m_lock;
		// ��־����
		queue<stLogItem> m_logItemQueue;
		// �ַ�����ʽ������
		char m_logMsg[MAX_LOG_MSG_SIZE];
	};



	extern Logger g_Logger;
	#define BLOGD	g_Logger.debug
	#define BLOGI	g_Logger.info
	#define BLOGW	g_Logger.warn
	#define BLOGE	g_Logger.error



}	// namespace

#endif

