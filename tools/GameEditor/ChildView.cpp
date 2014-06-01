// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "GameEditor.h"
#include "ChildView.h"
#include ".\childview.h"
#include "DropItemDlg.h"
#include "CreateMonDlg.h"
#include "CreateNpcDlg.h"
#include "CreateJumpPointDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CChildView

CChildView::CChildView()
{
	m_miniMap = NULL;
	m_isMoveMap = FALSE;
	m_ptMap.SetPoint(0,0);
	m_ptRDown.SetPoint(0,0);
	m_ptRDownMap.SetPoint(0,0);
	mMouseCell.SetPoint(0,0);
	m_isShowMainDlg = TRUE;
	m_curMapId = 0;
	m_ptLDown.SetPoint(0, 0);
	m_ptLDownMon.SetPoint(0, 0);
	m_ptLDownNPC.SetPoint(0,0);
	m_ptLDownJumpPoint.SetPoint(0,0);
	m_isMoveMon = FALSE;
	m_isMoveNPC = FALSE;
	m_isMoveJumpPoint = FALSE;
	m_isMoveMiniMap = FALSE;
	m_isViewBlock = FALSE;

	g_pMonMgr = &m_mnstMgr;
	g_pNpcMgr = &m_npcMgr;
	g_pJumpPointMgr = &m_jumpPointMgr;
}

CChildView::~CChildView()
{
	//delete pDDraw;
	//pDDraw = NULL;

	//delete pText;
	//pText = NULL;

	// 释放所有资源
	IEngine* pEngine = GetEngine();
	ReleaseEngine( pEngine );
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_VIEW_MAIN, OnViewMain)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAIN, OnUpdateViewMain)
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_VIEW_BLOCK, OnViewBlock)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BLOCK, OnUpdateViewBlock)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	
	return TRUE;
}

void CChildView::DrawMap()
{
	SetMapPt((int)m_ptMap.x, (int)m_ptMap.y);
	m_map.setLayerVisible("block", m_isViewBlock?true:false);
	m_map.render();
	m_npcMgr.Draw(m_ptMap);
	m_mnstMgr.Draw(m_ptMap);
	m_jumpPointMgr.Draw(m_ptMap);
}

void CChildView::DrawMiniMap()
{
	if (!CommData::isShowMiniMap)
		return;

	if (m_miniMap == NULL)
		return;

	IEngine* pEngine = GetEngine();
	IVideo* pVideo = pEngine->getVideo();

	float scalX = m_mapScalX;
	float scalY = m_mapScalY;

	int screenW = pEngine->getScreenWidth();
	int screenH = pEngine->getScreenHeight();
	
	// 小地图
	pVideo->drawTexture(m_miniMap, NULL, NULL, &ZeroPoint);
	
	m_mnstMgr.DrawToMiniMap(scalX, scalY);

	// 画出当前地图在小地图上的位置
#if 0
	// 定位到格子
	int x1 = (m_ptMap.x) / CXTILE;
	int y1 = (m_ptMap.y) / CYTILE;
	int x2 = (m_ptMap.x + screenW) / CXTILE;
	int y2 = (m_ptMap.y + screenH) / CYTILE;
	x1 /= scalX;
	x2 /= scalX;
	y1 /= scalY;
	y2 /= scalY;
	x1 *= CXTILE;
	x2 *= CXTILE;
	y1 *= CYTILE;
	y2 *= CYTILE;
#else
	// 定位到像素
	int x1 = (m_ptMap.x);
	int y1 = (m_ptMap.y);
	int x2 = (m_ptMap.x + screenW);
	int y2 = (m_ptMap.y + screenH);
	x1 /= scalX;
	x2 /= scalX;
	y1 /= scalY;
	y2 /= scalY;
#endif
	Pointf p1(x1, y1);
	Pointf p2(x2, y2);
	pVideo->drawRect(p1, p2, BLUE);

	// 画出怪物分布
	m_mnstMgr.DrawMonsterPosition(scalX, scalY);

	// 画出NPC分布
	m_npcMgr.DrawNpcPosition(scalX, scalY);

	// 画出跳转点分布
	m_jumpPointMgr.DrawPosition(scalX, scalY);

	// 小地图背景色
	Pointf p11(0, -1);
	Pointf p22(m_miniMap->width() + 1, m_miniMap->height());
	pVideo->drawRect(p11, p22, MINIMAP_BOARD, 2.0f);
}

