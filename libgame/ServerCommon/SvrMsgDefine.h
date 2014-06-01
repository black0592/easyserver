#ifndef __SVR_MSG_DEFINE_H__
#define __SVR_MSG_DEFINE_H__
#include "ProtoSvrMsgType.pb.h"
using namespace ServerProtocol;

// super服务
#define CMD_SUPER_SERVICE(op)		MSG(SVR_MSG_SUPER_SERVICE, op)

// zone服务
#define CMD_ZONE_SERVICE(op)		MSG(SVR_MSG_ZONE_SERVICE, op)

// login服务
#define CMD_LOGIN_SERVICE(op)		MSG(SVR_MSG_LOGIN_SERVICE, op)

// gateway服务(服务于客户端，貌似不用内部服务)
#define CMD_GATEWAY_SERVICE(op)		MSG(SVR_MSG_GATEWAY_SERVICE, op)

// game服务
#define CMD_GAME_SERVICE(op)		MSG(SVR_MSG_GAME_SERVICE, op)

// session服务
#define CMD_SESSION_SERVICE(op)		MSG(SVR_MSG_SESSION_SERVICE, op)

// loginDB服务
#define CMD_LOGINDB_SERVICE(op)		MSG(SVR_MSG_LOGINDB_SERVICE, op)

// gameDB服务
#define CMD_GAMEDB_SERVICE(op)		MSG(SVR_MSG_GAMEDB_SERVICE, op)



#endif



