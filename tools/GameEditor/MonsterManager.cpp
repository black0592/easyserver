#include "StdAfx.h"
#include ".\monstermanager.h"
#include "CommData.h"


#define MONINFO_PATH	"moninfo"

Monster::Monster()
{
	m_isMouseMove = FALSE;
	m_isSelected = FALSE;
	m_nameColor = YELLOW;
	m_Effect = EF_NORMAL;
	m_baseId = 0;
	m_id = 0;
	m_num = 1;
	m_pt.x = m_pt.y = 500;
	m_curFrm = 8;
}


Monster::~Monster()
{

}

void Monster::SetID(UINT id)
{
	m_id = id;
}

UINT Monster::GetID()
{
	return m_id;
}

void Monster::SetNum(UINT num)
{
	m_num = num;
}

UINT Monster::GetNum()
{
	return m_num;
}

void Monster::SetName(const char* name)
{
	m_name = Misc::utf8ToGbk(name);
}

void Monster::SetName(const std::string& name)
{
	m_name = Misc::utf8ToGbk(name);
}

const char* Monster::GetName()
{
	return m_name.c_str();
}

UINT Monster::GetLevel()
{
	const stMonBase* base = GameTable::getMonBase(m_baseId);
	return base ? base->level : 0;
}

string Monster::getBaseName()
{
	const stMonBase* base = GameTable::getMonBase(m_baseId);
	return base ? base->name : "(null)";
}

void Monster::FillInfo(MONINFO& info)
{
	m_info = info;
}

MONINFO& Monster::GetMonInfo()
{
	return m_info;
}


POINT Monster::GetPoint()
{
	return m_pt;
}


void Monster::SetPoint(POINT pt)
{
	m_pt = pt;
	m_info.birthRect.x = pt.x / CXTILE;
	m_info.birthRect.y = pt.y / CYTILE;
}

void Monster::SetGridPos(int x, int y)
{
	POINT pt;
	pt.x = x * CXTILE;
	pt.y = y * CYTILE;
	SetPoint(pt);
}

bool Monster::Load(UINT baseId)
{
	const stMonBase* base = GameTable::getMonBase(baseId);
	if (base == NULL) {
		TRACE("未找到怪物");
		return false;
	}

	m_baseId = baseId;

	CString strName;
	strName.Format("%s/monster/02%03d002.plist", EditorConfig::Instance()->getClientPath(), base->imgId);
	return mSpriteSheet.load(strName);
}

void Monster::Draw(POINT ptMap)
{
	IEngine* pEngine = GetEngine();
	IVideo* pVideo = pEngine->getVideo();
	easy2d::IFont* pFont = pEngine->getFont();

	// 画怪物身体图片
	int x = m_pt.x - ptMap.x;
	int y = m_pt.y - ptMap.y;
	mSpriteSheet.render(x+CXTILE/2, y+CYTILE/2);
	if (CommData::isShowMiniMap) {
		pVideo->drawSolidRect(Pointf(x, y), Pointf(x+CXTILE, y+CYTILE), 0x50FFFF00);
		pVideo->drawRect(Pointf(x, y), Pointf(x+CXTILE, y+CYTILE), RED, 2.0f);
	}

	if (m_isMouseMove || m_isSelected) {
		// 选中状态
		m_nameColor = RED;
		m_Effect = EF_RED;
		int x1=0, y1=0, x2=0, y2=0;
		
		// 出生区域
		x = m_pt.x - ptMap.x;
		y = m_pt.y - ptMap.y;
		int birthW = m_info.birthRect.cx * CXTILE;
		int birthH = m_info.birthRect.cy * CYTILE;
		x1 = x - birthW;
		y1 = y - birthH;
		x2 = x + birthW + CXTILE;
		y2 = y + birthH + CYTILE;
		pVideo->drawRect(Pointf(x1, y1), Pointf(x2, y2), YELLOW);

		// 视野(攻击)范围
		x = m_pt.x - ptMap.x;
		y = m_pt.y - ptMap.y;
		int viewW = m_info.view * CXTILE;
		int viewH = m_info.view * CYTILE;
		x1 = x - viewW;
		y1 = y - viewH;
		x2 = x + viewW + CXTILE;
		y2 = y + viewH + CYTILE;
		pVideo->drawRect(Pointf(x1, y1), Pointf(x2, y2), RED);
	} else {
		// 非选中状态
		m_nameColor = YELLOW;
		m_Effect = EF_NORMAL;
	}

	// 绘制名字、数量、掉落编号
	CString strName = m_name.c_str();
	if (m_num > 1) {
		CString strNum;
		strNum.Format("(数量*%d)", m_num);
		strName += strNum;
	}
	if (m_info.dropInfo[0].dropId != 0) {
		CString strDrop;
		strDrop.Format("[掉落%d]", m_info.dropInfo[0].dropId);
		strName += strDrop;
	}

	// 画怪物名字
	int frameHeight = 0;
	int offsetX = (int)strName.GetLength() * 4 - CXTILE/2;
	FRAME_INFO* frm = mSpriteSheet.getFrame(0);
	if (frm != NULL) {
		frameHeight = mSpriteSheet.getFrame(0)->height;
	} else {
		frameHeight = 86;
	}

	stFontEffect fontEff;
	pFont->drawString(x-offsetX, y-frameHeight, strName, m_nameColor, fontEff);
}