void CChildView::DrawPostionInfo()
{
	//if (!m_map.isLoaded())
	//	return;

	IEngine* pEngine = GetEngine();
	if (pEngine == NULL) return;
	IVideo* pVideo = pEngine->getVideo();
	if (pVideo == NULL) return;
	easy2d::IFont* pFont = pEngine->getFont();
	if (pFont == NULL) return;

	//const float FPS_X = 3;
	//const float FPS_Y = 2;
	//const stStatus& status = pVideo->getStatus();
	//const char* format = "[Status]\n"
	//	"FPS: %u \n"
	//	"Draw: %u \n"
	//	"Sprites: %u \n"
	//	"TexMem: %u M \n";
	//char szBuffer[512];
	//sprintf(szBuffer, format, status.fps, status.drawCount, status.spriteCount, TextureMgr::instance().getTexMemSize()/1024/1024);
	//pFont->drawString(FPS_X, FPS_Y, szBuffer, 0xFF00FF00, DefaultFontEffect);


	CString strPosInfo;
	strPosInfo.Format("坐标：%d, %d", mMouseCell.x, mMouseCell.y);
	stFontEffect fontEft;
	//fontEft.color = RED;
	pFont->drawString(20, pEngine->getScreenHeight()-40, strPosInfo, RED, fontEft);	
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	// TODO: 在此处添加消息处理程序代码

	//// 不要为绘制消息而调用 CWnd::OnPaint()
	IEngine* pEngine = GetEngine();
	if (pEngine == NULL)
		return;

	//pEngine->externalUpdate(30);

	pEngine->externalClear();

	DrawMap();
	DrawMiniMap();
	DrawPostionInfo();

	pEngine->externalSwap();
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CWnd::OnEraseBkgnd(pDC);
	return FALSE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	int width = lpCreateStruct->cx;
	int height = lpCreateStruct->cy;

	// 创建主对话框
	CRect rt;
	m_pMainDlg = new CMainDlg;
	m_pMainDlg->Create(IDD_DLG_MAIN, this);
	m_pMainDlg->GetWindowRect(&rt);
	m_pMainDlg->SetWindowPos(NULL, 870, 100, rt.Width(), rt.Height(), 0);
	m_pMainDlg->ShowWindow(SW_SHOW);

	m_mnstMgr.InitListCtrl(&m_pMainDlg->m_pMonsterDlg->m_monList);
	m_npcMgr.InitListCtrl(&m_pMainDlg->m_pNpcInfoDlg->m_npcList);
	m_jumpPointMgr.InitListCtrl(&m_pMainDlg->m_pJumpPointDlg->m_jumpPointList);

	
	//////////////////////////////////////////////////////////////////////////
	IEngine* pEngine = CreateEngine(CWnd::GetSafeHwnd());
	pEngine->setTitle( "OpenglES2-游戏窗口" );
	pEngine->start(width, height);
	pEngine->setBackgroundColor(0xFF808080);
	pEngine->setLockFPS(60);

	// 加载字体
	stFontConfig cfg;
	cfg.name = "宋体";
	cfg.file = "assets/fonts/simsun.ttc";
	cfg.fontSize = 12;
	ResourceLoader& readLoader = ResourceLoader::instance();
	easy2d::IFont* pFont = pEngine->getFont();
	pFont->loadFont(cfg);

	//////////////////////////////////////////////////////////////////////////

	SetTimer(1, 150, NULL);
	SetTimer(2, 1, NULL);
	
	return 0;
}

