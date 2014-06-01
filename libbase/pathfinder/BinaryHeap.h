#pragma once
#include "TraversalNote.h"
#include <vector>
using namespace std;

typedef bool (*SORTFUNC)(void*, void*);

class BinaryHeap
{
public:
	BinaryHeap(void);
	virtual ~BinaryHeap(void);

	int length() { return (int)data.size() - 1; }

	// ��ӽڵ�
	void push(void* o);

	// ����������±�
	int indexOf(void* o);

	// �޸Ľڵ������˳��
	void modify(void* o);
	void modifyIndex(int index);

	// ��������ȡ���׽ڵ㣨��Сֵ��
	void* shift();

	// ���
	void clear();

	static bool defaultSortMetord(void* obj1, void* obj2);

public:
	SORTFUNC sortMetord;

private:
	vector<void*> data;
};
