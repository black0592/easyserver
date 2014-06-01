#pragma once
#include "afxcmn.h"


// CMonsterDlg 对话框

class CMonsterDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonsterDlg)

public:
	CMonsterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMonsterDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MONSTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSave();
	CListCtrl m_monList;
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedListMonster(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemdblclickListMonster(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListMonster(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnCreatemon();
	afx_msg void OnBnClickedBtnDelMonster();

	BOOL isCheckDropItem() { return m_checkDropItem; }

	// 是否查看掉落
	BOOL m_checkDropItem;
};
