// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "GameEditor.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

//#include <vld.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// 创建一个视图以占用框架的工作区
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.cx = 800;
	cs.cy = 600;
	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 否则，执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFrameWnd::OnRButtonDown(nFlags, point);
}

void CMainFrame::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFrameWnd::OnRButtonUp(nFlags, point);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFrameWnd::OnMouseMove(nFlags, point);
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch(message)
	{
	case WM_MAINDLG_LOADMAP:
		{
			// 加载地图
			m_wndView.LoadMap((UINT)lParam);
		}
		break;

		//-------------------------------------

	case WM_NEWMONDLG_CREATE_MONSTER:
		{
			MONINFO* pInfo = (MONINFO*)lParam;
			m_wndView.CreateMonster(pInfo);
			delete pInfo;
		}
		break;

	case WM_NEWMONDLG_EDIT_MONSTER:
		{
			MONINFO* pInfo = (MONINFO*)lParam;
			m_wndView.EditMonster((UINT)wParam, pInfo);
			delete pInfo;
		}
		break;

	case WM_MONDLG_SELECT_MONSTER:
		{
			m_wndView.SelectMonster((UINT)lParam);
		}
		break;

		// -----------------------------------------

	case WM_NEWMONDLG_CREATE_NPC:
		{
			NPCINFO* pInfo = (NPCINFO*)lParam;
			m_wndView.CreateNpc(pInfo);
			delete pInfo;
		}
		break;

	case WM_NEWMONDLG_EDIT_NPC:
		{
			NPCINFO* pInfo = (NPCINFO*)lParam;
			m_wndView.EditNpc((UINT)wParam, pInfo);
			delete pInfo;
		}
		break;

	case WM_MONDLG_SELECT_NPC:
		{
			m_wndView.SelectNpc((UINT)lParam);
		}
		break;


		// -----------------------------------------

	case WM_NEWMONDLG_CREATE_JUMPPOINT:
		{
			JumpPointInfo* pInfo = (JumpPointInfo*)lParam;
			m_wndView.CreateJumpPoint(pInfo);
			delete pInfo;
		}
		break;

	case WM_NEWMONDLG_EDIT_JUMPPOINT:
		{
			JumpPointInfo* pInfo = (JumpPointInfo*)lParam;
			m_wndView.EditJumpPoint((UINT)wParam, pInfo);
			delete pInfo;
		}
		break;

	case WM_MONDLG_SELECT_JUMPPOINT:
		{
			m_wndView.SelectJumpPoint((UINT)lParam);
		}
		break;


	default:
		break;
	}

	return CFrameWnd::WindowProc(message, wParam, lParam);
}
