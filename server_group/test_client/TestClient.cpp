#include "ClientHeader.h"
#include "TestClient.h"
#include "Define.h"
#include "Map.h"
#include "ProtoGeneral.pb.h"
#include "ProtoRelations.pb.h"
#include "ProtoScript.pb.h"

void MYTRACE(const char* format, ...)
{
	char buffer[1024] = {0};
	va_list argptr;				
	va_start(argptr, format);	
	vsnprintf( buffer, sizeof(buffer), format, argptr );	
	va_end(argptr); 
	printf("%s\n", buffer);
}


// 当前服务器版本
#define	SERVER_VERSION		145
#define RAND_RELOGIN		1



TestClient::TestClient()
	: TimerEvent(0, false)
	, mOneMillTimer(10)
	, mOneSecTimer(800)
	, ProtoClient("TestClient")
{
	mLoginKey = 0;
	mIndex = 0;
	mCharId = 0;
	//mConnState = Connect_None;
	mState = State_Register;
}

TestClient::~TestClient()
{

}

void TestClient::updateGameLogic()
{
	//return;

	if (mOneSecTimer.isTimeout()) {
		//printf("[%u]进入游戏逻辑\n", mIndex);

#if 0
		RequestCheckPing cmd;
		cmd.set_timestamp( Platform::tick() );
		sendProtoMsg(cmd, CMD_CHARDATA(OP_CHECK_PING));
#endif

#if 1

		if (true) {
			printf("[%u]执行随机寻路\n", mIndex);

			Map* pMap = MapManager::getInstance().getMap(mCharBase.mapid());
			if (pMap == NULL) {
				printf("[%u]无效的地图[%u]，没找到啊\n", mIndex, mCharBase.mapid());
				return;
			}

			SIZE mapSize = pMap->getMapSizePerPixel();

			int curX = mCharBase.x();
			int curY = mCharBase.y();

			int dirX = rand() % 2;
			int dirY = rand() % 2;

			if (dirX == 0)
				curX += CXTILE;
			else
				curX -= CXTILE;

			if (dirY == 0)
				curY += CYTILE;
			else
				curY -= CYTILE;

			if (curX < 0) curX = 0;
			if (curX >= mapSize.cx) curX = mapSize.cx - CXTILE;

			if (curY < 0) curY = 0;
			if (curY >= mapSize.cy) curY = mapSize.cy - CYTILE;


			uint gridX = curX / CXTILE;
			uint gridY = curY / CYTILE;
			

			if ( !pMap->isCanMove(gridX, gridY, true) ) {
				return;

				/*
				stTILE* tile = pMap->getSideTile(gridX, gridY);
				if (tile == NULL)
					return;

				gridX = tile->pos.x;
				gridY = tile->pos.y;
				*/
			}

			uint nextPixelX = gridX * CXTILE;
			uint nextPixelY = gridY * CYTILE;

			mCharBase.set_x(nextPixelX);
			mCharBase.set_y(nextPixelY);

			/*MsgCharMove cmd;
			cmd.set_x(nextPixelX);
			cmd.set_y(nextPixelY);
			cmd.set_flag(1);
			sendProtoMsg(cmd, CMD_SCENEDATA(OP_SCENE_CHAR_MOVE));*/

			MsgCharStopMove cmdStop;
			cmdStop.set_x(nextPixelX);
			cmdStop.set_y(nextPixelY);
			sendProtoMsg(cmdStop, CMD_SCENEDATA(OP_SCENE_CHAR_STOP_MOVE));
		}
#endif


#if RAND_RELOGIN
		// 重新登录
		if (rand_reconnect > 0) {
			if (rand() % rand_reconnect == 0) {
				printf("[%u]<<<执行随机重连>>>\n", mIndex);
				close();
				mState = State_Register;
			}
		}
#endif
	}
}

void TestClient::waiting()
{
	mState = State_Waiting;
}

void TestClient::mainLoop()
{
	if (mState == State_Register) {
		updateRegister();
	}

	if (mState == State_Login) {
		updateLogin();
	}

	if (mState == State_SelZone) {
		updateSelectZone();
	}

	if (mState == State_EnterZone) {
		updateEnterZone();
	}

	if (mState == State_SelChar) {
		updateSelectChar();
	}

	if (mState == State_StartGame) {
		updateStartGame();
	}

	if (mState == State_GameLogic) {
		updateGameLogic();
	}
	
}

