#ifndef __CHARDATA_H__
#define __CHARDATA_H__

#include "ProtoGameData.pb.h"


#include "ItemData.h"
#include "Define.h"


// 对象战斗属性
enum EObjectAttrib
{
	E_MaxHP,		// 最大生命值
	E_MaxMP,		// 最大魔法值
	E_PhyAttack,	// 物理攻击
	E_MagAttack,	// 魔法攻击
	E_PhyDefence,	// 物理防御
	E_MagDefence,	// 魔法防御
	E_HitChance,	// 命中率
	E_DodgeChance,	// 闪避率
	E_CriticalHitChance,	// 暴击概率
	E_CriticalDodgeChance,	// 暴击躲避概率
	E_MoveSpeed,	// 移动速度
	E_AttackSpeed,	// 攻击速度
	E_STR,			// 力量
	E_DEX,			// 敏捷
	E_INT,			// 智力

	E_ObjAttribMax,	// 数值最大数量
};

// 运行时计算的属性值
struct stRuntimeAttrib
{
	// 常用数值
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
		stRuntimeAttrib tmp(*this);      // 调用拷贝构造函数，通过复制产生一个新的object
		tmp *= rhs;             // 利用operator += 实现operator +
		return tmp;             // 返回计算结果
	};

	stRuntimeAttrib& operator+=(const stRuntimeAttrib& rhs) {
		for (size_t i=0; i<prop.size(); i++) {
			prop[i] += rhs[(EObjectAttrib)i];
		}
		return *this;
	}

	const stRuntimeAttrib operator+ (const stRuntimeAttrib& rhs) const {
		stRuntimeAttrib tmp(*this);      // 调用拷贝构造函数，通过复制产生一个新的object
		tmp += rhs;             // 利用operator += 实现operator +
		return tmp;             // 返回计算结果
	};

	float& operator[](EObjectAttrib type)
	{
		return prop[type];
	}

	const float& operator[](EObjectAttrib type) const
	{
		return prop[type];
	}

	// 向下取整
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
// 旧的，以后不用了


// 包裹坐标
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






