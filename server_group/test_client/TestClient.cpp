#include "ClientHeader.h"
#include "TestClient.h"
#include "Define.h"


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

void TestClient::waiting()
{
	mState = State_Waiting;
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



	return true;
}

