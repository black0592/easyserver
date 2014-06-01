#include "StdAfx.h"
#include "NpcManager.h"

#define NPCINFO_PATH	"npcinfo"

Npc::Npc()
{
	mFunc = 0;
	m_isMouseMove = false;
	m_isSelected = false;
	m_nameColor = YELLOW;
	m_Effect = EF_NORMAL;
	m_id = 0;
	m_baseId = 0;
	m_pt.x = m_pt.y = 500;
	m_curFrm = 8;
}


Npc::~Npc()
{

}

void Npc::SetID(UINT id)
{
	m_id = id;
}

UINT Npc::GetID()
{
	return m_id;
}

void Npc::SetBaseID(UINT id)
{
	m_baseId = id;
}

UINT Npc::GetBaseID()
{
	return m_baseId;
}

POINT Npc::GetPoint()
{
	return m_pt;
}


void Npc::SetName(const char* name)
{
	m_name = Misc::utf8ToGbk(name);
}

const char* Npc::GetName()
{
	return m_name.c_str();
}


void Npc::FillNpcInfo(NPCINFO& npcInfo)
{
	m_info = npcInfo;
	POINT pt = {npcInfo.mapX*CXTILE, npcInfo.mapY*CYTILE};
	this->SetPoint(pt);
}

NPCINFO& Npc::GetNpcInfo()
{
	return m_info;
}

void Npc::SetGridPos(int x, int y)
{
	POINT pt;
	pt.x = x * CXTILE;
	pt.y = y * CYTILE;
	SetPoint(pt);
}

bool Npc::Load(UINT baseId)
{
	const stNpcBase* base = GameTable::getNpcBase(baseId);
	if (base == NULL) {
		TRACE("无效的NPC编号");
		return false;
	}

	m_baseId = baseId;
	m_name = Misc::utf8ToGbk(base->name);

	CString strName;
	strName.Format("%s/npc/02%03d002.plist", EditorConfig::Instance()->getClientPath(), base->imgId);
	return mSpriteSheet.load(strName);
}

void Npc::Draw(POINT ptMap)
{
	IEngine* pEngine = GetEngine();
	IVideo* pVideo = pEngine->getVideo();
	easy2d::IFont* pFont = pEngine->getFont();


	// 画npc身体
	int x = m_pt.x - ptMap.x;
	int y = m_pt.y - ptMap.y;
	mSpriteSheet.render(x+CXTILE/2, y+CYTILE/2);
	if (CommData::isShowMiniMap) {
		pVideo->drawSolidRect(Pointf(x, y), Pointf(x+CXTILE, y+CYTILE), 0x5000FF00);
		pVideo->drawRect(Pointf(x, y), Pointf(x+CXTILE, y+CYTILE), RED, 2.0f);
	}

	if (m_isMouseMove || m_isSelected) {
		// 选中状态
		m_nameColor = RED;
		m_Effect = EF_RED;
	} else {
		m_nameColor = YELLOW;
		m_Effect = EF_NORMAL;
	}

	// 画npc名字
	int frameHeight = 0;
	int offsetX = (int)m_name.length() * 4 - CXTILE/2;
	FRAME_INFO* frm = mSpriteSheet.getFrame(0);
	if (frm != NULL) {
		frameHeight = mSpriteSheet.getFrame(0)->height;
	} else {
		frameHeight = 86;
	}

	stFontEffect fontEff;
	pFont->drawString(x-offsetX, y-frameHeight, m_name.c_str(), m_nameColor, fontEff);


	/*if (m_info.npcFunc != 255) {
	stFontEffect fontEff;
	pFont->drawString(x-32, y-frameHeight+30, m_name, m_nameColor, fontEff);
	} else {
	stFontEffect fontEff;
	pFont->drawString(x-32, y-frameHeight+30, "动物", m_nameColor, fontEff);
	}*/

}

void Npc::DrawPositionToMiniMap(FLOAT scalX, FLOAT scalY)
{
	IEngine* pEngine = GetEngine();
	IVideo* pVideo = pEngine->getVideo();

	int x1 = m_pt.x / CXTILE;
	int x2 = x1 + 1;
	int y1 = m_pt.y / CYTILE;
	int y2 = y1 + 1;
	x1 = x1 * CXTILE / scalX;
	y1 = y1 * CYTILE / scalY;
	x2 = x2 * CXTILE / scalX;
	y2 = y2 * CYTILE / scalY;

	if (this->m_info.npcFunc == 255) {
		// 装饰性NPC
		pVideo->drawSolidRect(Pointf(x1, y1), Pointf(x2, y2), WHITE);
	} else {
		// 功能NPC
		pVideo->drawSolidRect(Pointf(x1, y1), Pointf(x2, y2), GREEN);
	}
	pVideo->drawRect(Pointf(x1, y1), Pointf(x2, y2), RED);
}