void Monster::DrawToMiniMap(FLOAT scalX, FLOAT scalY)
{
	IEngine* pEngine = GetEngine();
	IVideo* pVideo = pEngine->getVideo();

	int x = 0, y = 0;

	if (m_isMouseMove || m_isSelected) {
		// 选中状态
		m_nameColor = RED;
		m_Effect = EF_RED;
		int x1=0, y1=0, x2=0, y2=0;

		// 出生范围
		x = m_pt.x / CXTILE;
		y = m_pt.y / CYTILE;
		int birthW = m_info.birthRect.cx;
		int birthH = m_info.birthRect.cy;
		x1 = x - birthW;
		y1 = y - birthH;
		x2 = x + birthW + 1;
		y2 = y + birthH + 1;
		x1 = x1 * CXTILE / scalX;
		y1 = y1 * CYTILE / scalY;
		x2 = x2 * CXTILE / scalX;
		y2 = y2 * CYTILE / scalY;
		pVideo->drawRect(Pointf(x1, y1), Pointf(x2, y2), YELLOW);

		// 视野范围
		x = m_pt.x / CXTILE;
		y = m_pt.y / CYTILE;
		int viewW = m_info.view;
		int viewH = m_info.view;
		x1 = x - viewW;
		y1 = y - viewH;
		x2 = x + viewW + 1;
		y2 = y + viewH + 1;
		x1 = x1 * CXTILE / scalX;
		y1 = y1 * CYTILE / scalY;
		x2 = x2 * CXTILE / scalX;
		y2 = y2 * CYTILE / scalY;
		pVideo->drawRect(Pointf(x1, y1), Pointf(x2, y2), RED);
	} else {
		// 非选中状态
		m_nameColor = YELLOW;
		m_Effect = EF_NORMAL;
	}
}

void Monster::DrawPositionToMiniMap(FLOAT scalX, FLOAT scalY)
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
	pVideo->drawSolidRect(Pointf(x1, y1), Pointf(x2, y2), YELLOW);
	pVideo->drawRect(Pointf(x1, y1), Pointf(x2, y2), RED);
}

void Monster::Update()
{
	m_curFrm++;
	if (m_curFrm > 11)
		m_curFrm = 8;
}


bool Monster::OnMouseMove(POINT ptMap, POINT mousePt)
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

bool Monster::IsSelected()
{
	if (m_isMouseMove || m_isSelected) {
		return true;
	}

	return false;
}

void Monster::SetSelected(BOOL flag)
{
	m_isSelected = flag;
}


//////////////////////////////////////////////////////////////////////////



MonsterManager::MonsterManager(void)
{
	m_uinID = 1;
	m_pListCtrl = NULL;
	m_mapId = 0;
	m_lastTick = 0;
	m_isDrawMonPostion = FALSE;
}

MonsterManager::~MonsterManager(void)
{
	Release();
}

