#ifndef __TEST_CLIENT_H__
#define __TEST_CLIENT_H__

//enum ConnectState
//{
//	Connect_None,	// 无链接
//	Connect_Login,	// 链接账号服务器
//	Connect_Game,	// 链接游戏服务器
//};


enum ClientState
{
	State_Waiting,
	State_Register,	// 注册帐号
	State_Login,	// 登录
	State_SelZone,	// 选区
	State_EnterZone,// 进入游戏服务器
	State_SelChar,	// 选角色
	State_StartGame,// 开始游戏(激活)
	State_GameLogic,
};

class TestClient : 
	public ProtoClient,
	public TimerEvent,
	public Singleton<TestClient>
{
public:
	TestClient();
	virtual ~TestClient();

	void setIndex(uint index) { mIndex = index; }
	uint getIndex() { return mIndex; }

	//void setConnectState(ConnectState state) { mConnState = state; }
	//uint getConnectState() { return mConnState; }

	void setState(ClientState state) { mState = state; }
	uint getState() { return mState; }

	virtual bool handleProtoMsg(const EventArgs& args);

protected:
	virtual bool onTimer();



private:
	void waiting();

private:
	uint mIndex;
	//ConnectState mConnState;
	ClientState mState;
	ElapseTimer mOneMillTimer;
	ElapseTimer mOneSecTimer;

	string mAccount;
	string mPassword;
	uint mLoginKey;
	uint mCharId;

	string mGatewayIP;
	uint mGatewayPort;
};


#endif