BOOL CChildView::LoadMap(UINT mapId)
{
	m_curMapId = mapId;

	const stMapInfo* info = GameTable::getMapInfo(mapId);
	if (info == NULL)
		return FALSE;

	// 造成快速2次加载地图会宕机
	//TextureMgr::instance().unloadAll();

	// 加载地图
	CString strMapFileName;
	strMapFileName.Format("%s/scene/%s/%s.tmx", EditorConfig::Instance()->getClientPath(), info->mapfile, info->mapfile);
	m_map.load(strMapFileName);

	// 加载小地图文件
	CString strMiniMapFileName;
	strMiniMapFileName.Format("%s/scene/%s/%s.png", EditorConfig::Instance()->getClientPath(), info->mapfile, info->mapfile);
	m_miniMap = TextureMgr::instance().loadTexture(strMiniMapFileName, false);
	if (m_miniMap == NULL)
		return FALSE;

	// 设置小地图
	int mapW = m_map.getMapWidth() * m_map.getTileWidth();
	int mapH = m_map.getMapHeight() * m_map.getTileHeight();
	m_mapScalX = (FLOAT)mapW / (FLOAT)m_miniMap->width();
	m_mapScalY = (FLOAT)mapH / (FLOAT)m_miniMap->height();

	// 加载NPC数据
	m_npcMgr.LoadNpc(mapId);

	// 加载怪物数据
	m_mnstMgr.LoadMonster(mapId);

	// 加载跳转点数据
	m_jumpPointMgr.LoadJumpPoint(mapId);

	// 设置地图坐标
	SetMapPt(m_ptMap.x, m_ptMap.y);

	// 更新画面
	Invalidate();

	return TRUE;
}

BOOL CChildView::CreateMonster(MONINFO* monInfo)
{
	const stMonBase* base = GameTable::getMonBase(monInfo->race);
	if (base == NULL) {
		AfxMessageBox("创建怪物失败，无效的种类");
		return FALSE;
	}

	monInfo->mapId = m_curMapId;

	IEngine* pEngine = GetEngine();
	if (pEngine == NULL)
		return FALSE;

	int birthX = (m_ptMap.x + pEngine->getScreenWidth()/2) / CXTILE;
	int birthY = (m_ptMap.y + pEngine->getScreenHeight()/2) / CYTILE;
	//int birthW = monInfo->birthRect.cx;
	//int birthH = monInfo->birthRect.cy;

	monInfo->birthRect.x = birthX;
	monInfo->birthRect.y = birthY;
	//monInfo->birthRect.cx = centerY - birthH;
	//monInfo->birthRect.cy = centerY + birthH;

	Monster* pMon = new Monster;
	pMon->SetName(base->name);
	pMon->SetNum(monInfo->num);
	pMon->FillInfo(*monInfo);
	pMon->Load(base->id);
	pMon->SetGridPos(birthX, birthY);

	if (!m_mnstMgr.AddMonster(pMon)) {
		delete pMon;
		pMon = NULL;
		TRACE("插入怪物失败, race=%u", monInfo->race);
		return FALSE;
	}

	if ( !m_mnstMgr.SaveToFile() ) {
		AfxMessageBox("保存失败!");
	}

	return TRUE;
}

