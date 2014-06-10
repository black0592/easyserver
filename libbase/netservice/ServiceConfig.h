#pragma once

struct stServiceConfig 
{
	// 服务器相关
	uint serverid;
	uint port;
	uint maxClient;

	// 注册用
	uint autoRegiter;

	// 数据库相关
	string dbIP;
	uint dbPort;
	string dbName;
	string dbUser;
	string dbPassword;

	stServiceConfig()
	{
		serverid = 0;
		port = 0;
		maxClient = 0;
		autoRegiter = 0;
		dbPort = 0;
	};
};


