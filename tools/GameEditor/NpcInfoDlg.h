#pragma once
#include "afxwin.h"


// NpcInfoDlg 对话框

class NpcInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(NpcInfoDlg)

public:
	NpcInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NpcInfoDlg();

	// 设置NPC的功能
	void SetFunc(UINT func);

	// 设置商店编号
	void SetShopNum(UINT shopNum);

// 对话框数据
	enum { IDD = IDD_DLG_NPCINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnLvnItemchangedListNpc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemdblclickListNpc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListNpc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnCreateNpc();
	afx_msg void OnBnClickedBtnDelNpc();
	CStatic m_txtFunc;
	CStatic m_txtShopNum;
	CListCtrl m_npcList;
};
