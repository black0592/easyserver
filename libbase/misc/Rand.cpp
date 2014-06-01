#include "BaseLib.h"
#include "Rand.h"

namespace easygame {

	// ��ʱ������ʼ�����������
	void Rand::srand()
	{
		::srand((uint)time(NULL));
	}

	// ȡһ�� 0 - max(������max) ֮��������
	int Rand::rand(int max)
	{
		if (max == 0) return 0;
		return ::rand() % max;
	}

	// ȡ��һ��ָ����֮��������(����min��max)
	int Rand::randRange(int min, int max)
	{
		int temp = max - min + 1;
		if (temp == 0) return min;
		return ( min + (::rand() % temp) );
	}

	// �жϰٷֱȸ���(num/100)
	bool Rand::randByPercent(int num)
	{
		int tmp = randRange(1,100);
		if ( tmp <= num )
			return true;
		return false;
	}

	// �ж�ǧ�ֱȸ���(num/1000)
	bool Rand::randByMilli(int num)
	{
		int tmp = randRange(1,1000);
		if ( tmp <= num )
			return true;
		return false;
	}

	// �ж���ֱȸ���(num/10000)
	bool Rand::randByMyria(int num)
	{
		int tmp = randRange(1,10000);
		if ( tmp <= num )
			return true;
		return false;
	}


}	// namespace


