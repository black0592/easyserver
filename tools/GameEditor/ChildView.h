// ChildView.h : CChildView 类的接口
//


#pragma once

#include "MainDlg.h"
#include "NpcManager.h"
#include "MonsterManager.h"
#include "JumpPointManager.h"
#include "MiniMapData.h"
#include "TileMap.h"

// CChildView 窗口

class CChildView : public CWnd, public IEngine::IEventListener
{
// 构造
public:
	CChildView();

// 属性
public:

// 操作
public:

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	void SetMapPt(int x, int y);
	CPoint GetMapPt() { return m_ptMap; }

	// 加载地图
	BOOL LoadMap(UINT mapId);

	void DrawMap();
	void DrawMiniMap();
	void DrawPostionInfo();

	// 创建怪物
	BOOL CreateMonster(MONINFO* monInfo);

	// 修改怪物属性
	BOOL EditMonster(UINT monUid, MONINFO* monInfo);

	// 选中怪物
	BOOL SelectMonster(UINT monUid);




	// 创建NPC
	BOOL CreateNpc(NPCINFO* npcInfo);

	// 修改NPC属性
	BOOL EditNpc(UINT npcUid, NPCINFO* npcInfo);

	// 选中NPC
	BOOL SelectNpc(UINT npcUid);




	// 创建跳转点
	BOOL CreateJumpPoint(JumpPointInfo* info);

	// 修改跳转点属性
	BOOL EditJumpPoint(UINT uid, JumpPointInfo* info);

	// 选中跳转点
	BOOL SelectJumpPoint(UINT uid);

private:
	// map
	BOOL m_isMoveMap;
	CPoint m_ptRDown;
	CPoint m_ptRDownMap;
	CPoint m_ptMap;
	CPoint m_ptLast;
	UINT m_curMapId;
	TileMap m_map;

	// 当前鼠标所在的格子坐标
	CPoint mMouseCell;

	// 移动怪物
	CPoint m_ptLDown;

	CPoint m_ptLDownMon;
	BOOL m_isMoveMon;

	CPoint m_ptLDownNPC;
	BOOL m_isMoveNPC;

	CPoint m_ptLDownJumpPoint;
	BOOL m_isMoveJumpPoint;
	
	// 移动小地图
	BOOL m_isMoveMiniMap;

	// npc
	NpcManager m_npcMgr;
	MonsterManager m_mnstMgr;
	JumpPointManager m_jumpPointMgr;

	// minimap
	//BOOL m_isShowMiniMap;
	ITexture* m_miniMap;
	FLOAT m_mapScalX;
	FLOAT m_mapScalY;


	// 对话框
	BOOL m_isShowMainDlg;
	CMainDlg* m_pMainDlg;

	// 阻挡区域
	BOOL m_isViewBlock;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnViewMain();
	afx_msg void OnUpdateViewMain(CCmdUI *pCmdUI);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnViewBlock();
	afx_msg void OnUpdateViewBlock(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};

