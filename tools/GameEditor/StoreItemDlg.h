#pragma once


// CStoreItemDlg �Ի���

class CStoreItemDlg : public CDialog
{
	DECLARE_DYNAMIC(CStoreItemDlg)

public:
	CStoreItemDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStoreItemDlg();

	void SetStoreId(UINT storeId){m_storeId = storeId;}

	void AddItem(UINT itemId, UINT price);

// �Ի�������
	enum { IDD = IDD_DLG_STOREITEMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ��Ʒ�б�
	UINT m_storeId;
	CListCtrl m_itemList;
	virtual BOOL OnInitDialog();
};
