#ifndef __MSG_DEFINE_H__
#define __MSG_DEFINE_H__
#include "ProtoMsgType.pb.h"
using namespace Protocol;

// ��¼��Ϣ
#define CMD_LOGIN(op)			MSG(MSG_LOGIN, op)

// ��ɫ�Լ�����Ϣ
#define CMD_SELECTCHAR(op)		MSG(MSG_SELECTCHAR, op)

// ��ɫ�Լ�����Ϣ
#define CMD_CHARDATA(op)		MSG(MSG_CHARDATA, op)

// ����ͨ��(����֪ͨ����Ϸ����)
#define CMD_GENERAL(op)			MSG(MSG_GENERAL, op)


#endif