void TestClient::updateRegister()
{
	// 先和老服务器断开连接
	close();

	printf("[%u]连接 silicon.3721play.com 7102\n", mIndex);

	// 再链接网关服务器
	bool success = connect("silicon.3721play.com", 7102);
	if (!success) {
		// 链接登录服务器失败，重新进入链接流程
		mState = State_Register;
		return;
	}

#if 0
	batchRegister();
	return;
#endif

	// 用一定的账号名规则，注册测试帐号

	// 千万账号
	char szAcc[250];
	sprintf(szAcc, "test_%08d", mIndex);
	mAccount = szAcc;
	mPassword = "1";

	RequestRegister cmd;
	cmd.set_account(mAccount);
	cmd.set_password(mPassword);
	cmd.set_email("test@qq.com");

	sendProtoMsg(cmd, CMD_LOGIN(OP_LOGIN_REGISTER));

	waiting();
}

void TestClient::batchRegister()
{
	for (uint i=0; i<99999999; i++) {
		char szAcc[250];
		sprintf(szAcc, "test_%08d", i);
		mAccount = szAcc;
		mPassword = "1";

		RequestRegister cmd;
		cmd.set_account(mAccount);
		cmd.set_password(mPassword);
		cmd.set_email("test@qq.com");

		sendProtoMsg(cmd, CMD_LOGIN(OP_LOGIN_REGISTER));
	}

	mState = State_Login;
}

void TestClient::updateLogin()
{
	RequestLogin cmd;
	cmd.set_account(mAccount);
	cmd.set_password(mPassword);
	cmd.set_version(SERVER_VERSION);
	sendProtoMsg(cmd, CMD_LOGIN(OP_LOGIN_LOGIN));

	waiting();
}

void TestClient::updateSelectZone()
{
	// 选择第一个正常运行的服务器
	const ZoneInfo& zoneInfo = mZoneList[0];

	RequestEnterServer cmd;
	cmd.set_account(mAccount);
	cmd.set_login_key(mLoginKey);
	cmd.set_zoneid(zoneInfo.id());
	sendProtoMsg(cmd, CMD_LOGIN(OP_LOGIN_ENTER_SERVER));

	waiting();
}

void TestClient::updateEnterZone()
{
	// 先和登录服务器断开连接
	close();

	// 再链接网关服务器
	bool success = connect(mGatewayIP.c_str(), mGatewayPort);
	if (!success) {
		// 链接网关服务器失败，重新进入登录流程
		mState = State_Login;
		return;
	}

	requestCharList();

	waiting();
}

void TestClient::requestCharList()
{
	// 如果链接网关成功，就请求角色列表
	RequestGetCharList cmd;
	cmd.set_version(SERVER_VERSION);
	cmd.set_account(mAccount);
	cmd.set_login_key(mLoginKey);
	sendProtoMsg(cmd, CMD_SELECTCHAR(OP_GET_CHARLIST));
}


void TestClient::updateSelectChar()
{
	if (mCharList.empty()) {
		// 创建角色
		RequestCreateChar cmd;
		cmd.set_charname(mAccount);
		cmd.set_country(1+rand()%2);		// 随机国家
		cmd.set_race(rand()%6);	// 随机职业
		sendProtoMsg(cmd, CMD_SELECTCHAR(OP_CREATE_CHAR)); 
	} else {
		// 选择第一个进入游戏
		mCharId = mCharList[0].charid();
		RequestEnterGame cmd;
		cmd.set_charid(mCharId);
		sendProtoMsg(cmd, CMD_SELECTCHAR(OP_ENTER_GAME));
	}

	waiting();
}

void TestClient::updateStartGame()
{
	RequestStartGame cmd;
	sendProtoMsg(cmd, CMD_CHARDATA(OP_START_GAME));
	
	waiting();
}


//////////////////////////////////////////////////////////////////////////

bool TestClient::onTimer()
{
	doMsgQueue();

	/*
	if (mOneMillTimer.isTimeout()) {

#if 0
		int temp = rand() % 3;
		if (temp == 0) {
			close();
		}
#endif

		// 自动重连并注册到Super服务器
		if ( !isConnected() ) {
			if ( !reconnect() ) {
				LOGE("连接游戏服务器失败,ip=%s, port=%d", getIP(), getPort());
				return true;
			}

			//registerToSuperServer();
			LOGI("[%u]连接游戏服务器成功！,ip=%s, port=%d", mIndex, getIP(), getPort());
		}
	}
	*/

	mainLoop();

	return true;
}