void MonsterManager::InitListCtrl(CListCtrl* listCtrl)
{
	m_pListCtrl = listCtrl;
}

void MonsterManager::Release()
{
	m_uinID = 1;

	while (!m_objMap.empty()) {
		map<UINT,Monster*>::iterator it = m_objMap.begin();
		Monster* pMonster = it->second;
		delete pMonster;
		m_objMap.erase(it);
	}
}

UINT MonsterManager::GetCount()
{
	return (UINT)m_objMap.size();
}

bool MonsterManager::AddMonster(Monster* monster)
{
	MONINFO& info = monster->GetMonInfo();

	int index = m_pListCtrl->GetItemCount();
	CString strTmp;
	strTmp.Format("%d", m_uinID);
	m_pListCtrl->InsertItem(index, strTmp);			// 序号

	strTmp.Format("[%d]%s(%d级)", monster->GetBaseID(), Misc::utf8ToGbk(monster->getBaseName()).c_str(), monster->GetLevel());
	m_pListCtrl->SetItemText(index, 1, strTmp);		// 名称

	strTmp.Format("%d", info.num);
	m_pListCtrl->SetItemText(index, 2, strTmp);		// 数量

	strTmp.Format("%d", info.ai);
	m_pListCtrl->SetItemText(index, 3, strTmp);		// 智能

	strTmp.Format("%d", info.speed);
	m_pListCtrl->SetItemText(index, 4, strTmp);		// 移速

	strTmp.Format("%d", info.view);
	m_pListCtrl->SetItemText(index, 5, strTmp);		// 视野

	strTmp.Format("%d", info.dropInfo[0].dropType);
	m_pListCtrl->SetItemText(index, 6, strTmp);		// BOSS掉落

	strTmp.Format("%d", info.dropInfo[0].dropId);
	m_pListCtrl->SetItemText(index, 7, strTmp);		// 掉落编号

	strTmp.Format("%d", info.dropInfo[0].dropProb);
	m_pListCtrl->SetItemText(index, 8, strTmp);		// 掉落概率

	strTmp.Format("%d", info.reviveTime);
	m_pListCtrl->SetItemText(index, 9, strTmp);		// 复活时间

	monster->SetID(m_uinID);
	m_objMap[m_uinID] = monster;
	m_uinID++;
	return true;
}

void MonsterManager::EditMonster(Monster* monster)
{
	for (int i=0; i<m_pListCtrl->GetItemCount(); ++i) {
		CString strID = m_pListCtrl->GetItemText(i, 0);
		UINT uinId = atoi(strID);
		Monster* pMon = this->FindMonster(uinId);
		if (pMon == NULL) continue;
		if (pMon->GetID() != monster->GetID()) continue;
		MONINFO& info = pMon->GetMonInfo();

		int index = i;

		CString strTmp;

		strTmp.Format("[%d]%s(%d级)", pMon->GetBaseID(), Misc::utf8ToGbk(pMon->getBaseName()).c_str(), pMon->GetLevel());
		m_pListCtrl->SetItemText(index, 1, strTmp);		// 名称

		strTmp.Format("%d", info.num);
		m_pListCtrl->SetItemText(index, 2, strTmp);		// 数量

		strTmp.Format("%d", info.ai);
		m_pListCtrl->SetItemText(index, 3, strTmp);		// 智能

		strTmp.Format("%d", info.speed);
		m_pListCtrl->SetItemText(index, 4, strTmp);		// 移速

		strTmp.Format("%d", info.view);
		m_pListCtrl->SetItemText(index, 5, strTmp);		// 视野

		strTmp.Format("%d", info.dropInfo[0].dropType);
		m_pListCtrl->SetItemText(index, 6, strTmp);		// BOSS掉落

		strTmp.Format("%d", info.dropInfo[0].dropId);
		m_pListCtrl->SetItemText(index, 7, strTmp);		// 掉落编号

		strTmp.Format("%d", info.dropInfo[0].dropProb);
		m_pListCtrl->SetItemText(index, 8, strTmp);		// 掉落概率

		strTmp.Format("%d", info.reviveTime);
		m_pListCtrl->SetItemText(index, 9, strTmp);		// 复活时间
	}
}

