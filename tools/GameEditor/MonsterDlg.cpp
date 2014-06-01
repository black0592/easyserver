// MonsterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GameEditor.h"
#include "MonsterDlg.h"
#include ".\monsterdlg.h"
#include "CreateMonDlg.h"
#include "DropItemDlg.h"

// CMonsterDlg 对话框

IMPLEMENT_DYNAMIC(CMonsterDlg, CDialog)
CMonsterDlg::CMonsterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterDlg::IDD, pParent)
	, m_checkDropItem(FALSE)
{
}

CMonsterDlg::~CMonsterDlg()
{
}

void CMonsterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MONSTER, m_monList);
	DDX_Check(pDX, IDC_CHECK_DROP, m_checkDropItem);
}


BEGIN_MESSAGE_MAP(CMonsterDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedBtnSave)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MONSTER, OnLvnItemchangedListMonster)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, OnHdnItemdblclickListMonster)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MONSTER, OnNMDblclkListMonster)
	ON_BN_CLICKED(IDC_BTN_CREATEMON, OnBnClickedBtnCreatemon)
	ON_BN_CLICKED(IDC_BTN_DEL_MONSTER, OnBnClickedBtnDelMonster)
END_MESSAGE_MAP()


// CMonsterDlg 消息处理程序


BOOL CMonsterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle = m_monList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_ONECLICKACTIVATE;
	dwStyle |= LVS_EX_SUBITEMIMAGES;
	m_monList.SetExtendedStyle(dwStyle);

	// TODO:  在此添加额外的初始化
	m_monList.InsertColumn(0, "序号", LVCFMT_LEFT, 40);
	m_monList.InsertColumn(1, "名称", LVCFMT_LEFT, 150);
	m_monList.InsertColumn(2, "数量", LVCFMT_LEFT, 40);
	m_monList.InsertColumn(3, "智能", LVCFMT_LEFT, 40);
	m_monList.InsertColumn(4, "移速", LVCFMT_LEFT, 40);
	m_monList.InsertColumn(5, "视野", LVCFMT_LEFT, 40);
	m_monList.InsertColumn(6, "BOSS掉落", LVCFMT_LEFT, 60);
	m_monList.InsertColumn(7, "掉落编号", LVCFMT_LEFT, 60);
	m_monList.InsertColumn(8, "掉落概率", LVCFMT_LEFT, 60);
	m_monList.InsertColumn(9, "复活时间", LVCFMT_LEFT, 60);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMonsterDlg::OnLvnItemchangedListMonster(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	
	POSITION pos = m_monList.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		int index = m_monList.GetNextSelectedItem(pos);
		CString strID = m_monList.GetItemText(index, 0);
		UINT uinId = atoi(strID);
		AfxGetMainWnd()->PostMessage(WM_MONDLG_SELECT_MONSTER, 0, (LPARAM)uinId);
	}
}

void CMonsterDlg::OnHdnItemdblclickListMonster(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	
	// 没有用
}

void CMonsterDlg::OnNMDblclkListMonster(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	UpdateData();

	POSITION pos = m_monList.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		int index = m_monList.GetNextSelectedItem(pos);
		CString strID = m_monList.GetItemText(index, 0);
		UINT uinId = atoi(strID);
		Monster* pMon = g_pMonMgr->FindMonster(uinId);
		if (pMon == NULL)
			return;

		if (m_checkDropItem) {
			CString strDropID = m_monList.GetItemText(index, 7);
			UINT dropId = atoi(strDropID);
			CDropItemDlg dlg;
			dlg.SetDropId(dropId);
			dlg.DoModal();
		} else {
			CCreateMonDlg dlg;
			dlg.EditMonster(pMon);
			dlg.DoModal();
		}
	}
}

void CMonsterDlg::OnBnClickedBtnCreatemon()
{
	// TODO: 在此添加控件通知处理程序代码
	CCreateMonDlg dlg;
	dlg.DoModal();
}


void CMonsterDlg::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( !g_pMonMgr->SaveToFile() ) {
		AfxMessageBox("保存失败!");
	} else {
		AfxMessageBox("保存完毕!");
	}
}




void CMonsterDlg::OnBnClickedBtnDelMonster()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_monList.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		int index = m_monList.GetNextSelectedItem(pos);
		CString strID = m_monList.GetItemText(index, 0);
		UINT uinId = atoi(strID);
		g_pMonMgr->DeleteMonster(uinId);

		if ( !g_pMonMgr->SaveToFile() ) {
			AfxMessageBox("保存失败!");
		}
	}
}