void Npc::Update()
{
	m_curFrm++;
	if (m_curFrm > 11)
		m_curFrm = 8;
}

void Npc::SetPoint(POINT pt)
{
	m_pt = pt;
	m_info.mapX = pt.x / CXTILE;
	m_info.mapY = pt.y / CYTILE;
}

bool Npc::OnMouseMove(POINT ptMap, POINT mousePt)
{
	int drawX = m_pt.x - ptMap.x;
	int drawY = m_pt.y - ptMap.y;

	int dx = (mousePt.x - drawX);
	int dy = (mousePt.y - drawY);

	// 	CString strInfo;
	// 	strInfo.Format("dx=%d, dy=%d\n", dx, dy);
	// 	OutputDebugString(strInfo);

	if ( (dx >= 0 && dx <= 55) && (dy >= -40 && dy<= 60) ) {
		m_isMouseMove = TRUE;
		return true;
	}

	m_isMouseMove = FALSE;
	return false;
}

bool Npc::IsSelected()
{
	if (m_isMouseMove || m_isSelected) {
		return true;
	}

	return false;
}

void Npc::SetSelected(BOOL flag)
{
	m_isSelected = flag;
}
//
//bool Npc::IsSelected(POINT ptMap, POINT mousePt)
//{
//	int drawX = m_pt.x - ptMap.x;
//	int drawY = m_pt.y - ptMap.y;
//
//	int dx = (mousePt.x - drawX);
//	int dy = (mousePt.y - drawY);
//
//	if ( (dx >= 10 && dx <= 40) && (dy >= 0 && dy<= 55) ) {
//		m_nameColor = RED;
//		m_Effect = EF_RED;
//		return true;
//	} 
//
//	//CString strInfo;
//	//strInfo.Format("dx=%d, dy=%d\n", dx, dy);
//	//OutputDebugString(strInfo);
//
//	m_nameColor = YELLOW;
//	m_Effect = EF_NORMAL;
//	return false;
//}


//////////////////////////////////////////////////////////////////////////


NpcManager::NpcManager(void)
{
	m_uinID = 1;
	m_pListCtrl = NULL;
	m_mapId = 0;
	m_lastTick = 0;
	m_isDrawNpcPostion = FALSE;
}

NpcManager::~NpcManager(void)
{
	Release();
}

void NpcManager::InitListCtrl(CListCtrl* listCtrl)
{
	m_pListCtrl = listCtrl;
}

void NpcManager::Release()
{
	m_uinID = 1;

	while (!m_objMap.empty()) {
		Npc* pNpc = m_objMap.begin()->second;
		delete pNpc;
		m_objMap.erase(m_objMap.begin());
	}
}

bool NpcManager::AddNpc(Npc* npc)
{
	const stNpcBase* base = GameTable::getNpcBase(npc->GetBaseID());
	if (base == NULL) {
		return false;
	}

	NPCINFO& info = npc->GetNpcInfo();

	int index = m_pListCtrl->GetItemCount();
	CString strTmp;
	strTmp.Format("%d", m_uinID);
	m_pListCtrl->InsertItem(index, strTmp);			// 序号

	strTmp.Format("[%d]%s", base->id, npc->GetName());
	m_pListCtrl->SetItemText(index, 1, strTmp);		// 名称
	
	strTmp.Format("%s", Misc::getNpcFuncStr(info.npcFunc));
	m_pListCtrl->SetItemText(index, 2, strTmp);		// 功能

	CString strStoreName = "(null)";
	const ShopInfo* pShopInfo = GameTable::getShopInfo( info.storeId );
	if (pShopInfo) {
		strStoreName = Misc::utf8ToGbk(pShopInfo->pDesc).c_str();
	}
	strTmp.Format("[%d]%s", info.storeId, strStoreName);
	m_pListCtrl->SetItemText(index, 3, strTmp);		// 商店编号

	strTmp.Format("%s", info.scriptFile.c_str());
	m_pListCtrl->SetItemText(index, 4, strTmp);		// 脚本文件名

	npc->SetID(m_uinID);
	m_objMap[m_uinID] = npc;
	m_uinID++;
	return true;
}

