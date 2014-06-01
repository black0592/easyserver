#ifndef __BASE_STRUCT_H__
#define __BASE_STRUCT_H__
#include "Types.h"
#include "Define.h"

// ��Щ��Ϣ�ṹ��Ҫ�ڷ�����֮�����ת���ģ�����д�����������ת����

#pragma pack(1)


// ��ͼ����(������ͼ��ţ�X��Y����)
struct stMapPos 
{
	uint mapId;	// ��ͼ���
	uint dupId;	// �������
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