#ifndef __SERVERINFO_H__
#define __SERVERINFO_H__


// ���������Ƴ���
#define SERVERNAME_LEN	50

// �������ŷ�����ʱ�����Բ��÷�����״̬��Ϊ���ȶ��еļ�
enum ServerState
{
	SS_Free,		// ����
	SS_Normal,		// ����
	SS_Busy,		// �ǳ�æ
	SS_Close,		// �ط�ά��
};

enum ServerType
{
	AllServer			= 0,		// ���з�����
	SuperServer			= 1,		// ������Ⱥ������(ȫ��ֻ��һ��������������Ϸ�����д���)
	ZoneServer			= 2,		// ����������(һ������һ��������ǰ�������й��ܷ�����)
	GatewayServer		= 3,		// ���ط�����(����ת����Ϣ���������ӣ�֧�ֶ�̬�Ӽ�)
	LoginServer			= 4,		// ��½������(��������Ϸһ����Ҳ��֧�ֶ����ͨ��dns��֧�ֶ�̬)
	GameServer			= 5,		// ��Ϸ�߼�������(һ�����ɶ��������ͼ��)
	SessionServer		= 6,		// session������(һ����һ����ȫ�ֹ���)
	LoginDBServer		= 7,		// �˺����ݿ����
	GameDBServer		= 8,		// ��Ϸ�߼����ݿ����
	MaxServerType,
};

__inline bool isValidServerType(ServerType svrType)
{
	return (svrType > AllServer || svrType < MaxServerType);
}


// ͨ��ip�Ͷ˿ڣ����Ը�����������һ��ΨһID
__inline uint64 server_hash_id(int ip, ushort port)
{
	return ( ((uint64)ip << 32) | (uint64)port );
}

__inline int get_server_ip(uint64 hashId)
{
	return ( (hashId & 0xFFFFFFFF00000000) >> 32 );
}

__inline ushort get_server_port(uint64 hashId)
{
	return ( (hashId & 0x000000000000FFFF) );
}

__inline string serverTypeToName(ServerType type)
{
	static string serverNames[MaxServerType] = 
	{
		"all","super","zone","gateway","login","game","session","logindb","gamedb"
	};

	return serverNames[type];
}


/*
#pragma pack(1)

struct stServerInfo 
{
	uint64 uniqueId;	// ΨһID(IP�Ͷ˿���ɵ�����ID)
	uint serverId;		// ��ʶID����Ҫ���ڵ�ͼ�ķֲ�ʽ����
	char name[SERVERNAME_LEN+1];
	byte type;			// ServerType
	int ip;				// ���ڵ�IP��ַ(������IP������IP)
	ushort port;		// �������Ķ˿ں�
	int onlineNum;		// ��ǰ��������
	int maxNum;			// �����֧�ֵ������������

	stServerInfo()
	{
		bzero(this, sizeof(*this));
	}
};

#pragma pack()
*/

#endif