void NpcManager::EditNpc(Npc* npc)
{
	for (int i=0; i<m_pListCtrl->GetItemCount(); ++i) {
		CString strID = m_pListCtrl->GetItemText(i, 0);
		UINT uinId = atoi(strID);
		Npc* pNpc = this->FindNpc(uinId);
		if (pNpc == NULL) continue;
		if (pNpc->GetID() != npc->GetID()) continue;

		const stNpcBase* base = GameTable::getNpcBase(npc->GetBaseID());
		if (base == NULL) {
			continue;
		}

		NPCINFO& info = pNpc->GetNpcInfo();

		int index = i;

		CString strTmp;

		strTmp.Format("[%d]%s", base->id, npc->GetName());
		m_pListCtrl->SetItemText(index, 1, strTmp);		// 名称

		strTmp.Format("%s", Misc::getNpcFuncStr(info.npcFunc));
		m_pListCtrl->SetItemText(index, 2, strTmp);		// 功能

		CString strStoreName = "(null)";
		const ShopInfo* pShopInfo = GameTable::getShopInfo( info.storeId );
		if (pShopInfo) {
			strStoreName = Misc::utf8ToGbk(pShopInfo->pDesc).c_str();
		}
		strTmp.Format("[%d]%s", info.storeId, strStoreName);
		m_pListCtrl->SetItemText(index, 3, strTmp);		// 商店编号

		strTmp.Format("%s", info.scriptFile.c_str());
		m_pListCtrl->SetItemText(index, 4, strTmp);		// 脚本文件名
	}
}

void NpcManager::DeleteNpc(UINT uinID)
{
	map<UINT,Npc*>::iterator itFind = m_objMap.find(uinID);
	if (itFind == m_objMap.end())
		return;

	Npc* pNpc = itFind->second;
	m_objMap.erase(itFind);
	delete pNpc;

	int count = m_pListCtrl->GetItemCount();
	for (int i=0; i<count; ++i) {
		CString strID = m_pListCtrl->GetItemText(i, 0);
		UINT id = atoi(strID);
		if (id == uinID) {
			m_pListCtrl->DeleteItem(i);
			break;
		}
	}
}

bool NpcManager::LoadNpc(UINT mapId)
{
	char xmlfile[260] = {0};
	sprintf(xmlfile, "npcinfo%u.xml", mapId);

	m_mapId = mapId;

	m_pListCtrl->DeleteAllItems();
	Release();

	char szFile[260] = {0};
	sprintf(szFile, "%s/%s", EditorConfig::Instance()->makeServerResPath(NPCINFO_PATH), xmlfile);

	xml_document doc;  
	File file;
	if (!file.open(szFile))
		return false;

	doc.load_buffer(file.getBufferPtr(), file.getLength());

	ushort count = 0;
	ushort index = 0;
	for (pugi::xml_node node1=doc.child("npcinfo").child("npc"); node1; node1=node1.next_sibling()) {
		NPCINFO info;
		int baseId = node1.attribute("BaseId").as_int();
		index++;

		info.baseId = baseId;

		info.mapX = node1.attribute("BirthX").as_int();
		info.mapY = node1.attribute("BirthY").as_int();
		info.npcFunc = node1.attribute("NpcFunc").as_int();
		info.storeId = node1.attribute("StoreId").as_int();
		info.scriptFile = Misc::utf8ToGbk( node1.attribute("ScriptFile").as_string() );
	
		const stNpcBase* base = GameTable::getNpcBase(baseId);
		if (!base) {
			LOGE("读取NPC分布出错，不存在的NPC类型, file=%s, race=%u", szFile, baseId);
			continue;
		}

		Npc* pNpc = new Npc;
		pNpc->SetName(base->name);
		pNpc->FillNpcInfo(info);
		pNpc->Load(base->id);
		pNpc->SetGridPos(info.mapX, info.mapY);

		if (!this->AddNpc(pNpc)) {
			SAFE_DELETE(pNpc);
			TRACE("插入NPC失败, file=%s, baseId=%u", szFile, info.baseId);
			continue;
		}
	}


	return true;
}

