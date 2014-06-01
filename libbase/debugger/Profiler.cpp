#include "BaseLib.h"
#include "Profiler.h"


namespace easygame {

	//FuncProfiler::FuncProfiler(const char* log, double timeout /* = 0 */)
	FuncProfiler::FuncProfiler(const char* log, int64 timeout /* = 0 */)
	{
		m_timeout = timeout;
		if (log == NULL) {
			m_strLog = "[����]";
		} else {
			m_strLog = "[����]";
			m_strLog += log;
		}

		m_lastTick = Platform::tick();

		//if ( !QueryPerformanceFrequency(&m_feq) ) {
		//	printf("ϵͳ��֧�ָ�Ƶʱ��\n");
		//}

		//QueryPerformanceCounter(&m_lastTick);
	}


	FuncProfiler::~FuncProfiler()
	{
		if (m_strLog.length() > 0) {
			//LARGE_INTEGER curTick;
			//QueryPerformanceCounter(&curTick);
			//double elapse = ((double)curTick.QuadPart-(double)m_lastTick.QuadPart)/((double)m_feq.QuadPart); //ʱ�����

			int64 curTick = Platform::tick();
			int64 elapse = curTick - m_lastTick;

			char temp[128];
			sprintf(temp, " time=%lld����", elapse);
			m_strLog += temp;

			if (elapse >= m_timeout) {
				BLOGW("%s", m_strLog.c_str());
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////


	Profiler::Profiler(void)
	{
		m_isLogOpened = true;
		m_timeout = 0;
		m_lastTick = Platform::tick();
		//if ( !QueryPerformanceFrequency(&m_feq) ) {
		//	printf("ϵͳ��֧�ָ�Ƶʱ��\n");
		//}
	}

	Profiler::~Profiler(void)
	{
	}

	void Profiler::begin()
	{
		stPF item;
		//QueryPerformanceCounter(&item.lastTick);
		item.lastTick = Platform::tick();
		m_fpItems.push(item);
	}

	//void Profiler::end(const char* log, double timeout /* = 0 */)
	void Profiler::end(const char* log, int64 timeout /* = 0 */)
	{
		if (m_fpItems.empty()) {
			printf("[���ش��󣡣���]Profiler�г��ֲ�ƥ��");
			return;
		}

		stPF& item = m_fpItems.top();
		//LARGE_INTEGER curTick;
		//QueryPerformanceCounter(&curTick);
		//double elapse = ((double)curTick.QuadPart-(double)item.lastTick.QuadPart)/((double)m_feq.QuadPart); //ʱ�����
		int64 curTick = Platform::tick();
		int64 elapse = curTick - item.lastTick;

		m_fpItems.pop();

		if (log == NULL)
			return;

		if (!m_isLogOpened)
			return;

		char temp[128];
		sprintf(temp, " time=%lld����", elapse);
		string strLog = "[����]";
		strLog += log;
		strLog += temp;

		int64 timeTmp = timeout > 0 ? timeout : m_timeout;
		if (elapse >= timeTmp) {
			BLOGW("%s", strLog.c_str());
		}
	}


}	// namespace


