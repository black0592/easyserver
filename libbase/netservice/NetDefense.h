#ifndef __NET_DEFENSE_H__
#define __NET_DEFENSE_H__

#include "BaseTypes.h"
#include "misc/Singleton.h"
#include "platform/Platform.h"
#include <map>


#define DDS_CHECK							// 是否开启DDS攻击检测
#define DDS_CHECK_TIME			50			// 如果两次收包间隔小于这个时间
#define DDS_CHECK_COUNT			200			// 如果连续收到dds攻击次数
#define BLACKIP_VALID_TIME		60 * 1000	// 黑名单有效时间（毫秒）

namespace easygame {

	// 比较简单的网络攻击检测和防御
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

		// 定时清理黑名单
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
		// IP黑名单
		BlacklistMap mIpBlacklist;
	};


}	// namespace


#endif