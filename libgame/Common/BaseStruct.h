#ifndef __BASE_STRUCT_H__
#define __BASE_STRUCT_H__
#include "Types.h"
#include "Define.h"

// 有些消息结构需要在服务器之间进行转发的，可以写在这里，方便中转操作

#pragma pack(1)


// 地图坐标(包含地图编号，X和Y坐标)
struct stMapPos 
{
	uint mapId;	// 地图编号
	uint dupId;	// 副本编号
	uint mapX;
	uint mapY;

	stMapPos(uint _mapId, uint _dupId, uint _mapX, uint _mapY)
	{
		mapId = _mapId;
		dupId = _dupId;
		mapX = _mapX;
		mapY = _mapY;
	}

	stMapPos()
	{
		bzero(this, sizeof(*this));
	}
};


#pragma pack()


#endif