void MonsterManager::DeleteMonster(UINT uinID)
{
	map<UINT,Monster*>::iterator itFind = m_objMap.find(uinID);
	if (itFind == m_objMap.end())
		return;

	Monster* pMon = itFind->second;
	m_objMap.erase(itFind);
	delete pMon;

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

bool MonsterManager::LoadMonster(UINT mapId)
{
	char xmlfile[260] = {0};
	sprintf(xmlfile, "moninfo%u.xml", mapId);


	m_mapId = mapId;

	m_pListCtrl->DeleteAllItems();
	Release();

	char szFile[260] = {0};
	sprintf(szFile, "%s/%s", EditorConfig::Instance()->makeServerResPath(MONINFO_PATH), xmlfile);

	xml_document doc;  
	File file;
	if (!file.open(szFile))
		return false;

	doc.load_buffer(file.getBufferPtr(), file.getLength());

	ushort count = 0;
	ushort index = 0;
	for (pugi::xml_node node1=doc.child("moninfo").child("mon"); node1; node1=node1.next_sibling()) {
		MONINFO info;
		info.mapId = mapId;
		int baseId = node1.attribute("BaseId").as_int();
		int num = node1.attribute("Num").as_int();
		index++;

		info.race = baseId;
		info.num = num;

		info.ai = node1.attribute("AI").as_int();
		info.speed = node1.attribute("Speed").as_int();
		info.view = node1.attribute("View").as_int();

		info.dropInfo[0].dropId = node1.attribute("DropId").as_int();
		info.dropInfo[0].dropProb = node1.attribute("DropProb").as_int();
		info.dropInfo[0].dropType = node1.attribute("DropType").as_int();

		info.dropInfo[1].dropId = node1.attribute("DropId2").as_int();
		info.dropInfo[1].dropProb = node1.attribute("DropProb2").as_int();
		info.dropInfo[1].dropType = node1.attribute("DropType2").as_int();

		info.dropInfo[2].dropId = node1.attribute("DropId3").as_int();
		info.dropInfo[2].dropProb = node1.attribute("DropProb3").as_int();
		info.dropInfo[2].dropType = node1.attribute("DropType3").as_int();

		info.reviveTime = node1.attribute("ReviveTime").as_int();
		info.isSiegeWar = node1.attribute("SiegeWar").as_int();
		info.isEventMon = node1.attribute("EventMon").as_int();
		info.birthRect.x = node1.attribute("BirthX").as_int();
		info.birthRect.y = node1.attribute("BirthY").as_int();
		info.birthRect.cx = node1.attribute("BirthWidth").as_int();
		info.birthRect.cy = node1.attribute("BirthHeight").as_int();

		const stMonBase* base = GameTable::getMonBase(baseId);
		if (!base) {
			LOGE("读取怪物分布出错，不存在的怪物类型, file=%s, race=%u", szFile, baseId);
			continue;
		}

		Monster* pMon = new Monster;
		pMon->SetName(base->name);
		pMon->SetNum(num);
		pMon->FillInfo(info);
		pMon->Load(base->id);
		pMon->SetGridPos(info.birthRect.x, info.birthRect.y);

		if (!this->AddMonster(pMon)) {
			SAFE_DELETE(pMon);
			TRACE("插入怪物失败, file=%s, race=%u", szFile, info.race);
			continue;
		}
	}


	return true;
}

bool MonsterManager::SaveToFile()
{
	if (this->GetCount() == 0)
		return false;

	char szFile[260] = {0};
	sprintf(szFile, "%s/moninfo%u.xml", EditorConfig::Instance()->makeServerResPath(MONINFO_PATH), m_mapId);
	ofstream ofOut(szFile);

	char szLine[10 * 1024];
	ZeroMemory(szLine, sizeof(szLine));

	sprintf(szLine, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n\r\n");
	Misc::writeUTF8Str(ofOut, szLine);

	sprintf(szLine, "<moninfo>\r\n\r\n");
	Misc::writeUTF8Str(ofOut, szLine);


	map<UINT,Monster*>::iterator itBegin = m_objMap.begin();
	map<UINT,Monster*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Monster* pMon = itBegin->second;
		MONINFO& info = pMon->GetMonInfo();	

		sprintf(szLine, "\t<mon BaseId=\"%d\" Num=\"%d\" AI=\"%d\" Speed=\"%d\" View=\"%d\" BirthX=\"%d\" BirthY=\"%d\" BirthWidth=\"%d\" BirthHeight=\"%d\" DropId=\"%d\" DropProb=\"%d\" DropType=\"%d\" ReviveTime=\"%d\" />\r\n",
			pMon->GetBaseID(), info.num, info.ai, info.speed, info.view, info.birthRect.x, info.birthRect.y, info.birthRect.cx, info.birthRect.cy,
			info.dropInfo[0].dropId, info.dropInfo[0].dropProb, info.dropInfo[0].dropType, info.reviveTime);
		Misc::writeUTF8Str(ofOut, szLine);
	
		++itBegin;
	}

	sprintf(szLine, "\r\n</moninfo>\r\n\r\n");
	Misc::writeUTF8Str(ofOut, szLine);

	return true;
}

void MonsterManager::Draw(POINT ptMap)
{
	map<UINT,Monster*>::iterator itBegin = m_objMap.begin();
	map<UINT,Monster*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Monster* pMon = itBegin->second;
		pMon->Draw(ptMap);
		++itBegin;
	}
}

