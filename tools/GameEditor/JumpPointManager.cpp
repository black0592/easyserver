#include "StdAfx.h"
#include "JumpPointManager.h"

#define JUMPPOINT_PATH	"jumppoint"

JumpPoint::JumpPoint()
{
	m_isMouseMove = false;
	m_isSelected = false;
	m_nameColor = YELLOW;
	m_Effect = EF_NORMAL;
	m_id = 0;
	m_pt.x = m_pt.y = 500;
	m_curFrm = 8;
}


JumpPoint::~JumpPoint()
{

}

void JumpPoint::SetID(UINT id)
{
	m_id = id;
}

UINT JumpPoint::GetID()
{
	return m_id;
}

POINT JumpPoint::GetPoint()
{
	return m_pt;
}

const char* JumpPoint::GetName()
{
	return "传送点";
}


void JumpPoint::FillInfo(JumpPointInfo& info)
{
	m_info = info;
	POINT pt = {info.mapX*CXTILE, info.mapY*CYTILE};
	this->SetPoint(pt);

	const stMapInfo* pMapInfo = GameTable::getMapInfo(info.targetMapId);
	if (pMapInfo == NULL) {
		return;
	}

	char temp[1024];
	sprintf(temp, "[%d]%s(%d,%d)", info.targetMapId, Misc::utf8ToGbk(pMapInfo->name).c_str(), info.targetMapX, info.targetMapY);
	m_name = temp;
}

JumpPointInfo& JumpPoint::GetInfo()
{
	return m_info;
}

void JumpPoint::SetGridPos(int x, int y)
{
	POINT pt;
	pt.x = x * CXTILE;
	pt.y = y * CYTILE;
	SetPoint(pt);
}

bool JumpPoint::Load(UINT looks)
{
	CString strName;
	strName.Format("%s/effect/other/teleport_1.plist", EditorConfig::Instance()->getClientPath());
	return mSpriteSheet.load(strName);
}

void JumpPoint::Draw(POINT ptMap)
{
	IEngine* pEngine = GetEngine();
	IVideo* pVideo = pEngine->getVideo();
	easy2d::IFont* pFont = pEngine->getFont();


	// 画跳转点
	int x = m_pt.x - ptMap.x;
	int y = m_pt.y - ptMap.y;
	mSpriteSheet.render(x+CXTILE/2, y+CYTILE/2 - CYTILE * 2);
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

void JumpPoint::DrawPositionToMiniMap(FLOAT scalX, FLOAT scalY)
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

	// 装饰性NPC
	pVideo->drawSolidRect(Pointf(x1, y1), Pointf(x2, y2), WHITE);
	pVideo->drawRect(Pointf(x1, y1), Pointf(x2, y2), RED);
}

void JumpPoint::Update()
{
	m_curFrm++;
	if (m_curFrm > 11)
		m_curFrm = 8;
}

void JumpPoint::SetPoint(POINT pt)
{
	m_pt = pt;
	m_info.mapX = pt.x / CXTILE;
	m_info.mapY = pt.y / CYTILE;
}

bool JumpPoint::OnMouseMove(POINT ptMap, POINT mousePt)
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

bool JumpPoint::IsSelected()
{
	if (m_isMouseMove || m_isSelected) {
		return true;
	}

	return false;
}

void JumpPoint::SetSelected(BOOL flag)
{
	m_isSelected = flag;
}
//
//bool JumpPoint::IsSelected(POINT ptMap, POINT mousePt)
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


JumpPointManager::JumpPointManager(void)
{
	m_uinID = 1;
	m_pListCtrl = NULL;
	m_mapId = 0;
	m_lastTick = 0;
	m_isDrawPostion = FALSE;
}

JumpPointManager::~JumpPointManager(void)
{
	Release();
}

void JumpPointManager::InitListCtrl(CListCtrl* listCtrl)
{
	m_pListCtrl = listCtrl;
}

void JumpPointManager::Release()
{
	m_uinID = 1;

	while (!m_objMap.empty()) {
		JumpPoint* pObj = m_objMap.begin()->second;
		delete pObj;
		m_objMap.erase(m_objMap.begin());
	}
}

