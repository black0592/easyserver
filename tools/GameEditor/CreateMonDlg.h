#pragma once
#include "afxwin.h"


// CCreateMonDlg 对话框

class CCreateMonDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateMonDlg)

public:
	CCreateMonDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateMonDlg();

// 对话框数据
	enum { IDD = IDD_DLG_NEW_MON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	BOOL m_isEditMode;		// 是否是修改方式
	Monster* m_pMonEdit;	// 需要被修改的怪物

private:
	// 怪物名称
	CComboBox m_cmbMonClass;
	// 怪物数量
	DWORD m_monNum;
	// 怪物AI
	DWORD m_monAI;
	// 怪物移动速度
	DWORD m_mvSpeed;
	// 怪物视野
	DWORD m_monView;
	// 怪物掉落编号
	DWORD m_monDropId;
	// 掉落概率(万分比)
	DWORD m_monDropProb;
	// 怪物复活时间
	DWORD m_monReviveTime;
	// 出生区域的X
	DWORD m_birthX;
	// 出生区域的Y
	DWORD m_birthY;
	// 出生区域的宽
	DWORD m_birthW;
	// 出生区域的高
	DWORD m_birthH;
	// 是否BOSS方式的掉落
	BOOL m_isBossDrop;


	static MONINFO m_lastInfo;
public:
	// 掉落备注名
	CStatic m_dropName;
	afx_msg void OnEnChangeEditDropid();
};
