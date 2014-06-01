#pragma once
#include "afxcmn.h"


// CDropItemDlg 对话框

class CDropItemDlg : public CDialog
{
	DECLARE_DYNAMIC(CDropItemDlg)

public:
	CDropItemDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDropItemDlg();

	void SetDropId(UINT dropId){m_dropId = dropId;}

	void AddItem(const stDropItem& dropItem);

// 对话框数据
	enum { IDD = IDD_DLG_DROPITEMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 物品列表
	UINT m_dropId;
	CListCtrl m_itemList;
	virtual BOOL OnInitDialog();
};