BOOL CChildView::EditMonster(UINT monUid, MONINFO* monInfo)
{
	Monster* pMon = m_mnstMgr.FindMonster(monUid);
	if (pMon == NULL) {
		AfxMessageBox("未找到指定ID的怪物");
		return FALSE;
	}

	const stMonBase* base = GameTable::getMonBase(monInfo->race);
	if (base == NULL) {
		AfxMessageBox("未找到指定种类的怪物，无效的种类");
		return FALSE;
	}

	monInfo->mapId = m_curMapId;

	int birthX = pMon->GetPoint().x / CXTILE;
	int birthY = pMon->GetPoint().y / CYTILE;
	int birthW = monInfo->birthRect.getWidth();
	int birthH = monInfo->birthRect.getHeight();

	monInfo->birthRect.x = birthX;
	monInfo->birthRect.y = birthY;
	monInfo->birthRect.cx = birthW;
	monInfo->birthRect.cy = birthH;

	pMon->SetName(base->name);
	pMon->SetNum(monInfo->num);
	pMon->FillInfo(*monInfo);
	pMon->Load(base->id);
	pMon->SetGridPos(birthX, birthY);

	m_mnstMgr.EditMonster(pMon);

	if ( !m_mnstMgr.SaveToFile() ) {
		AfxMessageBox("保存失败!");
	}

	return TRUE;
}

BOOL CChildView::SelectMonster(UINT monUid)
{
	IEngine* pEngine = GetEngine();
	if (pEngine == NULL)
		return FALSE;

	Monster* pMon = m_mnstMgr.FindMonster(monUid);
	if (pMon != NULL) {
		m_mnstMgr.UnSelectAll();
		pMon->SetSelected(TRUE);
		POINT pt = pMon->GetPoint();
		SetMapPt(pt.x - pEngine->getScreenWidth()/2, pt.y - pEngine->getScreenHeight()/2);
		return TRUE;
	}

	return FALSE;
}

// 创建NPC
BOOL CChildView::CreateNpc(NPCINFO* npcInfo)
{
	const stNpcBase* base = GameTable::getNpcBase(npcInfo->baseId);
	if (base == NULL) {
		AfxMessageBox("创建怪物失败，无效的种类");
		return FALSE;
	}

	IEngine* pEngine = GetEngine();
	if (pEngine == NULL)
		return FALSE;

	int birthX = (m_ptMap.x + pEngine->getScreenWidth()/2) / CXTILE;
	int birthY = (m_ptMap.y + pEngine->getScreenHeight()/2) / CYTILE;

	Npc* pNpc = new Npc;
	pNpc->SetName(base->name);
	pNpc->FillNpcInfo(*npcInfo);
	pNpc->Load(base->id);
	pNpc->SetGridPos(birthX, birthY);

	if (!m_npcMgr.AddNpc(pNpc)) {
		delete pNpc;
		pNpc = NULL;
		TRACE("插入怪物失败, race=%u", npcInfo->baseId);
		return FALSE;
	}

	if ( !m_npcMgr.SaveToFile() ) {
		AfxMessageBox("保存失败!");
	}

	return TRUE;
}

// 修改NPC属性
BOOL CChildView::EditNpc(UINT npcUid, NPCINFO* npcInfo)
{
	Npc* pNpc = m_npcMgr.FindNpc(npcUid);
	if (pNpc == NULL) {
		AfxMessageBox("未找到指定ID的NPC");
		return FALSE;
	}

	const stNpcBase* base = GameTable::getNpcBase(npcInfo->baseId);
	if (base == NULL) {
		AfxMessageBox("未找到指定种类的怪物，无效的种类");
		return FALSE;
	}

	int birthX = pNpc->GetPoint().x / CXTILE;
	int birthY = pNpc->GetPoint().y / CYTILE;

	pNpc->SetName(base->name);
	pNpc->FillNpcInfo(*npcInfo);
	pNpc->Load(base->id);
	pNpc->SetGridPos(birthX, birthY);

	m_npcMgr.EditNpc(pNpc);

	if ( !m_npcMgr.SaveToFile() ) {
		AfxMessageBox("保存失败!");
	}

	return TRUE;
}

