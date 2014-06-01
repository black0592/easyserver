#pragma once

#include "BaseTypes.h"
#include "BaseMisc.h"

class TraversalNote
{
public:
	TraversalNote(void);
	~TraversalNote(void);
	
	void reset();

	// �Ƿ��ڿ����б�
	bool noteOpen;
	// �Ƿ��ڹر��б�
	bool noteClosed;
	// ��������
	int cost;
	// �ڵ�÷�
	int score;
	// ���ڵ�
	TraversalNote* parent;
	// ����
	POINT point;
};
