#pragma once
#include "afxwin.h"


// CCreateMonDlg �Ի���

class CCreateMonDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateMonDlg)

public:
	CCreateMonDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCreateMonDlg();

// �Ի�������
	enum { IDD = IDD_DLG_NEW_MON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnDroplist();
	virtual BOOL OnInitDialog();

public:
	void EditMonster(Monster* pMon);
	void InitEditModeControls();
	void InitLastMonsterControls();

private:
	BOOL m_isEditMode;		// �Ƿ����޸ķ�ʽ
	Monster* m_pMonEdit;	// ��Ҫ���޸ĵĹ���

private:
	// ��������
	CComboBox m_cmbMonClass;
	// ��������
	DWORD m_monNum;
	// ����AI
	DWORD m_monAI;
	// �����ƶ��ٶ�
	DWORD m_mvSpeed;
	// ������Ұ
	DWORD m_monView;
	// ���������
	DWORD m_monDropId;
	// �������(��ֱ�)
	DWORD m_monDropProb;
	// ���︴��ʱ��
	DWORD m_monReviveTime;
	// ���������X
	DWORD m_birthX;
	// ���������Y
	DWORD m_birthY;
	// ��������Ŀ�
	DWORD m_birthW;
	// ��������ĸ�
	DWORD m_birthH;
	// �Ƿ�BOSS��ʽ�ĵ���
	BOOL m_isBossDrop;


	static MONINFO m_lastInfo;
public:
	// ���䱸ע��
	CStatic m_dropName;
	afx_msg void OnEnChangeEditDropid();
};
