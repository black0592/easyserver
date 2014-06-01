#pragma once
#include "afxwin.h"


// CCreateNpcDlg 对话框

class CCreateJumpPointDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateJumpPointDlg)

public:
	CCreateJumpPointDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateJumpPointDlg();

// 对话框数据
	enum { IDD = IDD_DLG_NEW_JUMPPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();

public:
	void EditJumpPoint(JumpPoint* pObj);
	void InitEditModeControls();

private:
	BOOL m_isEditMode;	// 是否是修改方式
	JumpPoint* m_pJumpPointEdit;	// 需要被修改的对象

private:
	// 目标地图
	CComboBox m_cmbTargetMap;
	// 目标X坐标
	DWORD m_targetMapX;
	// 目标Y坐标
	DWORD m_targetMapY;
	// 出生区域的X
	CString m_scriptFile;

	static JumpPointInfo m_lastInfo;
};
