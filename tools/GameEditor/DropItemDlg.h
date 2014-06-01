#pragma once
#include "afxcmn.h"


// CDropItemDlg �Ի���

class CDropItemDlg : public CDialog
{
	DECLARE_DYNAMIC(CDropItemDlg)

public:
	CDropItemDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDropItemDlg();

	void SetDropId(UINT dropId){m_dropId = dropId;}

	void AddItem(const stDropItem& dropItem);

// �Ի�������
	enum { IDD = IDD_DLG_DROPITEMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ��Ʒ�б�
	UINT m_dropId;
	CListCtrl m_itemList;
	virtual BOOL OnInitDialog();
};
