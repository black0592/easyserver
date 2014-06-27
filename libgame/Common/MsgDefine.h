#ifndef __MSG_DEFINE_H__
#define __MSG_DEFINE_H__
#include "ProtoMsgType.pb.h"
using namespace Protocol;

// 登录消息
#define CMD_LOGIN(op)			MSG(MSG_LOGIN, op)

// 角色自己的消息
#define CMD_SELECTCHAR(op)		MSG(MSG_SELECTCHAR, op)

// 角色自己的消息
#define CMD_CHARDATA(op)		MSG(MSG_CHARDATA, op)

// 其他通用(错误通知、游戏参数)
#define CMD_GENERAL(op)			MSG(MSG_GENERAL, op)


#endif



