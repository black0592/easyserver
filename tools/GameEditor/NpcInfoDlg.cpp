// NpcInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GameEditor.h"
#include "NpcInfoDlg.h"
#include "NpcManager.h"
#include "CreateNpcDlg.h"


// NpcInfoDlg 对话框

IMPLEMENT_DYNAMIC(NpcInfoDlg, CDialog)
NpcInfoDlg::NpcInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NpcInfoDlg::IDD, pParent)
{
}

NpcInfoDlg::~NpcInfoDlg()
{
}

void NpcInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FUNC, m_txtFunc);
	DDX_Control(pDX, IDC_STATIC_SHOP_NUM, m_txtShopNum);
	DDX_Control(pDX, IDC_LIST_NPC, m_npcList);
}


BEGIN_MESSAGE_MAP(NpcInfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_NPC, OnLvnItemchangedListNpc)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, OnHdnItemdblclickListNpc)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_NPC, OnNMDblclkListNpc)
	ON_BN_CLICKED(IDC_BTN_CREATENPC, OnBnClickedBtnCreateNpc)
	ON_BN_CLICKED(IDC_BTN_DEL_NPC, OnBnClickedBtnDelNpc)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// NpcInfoDlg 消息处理程序

BOOL NpcInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle = m_npcList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_ONECLICKACTIVATE;
	dwStyle |= LVS_EX_SUBITEMIMAGES;
	m_npcList.SetExtendedStyle(dwStyle);

	// TODO:  在此添加额外的初始化
	m_npcList.InsertColumn(0, "序号", LVCFMT_LEFT, 40);
	m_npcList.InsertColumn(1, "名称", LVCFMT_LEFT, 120);
	m_npcList.InsertColumn(3, "功能", LVCFMT_LEFT, 120);
	m_npcList.InsertColumn(4, "商店", LVCFMT_LEFT, 120);
	m_npcList.InsertColumn(5, "脚本", LVCFMT_LEFT, 120);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void NpcInfoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
}

void NpcInfoDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnCancel();
}

void NpcInfoDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CDialog::OnClose();
}


void NpcInfoDlg::SetFunc(UINT func)
{
	//CString strFunc;
	//strFunc.Format("功能：%s", Misc::getNpcFuncStr(func));
	//m_txtFunc.SetWindowText(strFunc);
}

void NpcInfoDlg::SetShopNum(UINT shopNum)
{
	//CString strShopNum;
	//strShopNum.Format("商店编号：%d", shopNum);
	//m_txtShopNum.SetWindowText(strShopNum);
}

void NpcInfoDlg::OnLvnItemchangedListNpc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	POSITION pos = m_npcList.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		int index = m_npcList.GetNextSelectedItem(pos);
		CString strID = m_npcList.GetItemText(index, 0);
		UINT uinId = atoi(strID);
		AfxGetMainWnd()->PostMessage(WM_MONDLG_SELECT_NPC, 0, (LPARAM)uinId);
	}
}

void NpcInfoDlg::OnHdnItemdblclickListNpc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	// 没有用
}

void NpcInfoDlg::OnNMDblclkListNpc(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	UpdateData();

	POSITION pos = m_npcList.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		int index = m_npcList.GetNextSelectedItem(pos);
		CString strID = m_npcList.GetItemText(index, 0);
		UINT uinId = atoi(strID);
		Npc* pNpc = g_pNpcMgr->FindNpc(uinId);
		if (pNpc == NULL)
			return;

		if (false/*m_checkDropItem*/) {
			/*CString strDropID = m_npcList.GetItemText(index, 7);
			UINT dropId = atoi(strDropID);
			CDropItemDlg dlg;
			dlg.SetDropId(dropId);
			dlg.DoModal();*/
		} else {
			CCreateNpcDlg dlg;
			dlg.EditNpc(pNpc);
			dlg.DoModal();
		}
	}
}

void NpcInfoDlg::OnBnClickedBtnCreateNpc()
{
	// TODO: 在此添加控件通知处理程序代码
	CCreateNpcDlg dlg;
	dlg.DoModal();
}


void NpcInfoDlg::OnBnClickedBtnSave()
{
	//// TODO: 在此添加控件通知处理程序代码
	if ( !g_pNpcMgr->SaveToFile() ) {
		AfxMessageBox("保存失败!");
	} else {
		AfxMessageBox("保存完毕!");
	}
}




void NpcInfoDlg::OnBnClickedBtnDelNpc()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_npcList.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		int index = m_npcList.GetNextSelectedItem(pos);
		CString strID = m_npcList.GetItemText(index, 0);
		UINT uinId = atoi(strID);
		g_pNpcMgr->DeleteNpc(uinId);

		if ( !g_pNpcMgr->SaveToFile() ) {
			AfxMessageBox("保存失败!");
		}
	}
}






