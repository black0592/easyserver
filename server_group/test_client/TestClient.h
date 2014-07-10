#ifndef __TEST_CLIENT_H__
#define __TEST_CLIENT_H__

//enum ConnectState
//{
//	Connect_None,	// ������
//	Connect_Login,	// �����˺ŷ�����
//	Connect_Game,	// ������Ϸ������
//};


enum ClientState
{
	State_Waiting,
	State_Register,	// ע���ʺ�
	State_Login,	// ��¼
	State_SelZone,	// ѡ��
	State_EnterZone,// ������Ϸ������
	State_SelChar,	// ѡ��ɫ
	State_StartGame,// ��ʼ��Ϸ(����)
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