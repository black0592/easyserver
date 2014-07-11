#ifndef __BASE_CMD_H__
#define __BASE_CMD_H__

// byte data[0]; ������������������
#pragma warning(disable:4200)

#pragma pack(1)


#define MAKE_CMDID(cmd, para)	( ((ushort)cmd<<8) | (ushort)para )
#define MSG(cmd, para)	( ((ushort)cmd<<8) | (ushort)para )
#define GET_CMD(cmdId)	 ( (cmdId & 0xFF00) >> 8 )
#define GET_PARA(cmdId)  ( (cmdId & 0x00FF) )


//////////////////////////////////////////////////////////////////////////
// ���������Ϣָ��Ľṹ
typedef byte cmd_type;
typedef byte para_type;

#define FLAG_NONE		0	// ��
#define FLAG_ZIP		1	// ѹ��
#define FLAG_ENCRYPT	2	// ����

#define TYPE_NAME_SIZE	128	// ��Ϣ���Ƴ���

const cmd_type		CMD_NULL = 0;	//��ָ����
const para_type		PARA_NULL = 0;	//�ղ���ָ����

const cmd_type		CMD_PING = -1;	//pingָ����
const para_type		PARA_PING = -1;	//ping����ָ����

struct stBaseCmd 
{
	byte		flag;	// ѹ���ͼ��ܱ��
	cmd_type	cmd;	// ����Ϣ��
	para_type	para;	// ����Ϣ��

#if ENABLE_PROTO_REFLECT
	char		name[TYPE_NAME_SIZE];	// ��Ϣ����
#endif

	stBaseCmd(cmd_type _cmd, para_type _para)
		:cmd(_cmd), para(_para), flag(FLAG_NONE)
	{
#if ENABLE_PROTO_REFLECT
		memset(name, 0, sizeof(name));
#endif
	}

	stBaseCmd(uint _cmdID)
	{
		cmd = GET_CMD(_cmdID);
		para = GET_PARA(_cmdID);
		flag = FLAG_NONE;
#if ENABLE_PROTO_REFLECT
		memset(name, 0, sizeof(name));
#endif
	}

#if ENABLE_PROTO_REFLECT
	void setTypeName(const char* typeName)
	{
#ifdef _DEBUG
		size_t len = strlen(typeName);
		assert(len < TYPE_NAME_SIZE);
#endif
		strncpy(name, typeName, TYPE_NAME_SIZE-1);
	}

	const char* getTypeName()
	{
		return name;
	}

#endif

	const uint getCmdID() const
	{
		ushort cmdID = MAKE_CMDID(cmd, para);
		return (uint)cmdID;
	}
};


// �յ�pingָ�������ͨ
struct stPing : public stBaseCmd
{
	stPing() : stBaseCmd(CMD_PING, PARA_PING)
	{
	}
};


#pragma pack()


#endif