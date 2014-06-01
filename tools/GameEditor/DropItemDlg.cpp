// DropItemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GameEditor.h"
#include "DropItemDlg.h"
#include ".\dropitemdlg.h"


// CDropItemDlg 对话框

IMPLEMENT_DYNAMIC(CDropItemDlg, CDialog)
CDropItemDlg::CDropItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDropItemDlg::IDD, pParent)
{
	m_dropId = 0xFFFFFFFF;
}

CDropItemDlg::~CDropItemDlg()
{
}

void CDropItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ITEM, m_itemList);
}


BEGIN_MESSAGE_MAP(CDropItemDlg, CDialog)
END_MESSAGE_MAP()


// CDropItemDlg 消息处理程序

BOOL CDropItemDlg::OnInitDialog()
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
	m_itemList.InsertColumn(2, "概率", LVCFMT_LEFT, 120);

	if (m_dropId != 0xFFFFFFFF) {
		DropItemList* list = DropItemManager::Instance()->getDropList(m_dropId);
		if (list) {
			vector<stDropItem> items;
			list->getItems(items);
			for (size_t i=0; i<items.size(); ++i) {
				AddItem(items[i]);
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDropItemDlg::AddItem(const stDropItem& dropItem)
{
	const stItemBase* base = GameTable::getItemBase(dropItem.itemId);
	if (base == NULL) return;

	int index = m_itemList.GetItemCount();
	CString strTmp;
	strTmp.Format("%d", base->id);
	m_itemList.InsertItem(index, strTmp);			// 序号

	strTmp.Format("%s", Misc::utf8ToGbk(base->name).c_str());
	m_itemList.SetItemText(index, 1, strTmp);		// 名称

	strTmp.Format("%d", dropItem.prob);
	m_itemList.SetItemText(index, 2, strTmp);		// 概率
}


