#ifndef __SERVER_SCRUCT_H__
#define __SERVER_STRUCT_H__

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




#endif