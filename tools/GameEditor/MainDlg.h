#pragma once
#include "afxwin.h"
#include "MonsterDlg.h"
#include "NpcInfoDlg.h"
#include "JumpPointInfoDlg.h"

// CMainDlg �Ի���

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainDlg();

// �Ի�������
	enum { IDD = IDD_DLG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtLoadmap();
	// ��ͼ�б�
	CComboBox m_combMapList;
	virtual BOOL OnInitDialog();
	// ������Ϣ
	CStatic m_posInfo;
	// ����ѡ��
	CComboBox m_cmbBoard;
	afx_msg void OnCbnSelchangeComboBoard();

	BOOL isCheckDropItem() { m_pMonsterDlg->UpdateData(); return m_pMonsterDlg->isCheckDropItem(); }

	
	NpcInfoDlg* m_pNpcInfoDlg;
	CMonsterDlg* m_pMonsterDlg;
	JumpPointInfoDlg* m_pJumpPointDlg;
	afx_msg void OnBnClickedBtnLoaddropitem();
};
