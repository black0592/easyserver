#pragma once
#include "afxwin.h"


// NpcInfoDlg �Ի���

class NpcInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(NpcInfoDlg)

public:
	NpcInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~NpcInfoDlg();

	// ����NPC�Ĺ���
	void SetFunc(UINT func);

	// �����̵���
	void SetShopNum(UINT shopNum);

// �Ի�������
	enum { IDD = IDD_DLG_NPCINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
