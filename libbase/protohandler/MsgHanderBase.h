#ifndef __MSG_HANDLER_BASE_H__
#define __MSG_HANDLER_BASE_H__
#include "BaseCmd.h"
#include "NetEventArgs.h"
#include "ProtobufDefine.h"

// ע�������¼�������(��Ϣ�źʹ������İ�)
bool bindMsgHandler(cmd_type cmd, para_type para, ProtoMessage& msg, const Subscriber* subscriber);

Subscriber* getMsgHandler(ushort cmdID);

//// ������������������ݰ�
//void handlerSocketPacket();

// �ͷ�������Ϣ������
// ����Ϸ�˳�֮ǰ���ȹر����е��������ӣ�Ȼ���������������ͷ���Ϣ������
void releaseMsgHandler();

//////////////////////////////////////////////////////////////////////////
// ע�ᷢ����Ϣ
void regProtoMsg(cmd_type cmd, para_type para, ProtoMessage& msg);

// ͨ��protobuf�ṹ�����ҳ���Ϣ
stBaseCmd findCmdByMsgName(const string& typeName);


//////////////////////////////////////////////////////////////////////////


// �Զ�ִ�п�ʼ����
#define AUTORUN_BEGIN(uname)		    \
class uname { public: uname(); }uname;	uname::uname() {


// �Զ�ִ�н�������
#define AUTORUN_END()	}


#define REG_PROTOMSG(msgID, protoCls)	\
{		\
	protoCls tmp;	\
	::regProtoMsg(msgID, tmp);	\
}		\



// ��ȫ�ֺ���
#define BIND_FUNC(msgID, protoCls)	\
{           \
	protoCls tmp;	\
	::bindMsgHandler(msgID, tmp, new Subscriber(&handle##protoCls));  \
}            \


// �����Ա����(�����ں���Ĭ����this�����ٴ�����)
#define BIND_MEMBER_FUNC(msgID, cls, protoCls)	\
{           \
	protoCls tmp;	\
	::bindMsgHandler(msgID, tmp, new Subscriber(&cls::handle##protoCls,this));  \
}            \


#endif