#ifndef __MSG_HANDLER_BASE_H__
#define __MSG_HANDLER_BASE_H__
#include "BaseCmd.h"
#include "NetEventArgs.h"
#include "ProtobufDefine.h"

// 注册网络事件处理函数(消息号和处理函数的绑定)
bool bindMsgHandler(cmd_type cmd, para_type para, ProtoMessage& msg, const Subscriber* subscriber);

Subscriber* getMsgHandler(ushort cmdID);

//// 处理服务器发来的数据包
//void handlerSocketPacket();

// 释放所有消息处理器
// 在游戏退出之前，先关闭所有的网络连接，然后调用这个函数，释放消息处理器
void releaseMsgHandler();

//////////////////////////////////////////////////////////////////////////
// 注册发送消息
void regProtoMsg(cmd_type cmd, para_type para, ProtoMessage& msg);

// 通过protobuf结构名查找出消息
stBaseCmd findCmdByMsgName(const string& typeName);


//////////////////////////////////////////////////////////////////////////


// 自动执行开始定义
#define AUTORUN_BEGIN(uname)		    \
class uname { public: uname(); }uname;	uname::uname() {


// 自动执行结束定义
#define AUTORUN_END()	}


#define REG_PROTOMSG(msgID, protoCls)	\
{		\
	protoCls tmp;	\
	::regProtoMsg(msgID, tmp);	\
}		\



// 绑定全局函数
#define BIND_FUNC(msgID, protoCls)	\
{           \
	protoCls tmp;	\
	::bindMsgHandler(msgID, tmp, new Subscriber(&handle##protoCls));  \
}            \


// 绑定类成员函数(对象在宏里默认用this，减少代码量)
#define BIND_MEMBER_FUNC(msgID, cls, protoCls)	\
{           \
	protoCls tmp;	\
	::bindMsgHandler(msgID, tmp, new Subscriber(&cls::handle##protoCls,this));  \
}            \


#endif