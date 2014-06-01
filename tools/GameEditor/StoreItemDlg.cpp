// StoreItemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GameEditor.h"
#include "StoreItemDlg.h"


// StoreItemDlg 对话框

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


// StoreItemDlg 消息处理程序

BOOL CStoreItemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle = m_itemList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_ONECLICKACTIVATE;
	dwStyle |= LVS_EX_SUBITEMIMAGES;
	m_itemList.SetExtendedStyle(dwStyle);

	// TODO:  在此添加额外的初始化
	m_itemList.InsertColumn(0, "编号", LVCFMT_LEFT, 40);
	m_itemList.InsertColumn(1, "名称", LVCFMT_LEFT, 120);
	m_itemList.InsertColumn(2, "价格", LVCFMT_LEFT, 120);

	if (m_storeId != 0xFFFFFFFF) {
		const ShopInfo* pShopInfo = GameTable::getShopInfo( m_storeId );
		if (NULL == pShopInfo)
			return TRUE;

		for (size_t i=0; i<pShopInfo->goods.size(); ++i) {
			AddItem(pShopInfo->goods[i].iItemId, pShopInfo->goods[i].iPrice);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CStoreItemDlg::AddItem(UINT itemId, UINT price)
{
	const stItemBase* base = GameTable::getItemBase(itemId);
	if (base == NULL) return;

	const 

	int index = m_itemList.GetItemCount();
	CString strTmp;
	strTmp.Format("%d", base->id);
	m_itemList.InsertItem(index, strTmp);			// 序号

	strTmp.Format("%s", Misc::utf8ToGbk(base->name).c_str());
	m_itemList.SetItemText(index, 1, strTmp);		// 名称

	strTmp.Format("%d", price);
	m_itemList.SetItemText(index, 2, strTmp);		// 价格
}