bool JumpPointManager::AddJumpPoint(JumpPoint* obj)
{
	JumpPointInfo& info = obj->GetInfo();

	int index = m_pListCtrl->GetItemCount();
	CString strTmp;
	strTmp.Format("%d", m_uinID);
	m_pListCtrl->InsertItem(index, strTmp);			// 序号

	const stMapInfo* pMapInfo = GameTable::getMapInfo(info.targetMapId);
	if (pMapInfo == NULL) {
		return false;
	}
	const char* countryName = Misc::countrName(pMapInfo->country);
	strTmp.Format("[%d]%s-%s", pMapInfo->id, countryName, Misc::utf8ToGbk(pMapInfo->name).c_str());
	m_pListCtrl->SetItemText(index, 1, strTmp);		// 目标地图

	//strTmp.Format("[%d]%s", info.targetMapId, Misc::utf8ToGbk(pMapInfo->name).c_str());
	//m_pListCtrl->SetItemText(index, 1, strTmp);		// 目标地图
	
	strTmp.Format("%d", info.targetMapX);
	m_pListCtrl->SetItemText(index, 2, strTmp);		// 目标X

	strTmp.Format("%d", info.targetMapY);
	m_pListCtrl->SetItemText(index, 3, strTmp);		// 目标Y

	strTmp.Format("%s", info.scriptFile.c_str());
	m_pListCtrl->SetItemText(index, 4, strTmp);		// 脚本文件名

	obj->SetID(m_uinID);
	m_objMap[m_uinID] = obj;
	m_uinID++;

	return true;
}

void JumpPointManager::EditJumpPoint(JumpPoint* obj)
{
	for (int i=0; i<m_pListCtrl->GetItemCount(); ++i) {
		CString strID = m_pListCtrl->GetItemText(i, 0);
		UINT uinId = atoi(strID);
		JumpPoint* pObj = this->FindJumpPoint(uinId);
		if (pObj == NULL) continue;
		if (pObj->GetID() != obj->GetID()) continue;

		JumpPointInfo& info = obj->GetInfo();

		int index = i;

		CString strTmp;

		const stMapInfo* pMapInfo = GameTable::getMapInfo(info.targetMapId);
		if (pMapInfo == NULL) {
			continue;
		}
		const char* countryName = Misc::countrName(pMapInfo->country);
		strTmp.Format("[%d]%s-%s", pMapInfo->id, countryName, Misc::utf8ToGbk(pMapInfo->name).c_str());
		m_pListCtrl->SetItemText(index, 1, strTmp);		// 目标地图

		//strTmp.Format("[%d]%s", info.targetMapId, Misc::utf8ToGbk(pMapInfo->name).c_str());
		//m_pListCtrl->SetItemText(index, 1, strTmp);		// 目标地图

		strTmp.Format("%d", info.targetMapX);
		m_pListCtrl->SetItemText(index, 2, strTmp);		// 目标X

		strTmp.Format("%d", info.targetMapY);
		m_pListCtrl->SetItemText(index, 3, strTmp);		// 目标Y

		strTmp.Format("%s", info.scriptFile.c_str());
		m_pListCtrl->SetItemText(index, 4, strTmp);		// 脚本文件名
	}
}

void JumpPointManager::DeleteJumpPoint(UINT uinID)
{
	map<UINT,JumpPoint*>::iterator itFind = m_objMap.find(uinID);
	if (itFind == m_objMap.end())
		return;

	JumpPoint* pObj = itFind->second;
	m_objMap.erase(itFind);
	delete pObj;

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

bool JumpPointManager::LoadJumpPoint(UINT mapId)
{
	char xmlfile[260] = {0};
	sprintf(xmlfile, "jumppoint%u.xml", mapId);

	m_mapId = mapId;

	m_pListCtrl->DeleteAllItems();
	Release();

	char szFile[260] = {0};
	sprintf(szFile, "%s/%s", EditorConfig::Instance()->makeServerResPath(JUMPPOINT_PATH), xmlfile);

	xml_document doc;  
	File file;
	if (!file.open(szFile))
		return false;

	doc.load_buffer(file.getBufferPtr(), file.getLength());

	UINT count = 0;
	UINT index = 0;
	for (pugi::xml_node node1=doc.child("jumppoint").child("point"); node1; node1=node1.next_sibling()) {
		JumpPointInfo info;
		index++;

		info.mapX = node1.attribute("MapX").as_int();
		info.mapY = node1.attribute("MapY").as_int();
		info.targetMapId = node1.attribute("TargetMapId").as_int();
		info.targetMapX = node1.attribute("TargetMapX").as_int();
		info.targetMapY = node1.attribute("TargetMapY").as_int();
		info.scriptFile = Misc::utf8ToGbk( node1.attribute("ScriptFile").as_string() );
	
		JumpPoint* pObj = new JumpPoint;
		pObj->FillInfo(info);
		pObj->Load(0);
		pObj->SetGridPos(info.mapX, info.mapY);

		if (!this->AddJumpPoint(pObj)) {
			SAFE_DELETE(pObj);
			TRACE("插入跳转点失败, file=%s", szFile);
			continue;
		}
	}


	return true;
}

void JumpPointManager::Draw(POINT ptMap)
{
	map<UINT,JumpPoint*>::iterator itBegin = m_objMap.begin();
	map<UINT,JumpPoint*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		JumpPoint* pObj = itBegin->second;
		pObj->Draw(ptMap);
		++itBegin;
	}
}