BOOL CChildView::SelectNpc(UINT monUid)
{
	IEngine* pEngine = GetEngine();
	if (pEngine == NULL)
		return FALSE;

	Npc* pNpc = m_npcMgr.FindNpc(monUid);
	if (pNpc != NULL) {
		m_npcMgr.UnSelectAll();
		pNpc->SetSelected(TRUE);
		POINT pt = pNpc->GetPoint();
		SetMapPt(pt.x - pEngine->getScreenWidth()/2, pt.y - pEngine->getScreenHeight()/2);
		return TRUE;
	}

	return FALSE;
}



// 创建跳转点
BOOL CChildView::CreateJumpPoint(JumpPointInfo* info)
{
	IEngine* pEngine = GetEngine();
	if (pEngine == NULL)
		return FALSE;

	int birthX = (m_ptMap.x + pEngine->getScreenWidth()/2) / CXTILE;
	int birthY = (m_ptMap.y + pEngine->getScreenHeight()/2) / CYTILE;

	JumpPoint* pObj = new JumpPoint;
	pObj->FillInfo(*info);
	pObj->Load();
	pObj->SetGridPos(birthX, birthY);

	if (!m_jumpPointMgr.AddJumpPoint(pObj)) {
		delete pObj;
		pObj = NULL;
		TRACE("插入传送点失败");
		return FALSE;
	}

	if ( !m_jumpPointMgr.SaveToFile() ) {
		AfxMessageBox("保存失败!");
	}

	return FALSE;
}

// 修改跳转点属性
BOOL CChildView::EditJumpPoint(UINT uid, JumpPointInfo* info)
{
	JumpPoint* pObj = m_jumpPointMgr.FindJumpPoint(uid);
	if (pObj == NULL) {
		AfxMessageBox("未找到指定ID的传送点");
		return FALSE;
	}

	int birthX = pObj->GetPoint().x / CXTILE;
	int birthY = pObj->GetPoint().y / CYTILE;

	pObj->FillInfo(*info);
	pObj->Load();
	pObj->SetGridPos(birthX, birthY);

	m_jumpPointMgr.EditJumpPoint(pObj);

	if ( !m_jumpPointMgr.SaveToFile() ) {
		AfxMessageBox("保存失败!");
	}
	return FALSE;
}

// 选中跳转点
BOOL CChildView::SelectJumpPoint(UINT uid)
{
	IEngine* pEngine = GetEngine();
	if (pEngine == NULL)
		return FALSE;

	JumpPoint* pJumpPoint = m_jumpPointMgr.FindJumpPoint(uid);
	if (pJumpPoint != NULL) {
		m_jumpPointMgr.UnSelectAll();
		pJumpPoint->SetSelected(TRUE);
		POINT pt = pJumpPoint->GetPoint();
		SetMapPt(pt.x - pEngine->getScreenWidth()/2, pt.y - pEngine->getScreenHeight()/2);
		return TRUE;
	}

	return FALSE;
}



