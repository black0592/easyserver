#ifndef __SERVER_DEFINE_H__
#define __SERVER_DEFINE_H__


//---------------------------------
//---------- 主消息 ---------------

//// 服务器之间的消息定义(以目的服务器命名)
//const cmd_type SUPER_SERVICE	= 1;	// 超级服务器的消息
//const cmd_type ZONE_SERVICE		= 2;	// 区主服务器的消息
//const cmd_type LOGIN_SERVICE	= 3;	// 账号服务器的消息
//const cmd_type GATEWAY_SERVICE	= 4;	// 网关服务器的消息
const cmd_type GAME_SERVICE		= 5;	// PVE逻辑服务器的消息
//const cmd_type SESSION_SERVICE	= 6;	// PVP逻辑服务器的消息
//const cmd_type LOGINDB_SERVICE	= 7;	// 账号数据库代理的消息
const cmd_type GAMEDB_SERVICE	= 8;	// PVE数据库代理的消息


//----------------------------------




#endif