#include "BinaryHeap.h"

BinaryHeap::BinaryHeap(void)
{
	clear();
	sortMetord = BinaryHeap::defaultSortMetord;
}

BinaryHeap::~BinaryHeap(void)
{
	data.clear();
}

void BinaryHeap::push(void* o)
{
	data.push_back(o);
	int s = (int)data.size() - 1;	// s 当前节点
	modifyIndex(s);
}

int BinaryHeap::indexOf(void* o)
{
	for (int i=0; i<(int)data.size(); ++i) {
		if (data[i] == o) return i;
	}

	return -1;
}

void BinaryHeap::modify(void* o)
{
	int index = indexOf(o);
	if (index > 0)
		modifyIndex(index);
}

void BinaryHeap::modifyIndex(int index)
{
	int s = index;	// s 当前节点
	while (s > 1) {
		int p = int(s / 2);
		if ( sortMetord(data[s], data[p]) ) {
			void* t = data[s];
			data[s] = data[p];
			data[p] = t;
		} else {
			break;
		}

		s = p;
	}
}

void* BinaryHeap::shift()
{
	void* value = 0;
	if (data.size() == 1)	// 数组为空
		return 0;

	if (data.size() == 2) {	// 数组只有一个节点
		value = data[data.size()-1];
		data.pop_back();
		return value;
	}
	
	void* v = data[1];
	int s = 1;	//s 当前节点

	//将末节点移动到队首
	data[1] = data[data.size()-1];
	data.pop_back();

	while (true) {
		int os = s;	// os s的旧值
		int p = s * 2; // p 子节点 
		if (p < (int)data.size()) {
			if (sortMetord(data[p],data[s]))
				s = p;

			//如果另一个子节点更小
			if ( p + 1 < (int)data.size() && sortMetord(data[p + 1],data[s]) )
				s = p + 1;
		}

		if (s != os) {
			void* t = data[s];
			data[s] = data[os];
			data[os] = t;
		} else {
			break;
		}
	}

	return v;
}

void BinaryHeap::clear()
{
	data.clear();
	data.push_back(0);
}

bool BinaryHeap::defaultSortMetord(void* obj1, void* obj2)
{
	return obj1 < obj2;
}