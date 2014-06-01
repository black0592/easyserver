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


// ��ǰ�������汾
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
		//printf("[%u]������Ϸ�߼�\n", mIndex);

#if 0
		RequestCheckPing cmd;
		cmd.set_timestamp( Platform::tick() );
		sendProtoMsg(cmd, CMD_CHARDATA(OP_CHECK_PING));
#endif

#if 1

		if (true) {
			printf("[%u]ִ�����Ѱ·\n", mIndex);

			Map* pMap = MapManager::getInstance().getMap(mCharBase.mapid());
			if (pMap == NULL) {
				printf("[%u]��Ч�ĵ�ͼ[%u]��û�ҵ���\n", mIndex, mCharBase.mapid());
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
		// ���µ�¼
		if (rand_reconnect > 0) {
			if (rand() % rand_reconnect == 0) {
				printf("[%u]<<<ִ���������>>>\n", mIndex);
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
	// �Ⱥ��Ϸ������Ͽ�����
	close();

	printf("[%u]���� silicon.3721play.com 7102\n", mIndex);

	// ���������ط�����
	bool success = connect("silicon.3721play.com", 7102);
	if (!success) {
		// ���ӵ�¼������ʧ�ܣ����½�����������
		mState = State_Register;
		return;
	}

#if 0
	batchRegister();
	return;
#endif

	// ��һ�����˺�������ע������ʺ�

	// ǧ���˺�
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
	// ѡ���һ���������еķ�����
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
	// �Ⱥ͵�¼�������Ͽ�����
	close();

	// ���������ط�����
	bool success = connect(mGatewayIP.c_str(), mGatewayPort);
	if (!success) {
		// �������ط�����ʧ�ܣ����½����¼����
		mState = State_Login;
		return;
	}

	requestCharList();

	waiting();
}

void TestClient::requestCharList()
{
	// ����������سɹ����������ɫ�б�
	RequestGetCharList cmd;
	cmd.set_version(SERVER_VERSION);
	cmd.set_account(mAccount);
	cmd.set_login_key(mLoginKey);
	sendProtoMsg(cmd, CMD_SELECTCHAR(OP_GET_CHARLIST));
}


void TestClient::updateSelectChar()
{
	if (mCharList.empty()) {
		// ������ɫ
		RequestCreateChar cmd;
		cmd.set_charname(mAccount);
		cmd.set_country(1+rand()%2);		// �������
		cmd.set_race(rand()%6);	// ���ְҵ
		sendProtoMsg(cmd, CMD_SELECTCHAR(OP_CREATE_CHAR)); 
	} else {
		// ѡ���һ��������Ϸ
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

		// �Զ�������ע�ᵽSuper������
		if ( !isConnected() ) {
			if ( !reconnect() ) {
				LOGE("������Ϸ������ʧ��,ip=%s, port=%d", getIP(), getPort());
				return true;
			}

			//registerToSuperServer();
			LOGI("[%u]������Ϸ�������ɹ���,ip=%s, port=%d", mIndex, getIP(), getPort());
		}
	}
	*/

	mainLoop();

	return true;
}


bool TestClient::handleProtoMsg(const EventArgs& args)
{
	stBaseCmd* pCmd = Event2BaseCmd(args);

	// ��Ϣ���ܼ�¼
	char log[50];
	sprintf(log, "��Ϣִ��ʱ�����para=%d",pCmd->para);
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
		// �˺ŷ�����
	case OP_LOGIN_REGISTER:
		{
			ResponseRegister rev;
			Event2Proto(args, rev);

			if ( rev.result() == REGISTER_ERROR_SUCCESS) {
				LOGD("�˺� %s ע��ɹ�", mAccount.c_str());
			} else {
				//LOGE("�˺� %s ע��ʧ��", mAccount.c_str());
			}

			// ע���ֱ�ӽ����¼����
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
						// ֻ�����������еķ�������ά���Ĳ��ü���
						mZoneList.push_back( zoneInfo );
					}
				}

				mState = State_SelZone;
			} else {
				// ���µ�¼
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
				// ���µ�¼
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
		// ���ط�����
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
				// ���µ�¼
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

			// ���ص�ͼ
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
		// ��ɫ����

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
				// ���µ�¼
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
		// ��ɫ����

	case OP_FRIEND_GET_LIST:
		{
			ResponseFriendList rev;
			Event2Proto(args, rev);

			MYTRACE("�յ������б���ֵ");
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

			MYTRACE("�յ���Ӻ��ѷ���ֵ");
			const RelationsInfo& info = rev.info();
			MYTRACE("[%u]name=%s,race=%u,level=%u,map=%u,time=%u,online=%u", 
				info.charid(), info.name().c_str(), info.race(), info.level(), info.mapid(), info.time(), info.online());
		}
		break;

	case OP_FRIEND_REMOVE:
		{
			ResponseRemoveFriend rev;
			Event2Proto(args, rev);

			MYTRACE("�յ�ɾ�����ѷ���ֵ");
			MYTRACE("id=%u", rev.charid());
		}
		break;

	case OP_FRIEND_ADD_TIPS:
		{
			NotifyAddFriendTips rev;
			Event2Proto(args, rev);

			MYTRACE("�յ���Ӻ�����ʾ��Ϣ");
			MYTRACE("%s ����Ϊ���ѣ� �Ƿ����Ϊ����?", rev.name().c_str());
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
		// �ű�

	case OP_SCRIPT_UPDATE_NPC_DIALOG:
		{
			NotifyUpdateNpcDialog rev;
			Event2Proto(args, rev);

			// ��ӡ����
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
		// ������Ϣ

	case OP_CHAR_CHANGE_MAP:
		{
			NotifyCharChangeMap Noti;
			Event2Proto(args,Noti);

			mCharBase.set_mapid( Noti.map_id() );
			mCharBase.set_x( Noti.map_x() );
			mCharBase.set_y( Noti.map_y() );

			// ���ص�ͼ
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