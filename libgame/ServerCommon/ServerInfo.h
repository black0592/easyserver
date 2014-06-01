#ifndef __SERVERINFO_H__
#define __SERVERINFO_H__


// 服务器名称长度
#define SERVERNAME_LEN	50

// 搜索最优服务器时，可以采用服务器状态做为优先队列的键
enum ServerState
{
	SS_Free,		// 很闲
	SS_Normal,		// 正常
	SS_Busy,		// 非常忙
	SS_Close,		// 关服维护
};

enum ServerType
{
	AllServer			= 0,		// 所有服务器
	SuperServer			= 1,		// 超级集群管理器(全局只有一个，管理整个游戏的所有大区)
	ZoneServer			= 2,		// 区主服务器(一个大区一个，管理当前区的所有功能服务器)
	GatewayServer		= 3,		// 网关服务器(负责转发消息，处理链接，支持动态加减)
	LoginServer			= 4,		// 登陆服务器(可整个游戏一个，也可支持多个，通过dns可支持动态)
	GameServer			= 5,		// 游戏逻辑服务器(一个区可多个，按地图分)
	SessionServer		= 6,		// session服务器(一个区一个，全局功能)
	LoginDBServer		= 7,		// 账号数据库代理
	GameDBServer		= 8,		// 游戏逻辑数据库代理
	MaxServerType,
};

__inline bool isValidServerType(ServerType svrType)
{
	return (svrType > AllServer || svrType < MaxServerType);
}


// 通过ip和端口，可以给服务器生成一个唯一ID
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
	uint64 uniqueId;	// 唯一ID(IP和端口组成的数字ID)
	uint serverId;		// 标识ID（主要用在地图的分布式处理）
	char name[SERVERNAME_LEN+1];
	byte type;			// ServerType
	int ip;				// 所在的IP地址(局域网IP或外网IP)
	ushort port;		// 对外服务的端口号
	int onlineNum;		// 当前在线人数
	int maxNum;			// 这个服支持的最大人数限制

	stServerInfo()
	{
		bzero(this, sizeof(*this));
	}
};

#pragma pack()
*/

#endif