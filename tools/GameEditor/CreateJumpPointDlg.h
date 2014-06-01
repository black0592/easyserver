#pragma once
#include "afxwin.h"


// CCreateNpcDlg �Ի���

class CCreateJumpPointDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateJumpPointDlg)

public:
	CCreateJumpPointDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCreateJumpPointDlg();

// �Ի�������
	enum { IDD = IDD_DLG_NEW_JUMPPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();

public:
	void EditJumpPoint(JumpPoint* pObj);
	void InitEditModeControls();

private:
	BOOL m_isEditMode;	// �Ƿ����޸ķ�ʽ
	JumpPoint* m_pJumpPointEdit;	// ��Ҫ���޸ĵĶ���

private:
	// Ŀ���ͼ
	CComboBox m_cmbTargetMap;
	// Ŀ��X����
	DWORD m_targetMapX;
	// Ŀ��Y����
	DWORD m_targetMapY;
	// ���������X
	CString m_scriptFile;

	static JumpPointInfo m_lastInfo;
};