bool TestClient::handleProtoMsg(const EventArgs& args)
{
	stBaseCmd* pCmd = Event2BaseCmd(args);

	// 消息性能记录
	char log[50];
	sprintf(log, "消息执行时间过长para=%d",pCmd->para);
	FUNC_PF_EXT(log,5);

	switch (pCmd->cmd)
	{
	case MSG_GENERAL:
		handleErrorMsg(args);
		break;
	case MSG_LOGIN:
		handleLoginMsg(args);
		break;
	case MSG_SELECTCHAR:
		handleSelCharMsg(args);
		break;
	case MSG_CHARDATA:
		handleCharDataMsg(args);
		break;
	case MSG_RELATIONS:
		handleRelationsMsg(args);
		break;
	case MSG_SCRIPT:
		handleScriptMsg(args);
		break;
	case MSG_GAMEDATA:
		handleSceneMsg(args);
		break;
	}

	return true;
}

bool TestClient::handleErrorMsg(const EventArgs& args)
{
	stBaseCmd* pCmd = Event2BaseCmd(args);

	switch (pCmd->para)
	{
	case OP_GENERAL_ERROR:
		{
			NotifyGeneralError rev;
			Event2Proto(args, rev);

			string gbkStr = Platform::utf8ToGbk( rev.content() );

			LOGE(gbkStr.c_str());
		}
		break;
	}

	return true;
}

bool TestClient::handleLoginMsg(const EventArgs& args)
{
	stBaseCmd* pCmd = Event2BaseCmd(args);

	switch (pCmd->para)
	{
		//////////////////////////////////////////////////////////////////////////
		// 账号服务器
	case OP_LOGIN_REGISTER:
		{
			ResponseRegister rev;
			Event2Proto(args, rev);

			if ( rev.result() == REGISTER_ERROR_SUCCESS) {
				LOGD("账号 %s 注册成功", mAccount.c_str());
			} else {
				//LOGE("账号 %s 注册失败", mAccount.c_str());
			}

			// 注册后，直接进入登录流程
			mState = State_Login;
		}
		break;
	case OP_LOGIN_LOGIN:
		{
			ResponseLogin rev;
			Event2Proto(args, rev);

			if (rev.result() == LOGIN_ERROR_SUCCESS) {
				mLoginKey = rev.login_key();
				const RepeatedPtrField<ZoneInfo>& list = rev.zone_list();
				for (int i=0; i<list.size(); i++) {
					const ZoneInfo& zoneInfo = list.Get(i);
					if (zoneInfo.state() != ZoneInfo::ZS_STOPED) {
						// 只加入正常运行的服务器，维护的不用加了
						mZoneList.push_back( zoneInfo );
					}
				}

				mState = State_SelZone;
			} else {
				// 重新登录
				mState = State_Login;
			}
		}
		break;
	case OP_LOGIN_ENTER_SERVER:
		{
			ResponseEnterServer rev;
			Event2Proto(args, rev);

			if (rev.result() == ENTER_ZONE_ERROR_SUCCESS) {
				mGatewayIP = rev.ip();
				mGatewayPort = rev.port();

				mState = State_EnterZone;
			} else {
				// 重新登录
				mState = State_Login;
			}
		}
		break;
	}


	return true;
}

bool TestClient::handleSelCharMsg(const EventArgs& args)
{
	stBaseCmd* pCmd = Event2BaseCmd(args);

	switch (pCmd->para)
	{
		//////////////////////////////////////////////////////////////////////////
		// 网关服务器
	case OP_GET_CHARLIST:
		{
			ResponseGetCharList rev;
			Event2Proto(args, rev);

			if (rev.result() == RESULT_OK) {
				mCharList.clear();
				const RepeatedPtrField<SelectCharInfo>& list = rev.char_list();
				for (int i=0; i<list.size(); i++) {
					const SelectCharInfo& charInfo = list.Get(i);
					if (charInfo.charid() > 0) {
						mCharList.push_back( charInfo );
					}
				}

				mState = State_SelChar;
			} else {
				// 重新登录
				mState = State_Login;
			}
		}
		break;
	case OP_CREATE_CHAR:
		{
			requestCharList();

			waiting();
		}
		break;
	case OP_ENTER_GAME:
		{
			ResponseEnterGame rev;
			Event2Proto(args, rev);

			mCharBase = rev.charbase();
			mCharBin = rev.bindata();

			// 加载地图
			MapManager::getInstance().loadMap(mCharBase.mapid());

			mState = State_StartGame;
		}
		break;
	}


	return true;
}

