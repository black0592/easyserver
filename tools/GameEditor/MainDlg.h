#pragma once
#include "afxwin.h"
#include "MonsterDlg.h"
#include "NpcInfoDlg.h"
#include "JumpPointInfoDlg.h"

// CMainDlg 对话框

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtLoadmap();
	// 地图列表
	CComboBox m_combMapList;
	virtual BOOL OnInitDialog();
	// 坐标信息
	CStatic m_posInfo;
	// 功能选项
	CComboBox m_cmbBoard;
	afx_msg void OnCbnSelchangeComboBoard();

	BOOL isCheckDropItem() { m_pMonsterDlg->UpdateData(); return m_pMonsterDlg->isCheckDropItem(); }

	
	NpcInfoDlg* m_pNpcInfoDlg;
	CMonsterDlg* m_pMonsterDlg;
	JumpPointInfoDlg* m_pJumpPointDlg;
	afx_msg void OnBnClickedBtnLoaddropitem();
};
