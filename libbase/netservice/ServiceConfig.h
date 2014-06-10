#pragma once

struct stServiceConfig 
{
	// ���������
	uint serverid;
	uint port;
	uint maxClient;

	// ע����
	uint autoRegiter;

	// ���ݿ����
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


