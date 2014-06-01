#pragma once
#include "EntityManager.h"

class DropItemManager;

// 掉落物品信息
struct stDropItem
{
	int itemId;		// 物品ID
	int prob;		// 单个物品的掉落概率

	stDropItem()
	{
		itemId = 0;
		prob = 0;
	}
};

// 掉落物品表
class DropItemList : public Entity
{
	friend class DropItemManager;

public:
	DropItemList()
	{
		//prob = 0;
		max = 0;
	}

	virtual ~DropItemList(){}

	void getItems(vector<stDropItem>& vecItem);

public:
	//int prob;	// 掉落概率
	int max;	// 最大数量
	vector<stDropItem> m_vecDropInfo;
};


// 掉落物品配置
class DropItemManager : public EntityManager<EntityManagerID>, public Vek::Singleton<DropItemManager>
{
public:
	DropItemManager(void);
	virtual ~DropItemManager(void);

	// 读取数据文件
	bool load();

	// 输出到文件
	bool outputToFile();
	
	// 获得指定ID的掉落物品表
	DropItemList* getDropList(int listId);

};



