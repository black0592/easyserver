#ifndef __CHARDATA_H__
#define __CHARDATA_H__

#include "ProtoGameData.pb.h"


#include "ItemData.h"
#include "Define.h"


// ����ս������
enum EObjectAttrib
{
	E_MaxHP,		// �������ֵ
	E_MaxMP,		// ���ħ��ֵ
	E_PhyAttack,	// ������
	E_MagAttack,	// ħ������
	E_PhyDefence,	// �������
	E_MagDefence,	// ħ������
	E_HitChance,	// ������
	E_DodgeChance,	// ������
	E_CriticalHitChance,	// ��������
	E_CriticalDodgeChance,	// ������ܸ���
	E_MoveSpeed,	// �ƶ��ٶ�
	E_AttackSpeed,	// �����ٶ�
	E_STR,			// ����
	E_DEX,			// ����
	E_INT,			// ����

	E_ObjAttribMax,	// ��ֵ�������
};

// ����ʱ���������ֵ
struct stRuntimeAttrib
{
	// ������ֵ
	vector<float> prop;

	stRuntimeAttrib()
	{
		prop.resize(E_ObjAttribMax);
	}

	void clearValue()
	{
		for (size_t i=0; i<prop.size(); i++) {
			prop[i] = 0;
		}
	}

	stRuntimeAttrib& operator*=(const stRuntimeAttrib& rhs) {
		for (size_t i=0; i<prop.size(); i++) {
			prop[i] *= rhs[(EObjectAttrib)i];
		}
		return *this;
	}

	const stRuntimeAttrib operator* (const stRuntimeAttrib& rhs) const {
		stRuntimeAttrib tmp(*this);      // ���ÿ������캯����ͨ�����Ʋ���һ���µ�object
		tmp *= rhs;             // ����operator += ʵ��operator +
		return tmp;             // ���ؼ�����
	};

	stRuntimeAttrib& operator+=(const stRuntimeAttrib& rhs) {
		for (size_t i=0; i<prop.size(); i++) {
			prop[i] += rhs[(EObjectAttrib)i];
		}
		return *this;
	}

	const stRuntimeAttrib operator+ (const stRuntimeAttrib& rhs) const {
		stRuntimeAttrib tmp(*this);      // ���ÿ������캯����ͨ�����Ʋ���һ���µ�object
		tmp += rhs;             // ����operator += ʵ��operator +
		return tmp;             // ���ؼ�����
	};

	float& operator[](EObjectAttrib type)
	{
		return prop[type];
	}

	const float& operator[](EObjectAttrib type) const
	{
		return prop[type];
	}

	// ����ȡ��
	void floor()
	{
		for (size_t i=0; i<prop.size(); i++) {
			if (i == E_HitChance)
				continue;
			if (i == E_DodgeChance)
				continue;
			if (i == E_CriticalHitChance)
				continue;
			if (i == E_CriticalDodgeChance)
				continue;

			prop[i] = (int)prop[i];
		}
	}
};



//////////////////////////////////////////////////////////////////////////
// �ɵģ��Ժ�����


// ��������
struct stPkgPos
{
	ushort pkg;
	ushort slot;

	stPkgPos()
	{
		init();
	}

	void init()
	{
		pkg = (ushort)-1;
		slot = (ushort)-1;
	}
};

struct stItemEx 
{
	stPkgPos pos;
	ItemData itemData;
};



#endif






