#include "BaseLib.h"
#include "Rand.h"

namespace easygame {

	// 用时间来初始化随机数种子
	void Rand::srand()
	{
		::srand((uint)time(NULL));
	}

	// 取一个 0 - max(不包含max) 之间的随机数
	int Rand::rand(int max)
	{
		if (max == 0) return 0;
		return ::rand() % max;
	}

	// 取得一个指定数之间的随机数(包含min和max)
	int Rand::randRange(int min, int max)
	{
		int temp = max - min + 1;
		if (temp == 0) return min;
		return ( min + (::rand() % temp) );
	}

	// 判断百分比概率(num/100)
	bool Rand::randByPercent(int num)
	{
		int tmp = randRange(1,100);
		if ( tmp <= num )
			return true;
		return false;
	}

	// 判断千分比概率(num/1000)
	bool Rand::randByMilli(int num)
	{
		int tmp = randRange(1,1000);
		if ( tmp <= num )
			return true;
		return false;
	}

	// 判断万分比概率(num/10000)
	bool Rand::randByMyria(int num)
	{
		int tmp = randRange(1,10000);
		if ( tmp <= num )
			return true;
		return false;
	}


}	// namespace


