#ifndef __BASE_DEFINE_H__
#define __BASE_DEFINE_H__

// 只存放一些常量的定义 
#define HEADER_SIZE	2

// 消息标识的大小
#define CMD_FLAG_SIZE	1

// 用户封包，单个包的最大字节数(HEAD+消息)
#define MAX_USER_PACK_SIZE	64 * 1024

// 用户封包，单个消息的最大字节数(消息)
#define MAX_USER_CMD_SIZE	MAX_USER_PACK_SIZE - HEADER_SIZE

// 接收缓存数据大小
#define MAX_USER_SEND_SIZE	1 * 1024
#define MAX_USER_RECV_SIZE	4 * 1024

#ifndef MAX_PATH
#define MAX_PATH	260
#endif

#endif

