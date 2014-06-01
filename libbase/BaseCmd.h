#ifndef __BASE_CMD_H__
#define __BASE_CMD_H__

// byte data[0]; 这种声明，不报警告
#pragma warning(disable:4200)

#pragma pack(1)


#define MAKE_CMDID(cmd, para)	( ((ushort)cmd<<8) | (ushort)para )
#define MSG(cmd, para)	( ((ushort)cmd<<8) | (ushort)para )
#define GET_CMD(cmdId)	 ( (cmdId & 0xFF00) >> 8 )
#define GET_PARA(cmdId)  ( (cmdId & 0x00FF) )


//////////////////////////////////////////////////////////////////////////
// 最基础的消息指令的结构
typedef byte cmd_type;
typedef byte para_type;

#define FLAG_NONE		0	// 无
#define FLAG_ZIP		1	// 压缩
#define FLAG_ENCRYPT	2	// 加密

const cmd_type		CMD_NULL = 0;	//空指令编号
const para_type		PARA_NULL = 0;	//空参数指令编号
struct stBaseCmd 
{
	byte		flag;	// 压缩和加密标记
	cmd_type	cmd;	// 主消息号
	para_type	para;	// 子消息号

	stBaseCmd(cmd_type _cmd, para_type _para)
		:cmd(_cmd), para(_para), flag(FLAG_NONE)
	{

	}

	stBaseCmd(uint _cmdID)
	{
		cmd = GET_CMD(_cmdID);
		para = GET_PARA(_cmdID);
		flag = FLAG_NONE;
	}

	const uint getCmdID() const
	{
		ushort cmdID = MAKE_CMDID(cmd, para);
		return (uint)cmdID;
	}
};


// 空的ping指令，保持联通
struct stPing : public stBaseCmd
{
	stPing() : stBaseCmd(CMD_NULL, PARA_NULL)
	{
	}
};


#pragma pack()


#endif