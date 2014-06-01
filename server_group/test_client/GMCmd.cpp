#include "ClientHeader.h"
#include "GMCmd.h"
#include "TestClient.h"
#include "Define.h"
#include "Map.h"
#include "ProtoRelations.pb.h"
#include "ProtoScript.pb.h"
using namespace Protocol;



stGmCmd g_gmFunc[] = 
{
	{"friend",		GMCmdProcesser::relationsFriend},
	{"enemy",		GMCmdProcesser::relationsEnemy},
	{"talk",		GMCmdProcesser::talk},
	{"answer",		GMCmdProcesser::answer},
};


bool GMCmdProcesser::talk(TestClient* user, vector<string>& params)
{
	RequestOpenNpcDialog cmd;

	if (params.size() > 0) {
		uint npcUID = atoi(params[0].c_str());
		cmd.set_npc_uid(npcUID);
	}

	user->sendProtoMsg(cmd, CMD_SCRIPT(OP_SCIRPT_OPEN_NPC_DIALOG));
	return true;
}

bool GMCmdProcesser::answer(TestClient* user, vector<string>& params)
{
	RequestChooseAnswer cmd;

	if (params.size() > 0) {
		uint index = atoi(params[0].c_str());
		cmd.set_answer_index(index);
	}

	user->sendProtoMsg(cmd, CMD_SCRIPT(OP_SCRIPT_CHOOSE_ANSWER));
	return true;
}


bool GMCmdProcesser::relationsFriend(TestClient* user, vector<string>& params)
{
	if (params.size() < 2) {
		// 请求列表
		RequestFriendList cmd;
		user->sendProtoMsg(cmd, CMD_RELATIONS(OP_FRIEND_GET_LIST));
		return false;
	}

	string param1 = params[1];

	if (params[0] == "add") {
		RequestAddFriend cmd;
		cmd.set_name(param1);
		user->sendProtoMsg(cmd, CMD_RELATIONS(OP_FRIEND_ADD));
	}

	if (params[0] == "remove") {
		RequestRemoveFriend cmd;
		cmd.set_charid( atoi(param1.c_str()) );
		user->sendProtoMsg(cmd, CMD_RELATIONS(OP_FRIEND_REMOVE));
	}

	return true;
}

bool GMCmdProcesser::relationsEnemy(TestClient* user, vector<string>& params)
{
	if (params.size() < 2) {
		// 请求列表
		RequestEnemyList cmd;
		user->sendProtoMsg(cmd, CMD_RELATIONS(OP_ENEMY_GET_LIST));
		return false;
	}

	string param1 = params[1];

	if (params[0] == "remove") {
		RequestRemoveEnemy cmd;
		cmd.set_charid( atoi(param1.c_str()) );
		user->sendProtoMsg(cmd, CMD_RELATIONS(OP_ENEMY_REMOVE));
	}

	return true;
}



//////////////////////////////////////////////////////////////////////////

bool GMCmdProcesser::m_isInit = false;
map<string,stGmCmd> GMCmdProcesser::m_gmFuncMap;

easygame::Mutex GMCmdProcesser::mMutex;
list<string> GMCmdProcesser::mCommandList;

vector<TestClient*> GMCmdProcesser::mClientList;

GMCmdProcesser::GMCmdProcesser(void)
{
}

GMCmdProcesser::~GMCmdProcesser(void)
{
}

void GMCmdProcesser::pushTestClient(TestClient* client)
{
	mClientList.push_back(client);
}

void GMCmdProcesser::clearTestClient()
{
	mClientList.clear();
}

void GMCmdProcesser::pushCmd(const char* message)
{
	mMutex.lock();
	mCommandList.push_back(message);
	mMutex.unlock();
}

void GMCmdProcesser::execCommandList()
{
	if (mMutex.trylock()) {

		if (!mCommandList.empty()) {
			const string& message = mCommandList.front();


			for (size_t i=0; i<mClientList.size(); i++) {
				execGmCmd(mClientList[i], message.c_str());
			}
			
			
			mCommandList.pop_front();
		}

		mMutex.unlock();
	}
}

bool GMCmdProcesser::execGmCmd(TestClient* user, const char* message)
{
	if (!m_isInit) {
		for (int i=0; i<sizeof(g_gmFunc)/sizeof(stGmCmd); ++i) {
			m_gmFuncMap[g_gmFunc[i].strCmd] = g_gmFunc[i];
		}
		m_isInit = true;
	}

	vector<string> vecTok;
	splitString(vecTok, string(message), " \t");

	if (vecTok.empty())
		return false;

	string strCmd = vecTok[0];

	map<string,stGmCmd>::iterator itFind = m_gmFuncMap.find(strCmd);
	if (itFind == m_gmFuncMap.end())
		return false;

	stGmCmd& gmCmd = itFind->second;

	vecTok.erase(vecTok.begin());
	gmCmd.func(user, vecTok);

	return true;
}
