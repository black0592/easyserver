#ifndef __BASE_DEFINE_H__
#define __BASE_DEFINE_H__

// ֻ���һЩ�����Ķ��� 
#define HEADER_SIZE	2

// ��Ϣ��ʶ�Ĵ�С
#define CMD_FLAG_SIZE	1

// �û������������������ֽ���(HEAD+��Ϣ)
#define MAX_USER_PACK_SIZE	64 * 1024

// �û������������Ϣ������ֽ���(��Ϣ)
#define MAX_USER_CMD_SIZE	MAX_USER_PACK_SIZE - HEADER_SIZE

// ���ջ������ݴ�С
#define MAX_USER_SEND_SIZE	1 * 1024
#define MAX_USER_RECV_SIZE	4 * 1024

#ifndef MAX_PATH
#define MAX_PATH	260
#endif

#endif

