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

	// 添加节点
	void push(void* o);

	// 搜索对象的下标
	int indexOf(void* o);

	// 修改节点后重排顺序
	void modify(void* o);
	void modifyIndex(int index);

	// 从数组中取出首节点（最小值）
	void* shift();

	// 清空
	void clear();

	static bool defaultSortMetord(void* obj1, void* obj2);

public:
	SORTFUNC sortMetord;

private:
	vector<void*> data;
};
