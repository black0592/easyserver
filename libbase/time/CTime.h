#ifndef __CTIME_H__
#define __CTIME_H__
#include <string.h>

namespace easygame {

	// ����
	struct stDateTime
	{
		uint year;		// ��
		uint month;		// ��
		uint day;		// ��
		uint hour;		// ʱ
		uint minute;	// ��
		uint second;	// ��

		stDateTime()
		{
			year = month = day = 0;
			hour = minute = second = 0;
		}

		inline uint64 toNumber() const
		{
			uint64 time_num = 0;
			time_num |= ((uint64)minute);
			time_num |= ((uint64)hour << 8);
			time_num |= ((uint64)day << 16);
			time_num |= ((uint64)month << 24);
			time_num |= ((uint64)year << 32);
			return time_num;
		}

		inline bool operator < ( const stDateTime& datetime )
		{
			return this->toNumber() < datetime.toNumber();
		}

		inline bool operator <= ( const stDateTime& datetime )
		{
			return this->toNumber() <= datetime.toNumber();
		}

		inline bool operator > ( const stDateTime& datetime )
		{
		return this->toNumber() > datetime.toNumber();
		}

		inline bool operator >= ( const stDateTime& datetime )
		{
			return this->toNumber() >= datetime.toNumber();
		}
	};


	// ����ʱ�䣬����ʱ��ת��
	class LocalTime
	{
	public:
		LocalTime(time_t time = -1)
		{
			m_time = time;
			if (m_time == -1)
				m_time = ::time(NULL);

			m_tm = *::localtime(&m_time);
		}

		~LocalTime(){ }

		// ��ǰ����ʱ��(δ����ʱ��ת��)
		time_t timestamp() { return m_time; }

		// ��䵽DateTime���ݽṹ
		stDateTime getDateTime()
		{
			stDateTime dateTime;
			dateTime.year = year();
			dateTime.month= month();
			dateTime.day = day();
			dateTime.hour = hour();
			dateTime.minute = minute();
			dateTime.second = second();
			return dateTime;
		}

		// ����Ӣ�ĸ�ʽ�������ַ���
		string getAscTime()
		{
			string strTime = ::asctime(&m_tm);
			return strTime;
		}

		// �������ĸ�ʽ�����ַ���
		string getStrTime()
		{
			char szBuf[256];
			sprintf(szBuf,"%04d��%02d��%02d�� %02d:%02d:%02d", 
				year(), month(), day(), 
				hour(),minute(),second());
			string strTime = szBuf;
			return strTime;
		}

		// �������ĸ�ʽ�����ַ���
		string getStrTimestamp()
		{
			char szBuf[256];
			sprintf(szBuf,"%04d-%02d-%02d %02d:%02d:%02d", 
				year(), month(), day(), 
				hour(),minute(),second());
			string strTime = szBuf;
			return strTime;
		}

		// ��
		int year() { return m_tm.tm_year + 1900; }

		// ��(1-12)
		int month() { return m_tm.tm_mon + 1; }

		// ��(1-31)
		int day() { return m_tm.tm_mday; }

		// ����(0-6)
		int weekday() { return m_tm.tm_wday; }

		// ��(0-365)
		int yday() { return m_tm.tm_yday; }

		// ʱ(0-23)
		int hour() { return m_tm.tm_hour; }

		// ��(0-59)
		int minute() { return m_tm.tm_min; }

		// ��(0-59)
		int second() { return m_tm.tm_sec; }

		// �ж��Ƿ���ͬһ��
		bool isSameDay(LocalTime& localTime)
		{
			if (localTime.year() == year() &&
				localTime.month() == month() && 
				localTime.day() == day())
			{
				return true;
			}

			return false;
		}

	public:
		time_t m_time;
		tm m_tm;
	};


	//// localtime ����ʱ��(����ʱ��ת��)
	//// rtime	 ����ʱ��(δ����ʱ��ת��)
	//// ��ʵ��ʱ����
	//class RTime
	//{
	//public:
	//	RTime(void) { now(); }
	//	RTime(time_t time) { m_time = time; }
	//	virtual ~RTime(void) {}

	//	void time(time_t time) { m_time = time; }

	//	time_t time() { return m_time; }

	//	// ��¼��ǰʱ��
	//	void now() { m_time = ::time(NULL); }

	//	// ��ʵʱ��ת��Ϊ����
	//	time_t second() { return m_time; }

	//	// ��ñ���ʱ��(�����Ҫ�������ʱ�䣬�ȵ���now)
	//	LocalTime getLocalTime()
	//	{
	//		LocalTime sysTime(m_time);
	//		return sysTime;
	//	}

	//private:
	//	time_t m_time;
	//};

/*
#ifdef WIN32
#include <Windows.h>
	// high time
	// �߾���ʱ��
	class HighTime
	{
	public:
		HighTime() { if (!QueryPerformanceFrequency(&freq)) printf("ϵͳ��֧�ָ߾���ʱ��\n"); }
		virtual ~HighTime(){}

		// ��¼ʱ��
		void begin()
		{
			QueryPerformanceCounter(&lastTick);
		}

		// �õ���begin�����ڵ�΢��
		double end()
		{
			LARGE_INTEGER curTick;
			QueryPerformanceCounter(&curTick);
			double sec = ((double)curTick.QuadPart - (double)lastTick.QuadPart) / (double)freq.QuadPart;
			double mic = sec * 1000000.0f;	// ΢��
			return mic;
		}

	private:
		LARGE_INTEGER freq;
		LARGE_INTEGER lastTick;
	};

#endif
*/


} // namespace

#endif

