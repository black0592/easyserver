#ifndef __SVR_MSG_DEFINE_H__
#define __SVR_MSG_DEFINE_H__
#include "ProtoSvrMsgType.pb.h"
using namespace ServerProtocol;

// super����
#define CMD_SUPER_SERVICE(op)		MSG(SVR_MSG_SUPER_SERVICE, op)

// zone����
#define CMD_ZONE_SERVICE(op)		MSG(SVR_MSG_ZONE_SERVICE, op)

// login����
#define CMD_LOGIN_SERVICE(op)		MSG(SVR_MSG_LOGIN_SERVICE, op)

// gateway����(�����ڿͻ��ˣ�ò�Ʋ����ڲ�����)
#define CMD_GATEWAY_SERVICE(op)		MSG(SVR_MSG_GATEWAY_SERVICE, op)

// game����
#define CMD_GAME_SERVICE(op)		MSG(SVR_MSG_GAME_SERVICE, op)

// session����
#define CMD_SESSION_SERVICE(op)		MSG(SVR_MSG_SESSION_SERVICE, op)

// loginDB����
#define CMD_LOGINDB_SERVICE(op)		MSG(SVR_MSG_LOGINDB_SERVICE, op)

// gameDB����
#define CMD_GAMEDB_SERVICE(op)		MSG(SVR_MSG_GAMEDB_SERVICE, op)



#endif



