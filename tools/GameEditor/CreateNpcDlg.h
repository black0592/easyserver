#pragma once
#include "afxwin.h"


// CCreateNpcDlg �Ի���

class CCreateNpcDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateNpcDlg)

public:
	CCreateNpcDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCreateNpcDlg();

// �Ի�������
	enum { IDD = IDD_DLG_NEW_NPC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnStorelist();
	virtual BOOL OnInitDialog();

public:
	void EditNpc(Npc* pNpc);
	void InitEditModeControls();
	void InitLastNpcControls();

private:
	BOOL m_isEditMode;	// �Ƿ����޸ķ�ʽ
	Npc* m_pNpcEdit;	// ��Ҫ���޸ĵ�NPC

private:
	// npc����
	CComboBox m_cmbNpcClass;
	// npc����
	CComboBox m_cmbNpcFunc;
	// ���︴��ʱ��
	DWORD m_storeId;
	// ���������X
	CString m_scriptFile;

	static NPCINFO m_lastInfo;
public:
	CStatic m_storeName;
	afx_msg void OnEnChangeEditStoreId();
};
