#pragma once
#include "EntityManager.h"

class DropItemManager;

// ������Ʒ��Ϣ
struct stDropItem
{
	int itemId;		// ��ƷID
	int prob;		// ������Ʒ�ĵ������

	stDropItem()
	{
		itemId = 0;
		prob = 0;
	}
};

// ������Ʒ��
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
	//int prob;	// �������
	int max;	// �������
	vector<stDropItem> m_vecDropInfo;
};


// ������Ʒ����
class DropItemManager : public EntityManager<EntityManagerID>, public Vek::Singleton<DropItemManager>
{
public:
	DropItemManager(void);
	virtual ~DropItemManager(void);

	// ��ȡ�����ļ�
	bool load();

	// ������ļ�
	bool outputToFile();
	
	// ���ָ��ID�ĵ�����Ʒ��
	DropItemList* getDropList(int listId);

};



