#include "stdafx.h"
#include "BaseDataManager.h"
#include "DropItemManager.h"

void DropItemList::getItems(vector<stDropItem>& vecItem)
{
	for (size_t i=0; i<m_vecDropInfo.size(); ++i) {
		stDropItem& item = m_vecDropInfo[i];
		vecItem.push_back(item);
	}
}




DropItemManager::DropItemManager(void)
{
}

DropItemManager::~DropItemManager(void)
{
}

// 读取配置文件
bool DropItemManager::load()
{
	EntityManager::release();

	vector<DropItem*> dropList;
	GameTable::getAllDropItem(dropList);

	DropItemList* pDropItemList = NULL;
	for (size_t i=0; i<dropList.size(); i++) { 

		const DropItem* dropInfo = dropList[i];

		pDropItemList = new DropItemList;
		//pDropItemList->prob = dropNode.attribute("prob").as_int();
		//pDropItemList->max = dropNode.attribute("max").as_int();
		pDropItemList->setID(dropInfo->keyId.id);
		pDropItemList->setName("(null)");

		for (size_t i=0; i<dropInfo->items.size(); i++) {
			const DropItem::Drops& drops = dropInfo->items[i];
			stDropItem dropItem;
			dropItem.itemId = drops.iItemId;
			dropItem.prob = drops.iProbability;
			pDropItemList->m_vecDropInfo.push_back(dropItem);
		}

		if (!this->addEntity(pDropItemList))
			SAFE_DELETE(pDropItemList);
	}


	//char szFile[260] = {0};
	//sprintf(szFile, "%s/dropitem.xml", DROPITEM_PATH);
	//xml_document doc;  
	//File file;
	//if (!file.open(szFile))
	//	return false;

	//doc.load_buffer(file.getBufferPtr(), file.getLength());

	//DropItemList* pDropItemList = NULL;
	//for (pugi::xml_node dropNode=doc.child("dropitem").child("drop"); dropNode; dropNode=dropNode.next_sibling()) { 
	//	pDropItemList = new DropItemList;
	//	int id = dropNode.attribute("id").as_int();
	//	const char* name = dropNode.attribute("name").as_string();
	//	//pDropItemList->prob = dropNode.attribute("prob").as_int();
	//	pDropItemList->max = dropNode.attribute("max").as_int();
	//	pDropItemList->setID(id);
	//	pDropItemList->setName(name);

	//	for (pugi::xml_node itemNode=dropNode.child("item"); itemNode; itemNode=itemNode.next_sibling()) { 
	//		stDropItem dropItem;
	//		dropItem.itemId = itemNode.attribute("id").as_int();
	//		dropItem.prob = itemNode.attribute("prob").as_int();
	//		pDropItemList->m_vecDropInfo.push_back(dropItem);
	//	}

	//	if (!this->addEntity(pDropItemList))
	//		SAFE_DELETE(pDropItemList);
	//}

	return true;
}

bool DropItemManager::outputToFile()
{
	FILE* file = fopen("./dropitemlist.csv", "w+b");
	if (file == NULL) {
		printf("打开文件dropitemlist.csv失败\n");
		return false;
	}

	struct stAllList : public stCallBack<DropItemList> 
	{
		FILE* pFile;

		virtual bool exec(DropItemList* obj)
		{
			for (size_t i=0; i<obj->m_vecDropInfo.size(); ++i) {
				stDropItem& item = obj->m_vecDropInfo[i];
				char szBuff[512];
				const stItemBase* base = GameTable::getItemBase(item.itemId);
				if (base == NULL) {
					printf("不存在的物品: itemid=%u\n", item.itemId);
					return true;
				}
				sprintf(szBuff,"%u,%s\r\n", item.itemId, base->name);
				fwrite(szBuff, strlen(szBuff), 1, pFile);
			}

			return true;
		}
	};

	stAllList cb;
	cb.pFile = file;
	this->exec(cb);

	fclose(file);

	return true;
}

// 获得指定ID的掉落物品表
DropItemList* DropItemManager::getDropList(int listId)
{
	return (DropItemList*) this->getEntityByID(listId);
}




