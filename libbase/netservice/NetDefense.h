#ifndef __NET_DEFENSE_H__
#define __NET_DEFENSE_H__

#include "BaseTypes.h"
#include "misc/Singleton.h"
#include "platform/Platform.h"
#include <map>


#define DDS_CHECK							// �Ƿ���DDS�������
#define DDS_CHECK_TIME			50			// ��������հ����С�����ʱ��
#define DDS_CHECK_COUNT			200			// ��������յ�dds��������
#define BLACKIP_VALID_TIME		60 * 1000	// ��������Чʱ�䣨���룩

namespace easygame {

	// �Ƚϼ򵥵����繥�����ͷ���
	class NetDefense : public Singleton<NetDefense>
	{
	public:
		typedef std::map<string,int64> BlacklistMap;
		typedef BlacklistMap::iterator BlacklistMapIter;

	public:
		NetDefense()
		{
			mEnable = false;
		}
		virtual ~NetDefense()
		{

		}

		void setEnable(bool enable)
		{
			mEnable = enable;
		}

		bool isEnable()
		{
			return mEnable;
		}

		bool isInBlacklist(const char* ip)
		{
			if (!mEnable)
				return false;

			if (mIpBlacklist.empty())
				return false;

			if ( mIpBlacklist.find(ip) != mIpBlacklist.end() )
				return true;

			return false;
		}

		void addBlacklist(const char* ip)
		{
			if (!mEnable)
				return;

			mIpBlacklist[ip] = Platform::tick();
		}

		void removeBlacklist(const char* ip)
		{
			if (!mEnable)
				return;

			BlacklistMapIter it = mIpBlacklist.find(ip);
			if (it != mIpBlacklist.end())
				mIpBlacklist.erase(it);
		}

		void clearBlacklist()
		{
			if (!mEnable)
				return;

			mIpBlacklist.clear();
		}

		// ��ʱ���������
		void onTimer()
		{
			if (!mEnable)
				return;

			int64 curTick = Platform::tick();
			for (BlacklistMapIter it = mIpBlacklist.begin(); it != mIpBlacklist.end(); it++) {
				if (curTick - it->second > BLACKIP_VALID_TIME) {
					mIpBlacklist.erase(it);
					break;
				}
			}
		}


	protected:
		bool mEnable;
		// IP������
		BlacklistMap mIpBlacklist;
	};


}	// namespace


#endif