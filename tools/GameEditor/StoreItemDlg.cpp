// StoreItemDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameEditor.h"
#include "StoreItemDlg.h"


// StoreItemDlg �Ի���

IMPLEMENT_DYNAMIC(CStoreItemDlg, CDialog)

CStoreItemDlg::CStoreItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStoreItemDlg::IDD, pParent)
{
	m_storeId = 0xFFFFFFFF;
}

CStoreItemDlg::~CStoreItemDlg()
{
}

void CStoreItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ITEM, m_itemList);
}


BEGIN_MESSAGE_MAP(CStoreItemDlg, CDialog)
END_MESSAGE_MAP()


// StoreItemDlg ��Ϣ�������

BOOL CStoreItemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle = m_itemList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_ONECLICKACTIVATE;
	dwStyle |= LVS_EX_SUBITEMIMAGES;
	m_itemList.SetExtendedStyle(dwStyle);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_itemList.InsertColumn(0, "���", LVCFMT_LEFT, 40);
	m_itemList.InsertColumn(1, "����", LVCFMT_LEFT, 120);
	m_itemList.InsertColumn(2, "�۸�", LVCFMT_LEFT, 120);

	if (m_storeId != 0xFFFFFFFF) {
		const ShopInfo* pShopInfo = GameTable::getShopInfo( m_storeId );
		if (NULL == pShopInfo)
			return TRUE;

		for (size_t i=0; i<pShopInfo->goods.size(); ++i) {
			AddItem(pShopInfo->goods[i].iItemId, pShopInfo->goods[i].iPrice);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CStoreItemDlg::AddItem(UINT itemId, UINT price)
{
	const stItemBase* base = GameTable::getItemBase(itemId);
	if (base == NULL) return;

	const 

	int index = m_itemList.GetItemCount();
	CString strTmp;
	strTmp.Format("%d", base->id);
	m_itemList.InsertItem(index, strTmp);			// ���

	strTmp.Format("%s", Misc::utf8ToGbk(base->name).c_str());
	m_itemList.SetItemText(index, 1, strTmp);		// ����

	strTmp.Format("%d", price);
	m_itemList.SetItemText(index, 2, strTmp);		// �۸�
}