bool TestClient::handleCharDataMsg(const EventArgs& args)
{
	stBaseCmd* pCmd = Event2BaseCmd(args);

	switch (pCmd->para)
	{
		//////////////////////////////////////////////////////////////////////////
		// 角色数据

	case OP_CHECK_PING:
		{
			ResponseCheckPing rev;
			Event2Proto(args, rev);

			uint time = Platform::tick() - rev.timestamp();
			printf("Ping: %u\n", time);
		}
		break;

	case OP_START_GAME:
		{
			ResponseStartGame rev;
			Event2Proto(args, rev);

			if (rev.result() == RESULT_OK) {
				mState = State_GameLogic;
			} else {
				// 重新登录
				mState = State_Login;
			}
		}
		break;

	}


	return true;
}



bool TestClient::handleRelationsMsg(const EventArgs& args)
{
	stBaseCmd* pCmd = Event2BaseCmd(args);

	switch (pCmd->para)
	{
		//////////////////////////////////////////////////////////////////////////
		// 角色数据

	case OP_FRIEND_GET_LIST:
		{
			ResponseFriendList rev;
			Event2Proto(args, rev);

			MYTRACE("收到好友列表返回值");
			const RepeatedPtrField<RelationsInfo>& list = rev.friend_list();
			for (int i=0; i<list.size(); i++) {
				const RelationsInfo& info = list.Get(i);
				MYTRACE("[%u]name=%s,race=%u,level=%u,map=%u,time=%u,online=%u", 
					info.charid(), info.name().c_str(), info.race(), info.level(), info.mapid(), info.time(), info.online());
			}
		}
		break;

	case OP_FRIEND_ADD:
		{
			ResponseAddFriend rev;
			Event2Proto(args, rev);

			MYTRACE("收到添加好友返回值");
			const RelationsInfo& info = rev.info();
			MYTRACE("[%u]name=%s,race=%u,level=%u,map=%u,time=%u,online=%u", 
				info.charid(), info.name().c_str(), info.race(), info.level(), info.mapid(), info.time(), info.online());
		}
		break;

	case OP_FRIEND_REMOVE:
		{
			ResponseRemoveFriend rev;
			Event2Proto(args, rev);

			MYTRACE("收到删除好友返回值");
			MYTRACE("id=%u", rev.charid());
		}
		break;

	case OP_FRIEND_ADD_TIPS:
		{
			NotifyAddFriendTips rev;
			Event2Proto(args, rev);

			MYTRACE("收到添加好友提示信息");
			MYTRACE("%s 加你为好友， 是否加她为好友?", rev.name().c_str());
		}
		break;

	}


	return true;
}



bool TestClient::handleScriptMsg(const EventArgs& args)
{
	stBaseCmd* pCmd = Event2BaseCmd(args);

	switch (pCmd->para)
	{
		//////////////////////////////////////////////////////////////////////////
		// 脚本

	case OP_SCRIPT_UPDATE_NPC_DIALOG:
		{
			NotifyUpdateNpcDialog rev;
			Event2Proto(args, rev);

			// 打印内容
			MYTRACE(rev.content().c_str());
			const RepeatedPtrField<NpcDlgOption>& list = rev.options();
			for (int i=0; i<list.size(); i++) {
				const NpcDlgOption& option = list.Get(i);
				MYTRACE(option.text().c_str());
			}
		}
		break;
	}


	return true;
}

bool TestClient::handleSceneMsg(const EventArgs& args)
{
	stBaseCmd* pCmd = Event2BaseCmd(args);

	switch (pCmd->para)
	{
		//////////////////////////////////////////////////////////////////////////
		// 场景消息

	case OP_CHAR_CHANGE_MAP:
		{
			NotifyCharChangeMap Noti;
			Event2Proto(args,Noti);

			mCharBase.set_mapid( Noti.map_id() );
			mCharBase.set_x( Noti.map_x() );
			mCharBase.set_y( Noti.map_y() );

			// 加载地图
			MapManager::getInstance().loadMap(Noti.map_id());

			mState = State_StartGame;
		}
		break;
	}


	return true;
}


bool TestClient::getDefaultSkill(uint& skillId, uint& skillLevel)
{
	const RepeatedPtrField<SkillData>& skillList = mCharBin.skills();
	if (skillList.size() == 0)
		return false;

	skillId = skillList.Get(0).skill_id();
	skillLevel = skillList.Get(0).level();
	return true;
}