#pragma once
#include "afxcmn.h"


// CMonsterDlg �Ի���

class CMonsterDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonsterDlg)

public:
	CMonsterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMonsterDlg();

// �Ի�������
	enum { IDD = IDD_DLG_MONSTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	// �Ƿ�鿴����
	BOOL m_checkDropItem;
};
