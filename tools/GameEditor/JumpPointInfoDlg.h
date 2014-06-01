#pragma once
#include "afxcmn.h"


// JumpPointInfoDlg 对话框

class JumpPointInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(JumpPointInfoDlg)

public:
	JumpPointInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~JumpPointInfoDlg();

// 对话框数据
	enum { IDD = IDD_DLG_JUMPPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCreateJumppoint();
	afx_msg void OnBnClickedBtnDelJumppoint();

	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemdblclickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);

	CListCtrl m_jumpPointList;
	virtual BOOL OnInitDialog();
};
