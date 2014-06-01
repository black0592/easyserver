#pragma once

#define MINIMAPNUM		150
#define MINIMAPSIZEX	200
#define MINIMAPSIZEY	150
#define MINIMAPINFOSIZE	32

#define MINIMAP_FILENAME		"rse\\minimaps.pdic"
#define MINIMAP_INFOFILENAME	"rse\\minimap.info"

#define MINIMAP_CTL_BTNID		138

typedef struct _MiniScale
{
	float xs;
	float ys;
	void SetScale(float x,float y)
	{
		xs=x;
		ys=y;
	};
}  MiniScale;

typedef struct _MiniMapInfo
{
	RECT rt_rgn;
	char str_rgn[64];

	_MiniMapInfo()
	{
		SetRect(&rt_rgn,0,0,0,0);
		strncpy(str_rgn,"", 63);
	}

} MiniMapInfo;

typedef struct _SMiniMap
{
	BYTE		MapNum;
	WORD		src_x;
	WORD		src_y;
	MiniScale	scale;

	UINT			info_num;
	MiniMapInfo		info[MINIMAPINFOSIZE];

	_SMiniMap()
	{
		MapNum=0;
		src_x=0;
		src_y=0;
		info_num=0;
	}

} SMiniMap;