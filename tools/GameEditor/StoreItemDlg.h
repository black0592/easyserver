#pragma once


// CStoreItemDlg 对话框

class CStoreItemDlg : public CDialog
{
	DECLARE_DYNAMIC(CStoreItemDlg)

public:
	CStoreItemDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStoreItemDlg();

	void SetStoreId(UINT storeId){m_storeId = storeId;}

	void AddItem(UINT itemId, UINT price);

// 对话框数据
	enum { IDD = IDD_DLG_STOREITEMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 物品列表
	UINT m_storeId;
	CListCtrl m_itemList;
	virtual BOOL OnInitDialog();
};