void NpcManager::Draw(POINT ptMap)
{
	map<UINT,Npc*>::iterator itBegin = m_objMap.begin();
	map<UINT,Npc*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Npc* pNpc = itBegin->second;
		pNpc->Draw(ptMap);
		++itBegin;
	}
}

void NpcManager::DrawNpcPosition(FLOAT scalX, FLOAT scalY)
{
	DWORD curTick = GetTickCount();
	if (curTick - m_lastTick > 200) {
		m_lastTick = curTick;
		m_isDrawNpcPostion = !m_isDrawNpcPostion;
	}

	if (!m_isDrawNpcPostion) return;

	map<UINT,Npc*>::iterator itBegin = m_objMap.begin();
	map<UINT,Npc*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Npc* pNpc = itBegin->second;
		pNpc->DrawPositionToMiniMap(scalX, scalY);
		++itBegin;
	}
}

void NpcManager::Update()
{
	map<UINT,Npc*>::iterator itBegin = m_objMap.begin();
	map<UINT,Npc*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Npc* pNpc = itBegin->second;
		pNpc->Update();
		++itBegin;
	}
}

void NpcManager::UnSelectAll()
{
	map<UINT,Npc*>::iterator itBegin = m_objMap.begin();
	map<UINT,Npc*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Npc* pNpc = itBegin->second;
		pNpc->SetSelected(FALSE);
		++itBegin;
	}
}

Npc* NpcManager::FindSelectNpc(POINT ptMap, POINT mousePt)
{
	map<UINT,Npc*>::iterator itBegin = m_objMap.begin();
	map<UINT,Npc*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Npc* pNpc = itBegin->second;
		bool success = pNpc->OnMouseMove(ptMap, mousePt);
		if (success) return pNpc;
		++itBegin;
	}

	return NULL;
}

void NpcManager::OnMouseMove(POINT ptMap, POINT mousePt)
{
	map<UINT,Npc*>::iterator itBegin = m_objMap.begin();
	map<UINT,Npc*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Npc* pNpc = itBegin->second;
		pNpc->OnMouseMove(ptMap, mousePt);
		++itBegin;
	}
}


Npc* NpcManager::FindNpc(UINT id)
{
	Npc* pNpc = NULL;
	map<UINT,Npc*>::iterator itFind = m_objMap.find(id);
	if (itFind == m_objMap.end()) {
		return NULL;
	}

	return itFind->second;
}


Npc* NpcManager::GetSelectedNpc()
{
	map<UINT,Npc*>::iterator itBegin = m_objMap.begin();
	map<UINT,Npc*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Npc* pNpc = itBegin->second;
		if (pNpc->IsSelected())
			return pNpc;
		++itBegin;
	}

	return NULL;
}

UINT NpcManager::GetCount()
{
	return (UINT)m_objMap.size();
}

bool NpcManager::SaveToFile()
{
	if (this->GetCount() == 0)
		return false;

	char szFile[260] = {0};
	sprintf(szFile, "%s/npcinfo%u.xml", EditorConfig::Instance()->makeServerResPath(NPCINFO_PATH), m_mapId);
	ofstream ofOut(szFile);

	char szLine[10 * 1024];
	ZeroMemory(szLine, sizeof(szLine));

	sprintf(szLine, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n\r\n");
	Misc::writeUTF8Str(ofOut, szLine);

	sprintf(szLine, "<npcinfo>\r\n\r\n");
	Misc::writeUTF8Str(ofOut, szLine);


	map<UINT,Npc*>::iterator itBegin = m_objMap.begin();
	map<UINT,Npc*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Npc* pNpc = itBegin->second;
		const stNpcBase* base = GameTable::getNpcBase(pNpc->GetBaseID());
		if (base == NULL) {
			++itBegin;
			continue;
		}

		NPCINFO& info = pNpc->GetNpcInfo();

		sprintf(szLine, "\t<npc BaseId=\"%d\" BirthX=\"%d\" BirthY=\"%d\" NpcFunc=\"%d\" StoreId=\"%d\" ScriptFile=\"%s\" />\r\n",
			base->id, info.mapX, info.mapY, info.npcFunc, info.storeId, info.scriptFile.c_str());
		Misc::writeUTF8Str(ofOut, szLine);

		++itBegin;
	}

	sprintf(szLine, "\r\n</npcinfo>\r\n\r\n");
	Misc::writeUTF8Str(ofOut, szLine);

	return true;
}

