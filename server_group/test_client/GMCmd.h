#pragma once
#include "TestClient.h"

// 是否记录经验增长
bool isLogExp(const char* charname);

typedef bool (*GmCmdFunc) (TestClient* user, vector<string>& params);


struct stGmCmd 
{
	string strCmd;
	GmCmdFunc func;
};


class GMCmdProcesser
{
public:
	GMCmdProcesser(void);
	virtual ~GMCmdProcesser(void);

	static bool execGmCmd(TestClient* user, const char* message);

public:
	// 消息队列
	static void pushTestClient(TestClient* client);
	static void clearTestClient();

	static void pushCmd(const char* message);
	static void execCommandList();

public:
	// 社会关系测试
	static bool relationsFriend(TestClient* user, vector<string>& params);
	static bool relationsEnemy(TestClient* user, vector<string>& params);

	// 脚本测试
	static bool talk(TestClient* user, vector<string>& params);
	static bool answer(TestClient* user, vector<string>& params);
	

private:
	static bool m_isInit;
	static map<string,stGmCmd> m_gmFuncMap;

	static easygame::Mutex mMutex;
	static list<string> mCommandList;

	static vector<TestClient*> mClientList;
};
