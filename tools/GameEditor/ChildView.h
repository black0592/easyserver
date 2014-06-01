// ChildView.h : CChildView ��Ľӿ�
//


#pragma once

#include "MainDlg.h"
#include "NpcManager.h"
#include "MonsterManager.h"
#include "JumpPointManager.h"
#include "MiniMapData.h"
#include "TileMap.h"

// CChildView ����

class CChildView : public CWnd, public IEngine::IEventListener
{
// ����
public:
	CChildView();

// ����
public:

// ����
public:

// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	void SetMapPt(int x, int y);
	CPoint GetMapPt() { return m_ptMap; }

	// ���ص�ͼ
	BOOL LoadMap(UINT mapId);

	void DrawMap();
	void DrawMiniMap();
	void DrawPostionInfo();

	// ��������
	BOOL CreateMonster(MONINFO* monInfo);

	// �޸Ĺ�������
	BOOL EditMonster(UINT monUid, MONINFO* monInfo);

	// ѡ�й���
	BOOL SelectMonster(UINT monUid);




	// ����NPC
	BOOL CreateNpc(NPCINFO* npcInfo);

	// �޸�NPC����
	BOOL EditNpc(UINT npcUid, NPCINFO* npcInfo);

	// ѡ��NPC
	BOOL SelectNpc(UINT npcUid);




	// ������ת��
	BOOL CreateJumpPoint(JumpPointInfo* info);

	// �޸���ת������
	BOOL EditJumpPoint(UINT uid, JumpPointInfo* info);

	// ѡ����ת��
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

	// ��ǰ������ڵĸ�������
	CPoint mMouseCell;

	// �ƶ�����
	CPoint m_ptLDown;

	CPoint m_ptLDownMon;
	BOOL m_isMoveMon;

	CPoint m_ptLDownNPC;
	BOOL m_isMoveNPC;

	CPoint m_ptLDownJumpPoint;
	BOOL m_isMoveJumpPoint;
	
	// �ƶ�С��ͼ
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


	// �Ի���
	BOOL m_isShowMainDlg;
	CMainDlg* m_pMainDlg;

	// �赲����
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