void MonsterManager::DrawToMiniMap(FLOAT scalX, FLOAT scalY)
{
	map<UINT,Monster*>::iterator itBegin = m_objMap.begin();
	map<UINT,Monster*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Monster* pMon = itBegin->second;
		pMon->DrawToMiniMap(scalX, scalY);
		++itBegin;
	}
}

void MonsterManager::DrawMonsterPosition(FLOAT scalX, FLOAT scalY)
{ 
	DWORD curTick = GetTickCount();
	if (curTick - m_lastTick > 200) {
		m_lastTick = curTick;
		m_isDrawMonPostion = !m_isDrawMonPostion;
	}

	if (!m_isDrawMonPostion) return;

	map<UINT,Monster*>::iterator itBegin = m_objMap.begin();
	map<UINT,Monster*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Monster* pMon = itBegin->second;
		pMon->DrawPositionToMiniMap(scalX, scalY);
		++itBegin;
	}
}

void MonsterManager::Update()
{
	map<UINT,Monster*>::iterator itBegin = m_objMap.begin();
	map<UINT,Monster*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Monster* pMon = itBegin->second;
		pMon->Update();
		++itBegin;
	}
}

void MonsterManager::UnSelectAll()
{
	map<UINT,Monster*>::iterator itBegin = m_objMap.begin();
	map<UINT,Monster*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Monster* pMon = itBegin->second;
		pMon->SetSelected(FALSE);
		++itBegin;
	}
}

Monster* MonsterManager::FindSelectMonster(POINT ptMap, POINT mousePt)
{
	map<UINT,Monster*>::iterator itBegin = m_objMap.begin();
	map<UINT,Monster*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Monster* pMon = itBegin->second;
		bool success = pMon->OnMouseMove(ptMap, mousePt);
		if (success) return pMon;
		++itBegin;
	}

	return NULL;
}

void MonsterManager::OnMouseMove(POINT ptMap, POINT mousePt)
{
	map<UINT,Monster*>::iterator itBegin = m_objMap.begin();
	map<UINT,Monster*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Monster* pMon = itBegin->second;
		pMon->OnMouseMove(ptMap, mousePt);
		++itBegin;
	}
}


Monster* MonsterManager::FindMonster(UINT id)
{
	Monster* pMon = NULL;
	map<UINT,Monster*>::iterator itFind = m_objMap.find(id);
	if (itFind == m_objMap.end()) {
		return NULL;
	}

    return itFind->second;
}


Monster* MonsterManager::GetSelectedMonster()
{
	map<UINT,Monster*>::iterator itBegin = m_objMap.begin();
	map<UINT,Monster*>::iterator itEnd = m_objMap.end();
	while (itBegin != itEnd) {
		Monster* pMon = itBegin->second;
		if (pMon->IsSelected())
			return pMon;
		++itBegin;
	}

	return NULL;
}









