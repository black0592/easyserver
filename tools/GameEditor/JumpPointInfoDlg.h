#pragma once
#include "afxcmn.h"


// JumpPointInfoDlg �Ի���

class JumpPointInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(JumpPointInfoDlg)

public:
	JumpPointInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~JumpPointInfoDlg();

// �Ի�������
	enum { IDD = IDD_DLG_JUMPPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
