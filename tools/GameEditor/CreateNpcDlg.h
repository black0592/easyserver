#pragma once
#include "afxwin.h"


// CCreateNpcDlg 对话框

class CCreateNpcDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateNpcDlg)

public:
	CCreateNpcDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateNpcDlg();

// 对话框数据
	enum { IDD = IDD_DLG_NEW_NPC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	BOOL m_isEditMode;	// 是否是修改方式
	Npc* m_pNpcEdit;	// 需要被修改的NPC

private:
	// npc名字
	CComboBox m_cmbNpcClass;
	// npc功能
	CComboBox m_cmbNpcFunc;
	// 怪物复活时间
	DWORD m_storeId;
	// 出生区域的X
	CString m_scriptFile;

	static NPCINFO m_lastInfo;
public:
	CStatic m_storeName;
	afx_msg void OnEnChangeEditStoreId();
};
