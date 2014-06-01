#ifndef __GUILDMARK_CMD_H__
#define __GUILDMARK_CMD_H__
#include "BaseCmd.h"
#include "BaseStruct.h"
#include "Define.h"

namespace Protocol
{

#pragma pack(1)


	// 此功能块的结构基类
	struct stGuildMarkBaseCmd : public stBaseCmd
	{
		stGuildMarkBaseCmd(para_type para) : stBaseCmd(USERCMD_GUILDMARK, para)
		{
		}
	};


	//////////////////////////////////////////////////////////////////////////


	
	// 验证版本号
	const para_type GMARK_CMD_CHECK_VERSION = 1;		
	struct stGMarkCheckVersion : stGuildMarkBaseCmd
	{
		uint	version;
		byte	world;
		char	msgTicker[8][64];
		byte	result;

		stGMarkCheckVersion() : stGuildMarkBaseCmd(GMARK_CMD_CHECK_VERSION)
		{

		}
	};


	// 打包文件
	const para_type GMARK_CMD_PACK_FILE = 2;		
	struct stGMarkPackFile : stGuildMarkBaseCmd
	{
		byte			world;
		byte			FileCnt;
		uint			FileSize;
		byte			result;
		byte			pFile[0];	

		stGMarkPackFile() : stGuildMarkBaseCmd(GMARK_CMD_PACK_FILE)
		{

		}

		uint getSize() { return sizeof(*this) + FileSize; }
	};

	// 开始图片上传
	const para_type GMARK_CMD_IMG_UPLOAD = 3;		
	struct stGMarkImgUpload : stGuildMarkBaseCmd
	{
		byte	world;
		ushort	GuildId;

		uint	width;
		uint	height;
		int		size;
		ushort	ImgBuf[256];		//image size (12*12)'s max sprite buffer

		stGMarkImgUpload() : stGuildMarkBaseCmd(GMARK_CMD_IMG_UPLOAD)
		{

		}
	};

	// 结束图片上传
	const para_type GMARK_CMD_IMG_UPLOAD_END = 4;		
	struct stGMarkImgUploadEnd : stGuildMarkBaseCmd
	{
		byte	world;
		ushort	GuildId;

		stGMarkImgUploadEnd() : stGuildMarkBaseCmd(GMARK_CMD_IMG_UPLOAD_END)
		{

		}
	};

	// 删除图片
	const para_type GMARK_CMD_IMG_DELETE = 5;
	struct stGMarkImgDelete : stGuildMarkBaseCmd
	{
		byte	world;
		ushort	GuildId;

		stGMarkImgDelete() : stGuildMarkBaseCmd(GMARK_CMD_IMG_DELETE)
		{

		}
	};




#pragma pack()

};	// namespace


#endif