void CChildView::SetMapPt(int x, int y)
{
	IEngine* pEngine = GetEngine();
	if (pEngine == NULL)
		return;

	m_ptMap.x = x;
	m_ptMap.y = y;

	int mapW = m_map.getMapWidth() * m_map.getTileWidth();
	int mapH = m_map.getMapHeight() * m_map.getTileHeight();
	if (m_ptMap.x < 0) m_ptMap.x = 0;
	if (m_ptMap.y < 0) m_ptMap.y = 0;
	if (m_ptMap.x+(LONG)pEngine->getScreenWidth() > mapW) m_ptMap.x = mapW - (LONG)pEngine->getScreenWidth();
	if (m_ptMap.y+(LONG)pEngine->getScreenHeight() > mapH) m_ptMap.y = mapH - (LONG)pEngine->getScreenHeight();

	m_map.setPostion(m_ptMap.x, m_ptMap.y);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd::SetCapture();

	m_isMoveMap = TRUE;
	m_ptRDown = point;
	m_ptRDownMap = m_ptMap;

	CWnd::OnRButtonDown(nFlags, point);
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_isMoveMap = FALSE;

	CWnd::OnRButtonUp(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	IEngine* pEngine = GetEngine();
	if (pEngine == NULL)
		return;

	int screenW = pEngine->getScreenWidth();
	int screenH = pEngine->getScreenHeight();

	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_isMoveMiniMap) {
		FLOAT mapX = (FLOAT)((UINT)(point.x * m_mapScalX)) - screenW/2;
		FLOAT mapY = (FLOAT)((UINT)(point.y * m_mapScalY)) - screenH/2;
		SetMapPt((int)mapX, (int)mapY);

		Invalidate();
	} else if (m_isMoveMap) {
		int dx = point.x - m_ptRDown.x;
		int dy = point.y - m_ptRDown.y;
		SetMapPt(m_ptRDownMap.x - dx, m_ptRDownMap.y - dy);

		Invalidate();
	} else if (m_isMoveMon) {
		// 移动怪物
		Monster* pMon = m_mnstMgr.GetSelectedMonster();
		if (pMon != NULL) {
			int dx = point.x - m_ptLDown.x;
			int dy = point.y - m_ptLDown.y;
			
			CString strPosInfo;
			int x = (m_ptMap.x + point.x) / CXTILE;
			int y = (m_ptMap.y + point.y) / CYTILE;
			if (m_ptLast.x != x || m_ptLast.y != y) {
				m_ptLast.SetPoint(x, y);
				mMouseCell = m_ptLast;
				strPosInfo.Format("%d, %d", x, y);
				m_pMainDlg->m_posInfo.SetWindowText(strPosInfo);

				POINT pt;
				pt.x = x * CXTILE;
				pt.y = y * CYTILE;
				pMon->SetPoint(pt);
			}

			Invalidate();
		}
	} else if (m_isMoveNPC) {
		// 移动NPC
		Npc* pNpc = m_npcMgr.GetSelectedNpc();
		if (pNpc != NULL) {
			int dx = point.x - m_ptLDown.x;
			int dy = point.y - m_ptLDown.y;

			CString strPosInfo;
			int x = (m_ptMap.x + point.x) / CXTILE;
			int y = (m_ptMap.y + point.y) / CYTILE;
			if (m_ptLast.x != x || m_ptLast.y != y) {
				m_ptLast.SetPoint(x, y);
				mMouseCell = m_ptLast;
				strPosInfo.Format("%d, %d", x, y);
				m_pMainDlg->m_posInfo.SetWindowText(strPosInfo);

				POINT pt;
				pt.x = x * CXTILE;
				pt.y = y * CYTILE;
				pNpc->SetPoint(pt);
			}

			Invalidate();
		}
	} else if (m_isMoveJumpPoint) {
		// 移动跳转点
		JumpPoint* pJumpPoint = m_jumpPointMgr.GetSelectedJumpPoint();
		if (pJumpPoint != NULL) {
			int dx = point.x - m_ptLDown.x;
			int dy = point.y - m_ptLDown.y;

			CString strPosInfo;
			int x = (m_ptMap.x + point.x) / CXTILE;
			int y = (m_ptMap.y + point.y) / CYTILE;
			if (m_ptLast.x != x || m_ptLast.y != y) {
				m_ptLast.SetPoint(x, y);
				mMouseCell = m_ptLast;
				strPosInfo.Format("%d, %d", x, y);
				m_pMainDlg->m_posInfo.SetWindowText(strPosInfo);

				POINT pt;
				pt.x = x * CXTILE;
				pt.y = y * CYTILE;
				pJumpPoint->SetPoint(pt);
			}

			Invalidate();
		}
	} else {
		m_npcMgr.OnMouseMove(m_ptMap, point);
		m_mnstMgr.OnMouseMove(m_ptMap, point);

		CString strPosInfo;
		int x = (m_ptMap.x + point.x) / CXTILE;
		int y = (m_ptMap.y + point.y) / CYTILE;
		if (m_ptLast.x != x || m_ptLast.y != y) {
			m_ptLast.SetPoint(x, y);
			mMouseCell = m_ptLast;
			strPosInfo.Format("%d, %d", x, y);
			m_pMainDlg->m_posInfo.SetWindowText(strPosInfo);
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nChar >= 'A' && nChar <= 'Z') nChar += 32;
	switch(nChar)
	{
	case 'm':
		// 显示和隐藏小地图
		CommData::isShowMiniMap = !CommData::isShowMiniMap;
		Invalidate();
		break;
	default:
	    break;
	}

	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint ptMap = GetMapPt();
	const int SPEED = 200;
	switch(nChar)
	{
	case VK_UP:
		ptMap.y -= SPEED;
		Invalidate();
		break;
	case VK_DOWN:
		ptMap.y += SPEED;
		Invalidate();
		break;
	case VK_LEFT:
		ptMap.x -= SPEED;
		Invalidate();
	    break;
	case VK_RIGHT:
		ptMap.x += SPEED;
		Invalidate();
	    break;
	case VK_DELETE:
		{
			Monster* pMon = m_mnstMgr.GetSelectedMonster();
			if (pMon) {
				m_mnstMgr.DeleteMonster(pMon->GetID());
				if ( !m_mnstMgr.SaveToFile() ) {
					AfxMessageBox("保存失败!");
				}
			}

			Npc* pNpc = m_npcMgr.GetSelectedNpc();
			if (pNpc) {
				m_npcMgr.DeleteNpc(pNpc->GetID());
				if ( !m_npcMgr.SaveToFile() ) {
					AfxMessageBox("保存失败!");
				}
			}

			JumpPoint* pJumpPoint = m_jumpPointMgr.GetSelectedJumpPoint();
			if (pJumpPoint) {
				m_jumpPointMgr.DeleteJumpPoint(pJumpPoint->GetID());
				if ( !m_jumpPointMgr.SaveToFile() ) {
					AfxMessageBox("保存失败!");
				}
			}
		}
		break;
	default:
	    break;
	}

	SetMapPt(ptMap.x, ptMap.y);

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::SetCapture();

	IEngine* pEngine = GetEngine();
	if (pEngine == NULL)
		return;

	int screenW = pEngine->getScreenWidth();
	int screenH = pEngine->getScreenHeight();

	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (CommData::isShowMiniMap && m_miniMap && (point.x < m_miniMap->width() && point.y < m_miniMap->height())) {
		// 处理小地图的点击
		m_isMoveMiniMap = TRUE;
		FLOAT mapX = (FLOAT)((UINT)(point.x * m_mapScalX)) - screenW/2;
		FLOAT mapY = (FLOAT)((UINT)(point.y * m_mapScalY)) - screenH/2;
		SetMapPt((int)mapX, (int)mapY);
		Invalidate();
	} else {
		// 显示NPC信息
		m_npcMgr.UnSelectAll();
		Npc* pNpc = m_npcMgr.FindSelectNpc(m_ptMap, point);
		if (pNpc != NULL) {
			pNpc->SetSelected(TRUE);
			m_ptLDown = point;
			m_isMoveNPC = TRUE;
			m_ptLDownNPC = pNpc->GetPoint();
		}

		// 显示怪物信息
		m_mnstMgr.UnSelectAll();
		Monster* pMon = m_mnstMgr.FindSelectMonster(m_ptMap, point);
		if (pMon != NULL) {
			pMon->SetSelected(TRUE);
			m_ptLDown = point;
			m_isMoveMon = TRUE;
			m_ptLDownMon = pMon->GetPoint();
		}

		// 显示跳转点信息
		m_jumpPointMgr.UnSelectAll();
		JumpPoint* pJumpPoint = m_jumpPointMgr.FindSelectJumpPoint(m_ptMap, point);
		if (pJumpPoint != NULL) {
			pJumpPoint->SetSelected(TRUE);
			m_ptLDown = point;
			m_isMoveJumpPoint = TRUE;
			m_ptLDownJumpPoint = pJumpPoint->GetPoint();
		}
	}
	

	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ReleaseCapture();

	if (m_isMoveMon) {
		if ( !m_mnstMgr.SaveToFile() ) {
			AfxMessageBox("保存失败!");
		}
	}

	if (m_isMoveNPC) {
		if ( !m_npcMgr.SaveToFile() ) {
			AfxMessageBox("保存失败!");
		}
	}

	if (m_isMoveJumpPoint) {
		if ( !m_jumpPointMgr.SaveToFile() ) {
			AfxMessageBox("保存失败!");
		}
	}

	m_isMoveNPC = FALSE;
	m_isMoveMon = FALSE;
	m_isMoveJumpPoint = FALSE;
	m_isMoveMiniMap = FALSE;


	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	// 修改怪物
	Monster* pMon = m_mnstMgr.FindSelectMonster(m_ptMap, point);
	if (pMon != NULL) {
		pMon->SetSelected(TRUE);

		if (m_pMainDlg->isCheckDropItem()) {
			CDropItemDlg dlg;
			dlg.SetDropId(pMon->GetMonInfo().dropInfo[0].dropId);
			dlg.DoModal();
		} else {
			CCreateMonDlg dlg;
			dlg.EditMonster(pMon);
			dlg.DoModal();
		}
	}


	// 修改NPC
	Npc* pNpc = m_npcMgr.FindSelectNpc(m_ptMap, point);
	if (pNpc != NULL) {
		pNpc->SetSelected(TRUE);

		CCreateNpcDlg dlg;
		dlg.EditNpc(pNpc);
		dlg.DoModal();

		/*if (m_pMainDlg->isCheckDropItem()) {
			CDropItemDlg dlg;
			dlg.SetDropId(pNpc->GetMonInfo().dropInfo[0].dropId);
			dlg.DoModal();
		} else {
			CCreateMonDlg dlg;
			dlg.EditMonster(pNpc);
			dlg.DoModal();
		}*/
	}


	// 修改跳转点
	JumpPoint* pJumpPoint = m_jumpPointMgr.FindSelectJumpPoint(m_ptMap, point);
	if (pJumpPoint != NULL) {
		pJumpPoint->SetSelected(TRUE);

		CCreateJumpPointDlg dlg;
		dlg.EditJumpPoint(pJumpPoint);
		dlg.DoModal();
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}


void CChildView::OnViewMain()
{
	// TODO: 在此添加命令处理程序代码
	m_isShowMainDlg = !m_isShowMainDlg;
	if (m_isShowMainDlg) {
		m_pMainDlg->ShowWindow(SW_SHOW);
	} else {
		m_pMainDlg->ShowWindow(SW_HIDE);
	}
}

void CChildView::OnUpdateViewMain(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_isShowMainDlg);
}



LRESULT CChildView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CWnd::WindowProc(message, wParam, lParam);
}

void CChildView::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch(nIDEvent)
	{
	case 1:
		m_npcMgr.Update();
		m_mnstMgr.Update();
		m_jumpPointMgr.Update();
		Invalidate();
		break;
	case 2:
		{
			IEngine* pEngine = GetEngine();
			if (pEngine != NULL)
				pEngine->externalUpdate(30);
		}
	    break;
	}

	CWnd::OnTimer(nIDEvent);
}


void CChildView::OnViewBlock()
{
	// TODO: 在此添加命令处理程序代码
	m_isViewBlock = !m_isViewBlock;
}

void CChildView::OnUpdateViewBlock(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_isViewBlock);
}




void CChildView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	IEngine* pEngine = GetEngine();
	if (pEngine == NULL)
		return;

	pEngine->externalResize(cx, cy);
}

void CChildView::OnDestroy()
{
	__super::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	delete m_pMainDlg;
	m_npcMgr.Release();
	m_mnstMgr.Release();
}