void JumpPointManager::DrawPosition(FLOAT scalX, FLOAT scalY)
{
	DWORD curTick = GetTickCount();
	if (curTick - m_lastTick > 200) {
		m_lastTick = curTick;
		m_isDrawPostion = !m_isDrawPostion;
	}

	if (!m_isDrawPostion) return;

	map<UINT,JumpPoint*>::iterator itBegin = m_objMap.begin();
	map<UINT,JumpPoint*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		JumpPoint* pObj = itBegin->second;
		pObj->DrawPositionToMiniMap(scalX, scalY);
		++itBegin;
	}
}

void JumpPointManager::Update()
{
	map<UINT,JumpPoint*>::iterator itBegin = m_objMap.begin();
	map<UINT,JumpPoint*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		JumpPoint* pObj = itBegin->second;
		pObj->Update();
		++itBegin;
	}
}

void JumpPointManager::UnSelectAll()
{
	map<UINT,JumpPoint*>::iterator itBegin = m_objMap.begin();
	map<UINT,JumpPoint*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		JumpPoint* pObj = itBegin->second;
		pObj->SetSelected(FALSE);
		++itBegin;
	}
}

JumpPoint* JumpPointManager::FindSelectJumpPoint(POINT ptMap, POINT mousePt)
{
	map<UINT,JumpPoint*>::iterator itBegin = m_objMap.begin();
	map<UINT,JumpPoint*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		JumpPoint* pObj = itBegin->second;
		bool success = pObj->OnMouseMove(ptMap, mousePt);
		if (success) return pObj;
		++itBegin;
	}

	return NULL;
}

void JumpPointManager::OnMouseMove(POINT ptMap, POINT mousePt)
{
	map<UINT,JumpPoint*>::iterator itBegin = m_objMap.begin();
	map<UINT,JumpPoint*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		JumpPoint* pObj = itBegin->second;
		pObj->OnMouseMove(ptMap, mousePt);
		++itBegin;
	}
}


JumpPoint* JumpPointManager::FindJumpPoint(UINT id)
{
	JumpPoint* pNpc = NULL;
	map<UINT,JumpPoint*>::iterator itFind = m_objMap.find(id);
	if (itFind == m_objMap.end()) {
		return NULL;
	}

	return itFind->second;
}


JumpPoint* JumpPointManager::GetSelectedJumpPoint()
{
	map<UINT,JumpPoint*>::iterator itBegin = m_objMap.begin();
	map<UINT,JumpPoint*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		JumpPoint* pObj = itBegin->second;
		if (pObj->IsSelected())
			return pObj;
		++itBegin;
	}

	return NULL;
}

UINT JumpPointManager::GetCount()
{
	return (UINT)m_objMap.size();
}

bool JumpPointManager::SaveToFile()
{
	if (this->GetCount() == 0)
		return false;

	char szFile[260] = {0};
	sprintf(szFile, "%s/jumppoint%u.xml", EditorConfig::Instance()->makeServerResPath(JUMPPOINT_PATH), m_mapId);
	ofstream ofOut(szFile);

	char szLine[10 * 1024];
	ZeroMemory(szLine, sizeof(szLine));

	sprintf(szLine, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n\r\n");
	Misc::writeUTF8Str(ofOut, szLine);

	sprintf(szLine, "<jumppoint>\r\n\r\n");
	Misc::writeUTF8Str(ofOut, szLine);


	map<UINT,JumpPoint*>::iterator itBegin = m_objMap.begin();
	map<UINT,JumpPoint*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		JumpPoint* pObj = itBegin->second;
		JumpPointInfo& info = pObj->GetInfo();

		sprintf(szLine, "\t<point MapX=\"%d\" MapY=\"%d\" TargetMapId=\"%d\" TargetMapX=\"%d\" TargetMapY=\"%d\" ScriptFile=\"%s\" />\r\n",
			info.mapX, info.mapY, info.targetMapId, info.targetMapX, info.targetMapY, info.scriptFile.c_str());
		Misc::writeUTF8Str(ofOut, szLine);

		++itBegin;
	}

	sprintf(szLine, "\r\n</jumppoint>\r\n\r\n");
	Misc::writeUTF8Str(ofOut, szLine);

	return true;
}

