// JumpPointInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GameEditor.h"
#include "JumpPointInfoDlg.h"
#include "JumpPointManager.h"
#include "CreateJumpPointDlg.h"


// JumpPointInfoDlg 对话框

IMPLEMENT_DYNAMIC(JumpPointInfoDlg, CDialog)

JumpPointInfoDlg::JumpPointInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(JumpPointInfoDlg::IDD, pParent)
{

}

JumpPointInfoDlg::~JumpPointInfoDlg()
{
}

void JumpPointInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_JUMPPOINT, m_jumpPointList);

}


BEGIN_MESSAGE_MAP(JumpPointInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_CREATE_JUMPPOINT, &JumpPointInfoDlg::OnBnClickedBtnCreateJumppoint)
	ON_BN_CLICKED(IDC_BTN_DEL_JUMPPOINT, &JumpPointInfoDlg::OnBnClickedBtnDelJumppoint)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_JUMPPOINT, OnLvnItemchangedList)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, OnHdnItemdblclickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_JUMPPOINT, OnNMDblclkList)
END_MESSAGE_MAP()


// JumpPointInfoDlg 消息处理程序

void JumpPointInfoDlg::OnBnClickedBtnCreateJumppoint()
{
	// TODO: 在此添加控件通知处理程序代码
	CCreateJumpPointDlg dlg;
	dlg.DoModal();
}

void JumpPointInfoDlg::OnBnClickedBtnDelJumppoint()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_jumpPointList.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		int index = m_jumpPointList.GetNextSelectedItem(pos);
		CString strID = m_jumpPointList.GetItemText(index, 0);
		UINT uinId = atoi(strID);
		g_pJumpPointMgr->DeleteJumpPoint(uinId);

		if ( !g_pJumpPointMgr->SaveToFile() ) {
			AfxMessageBox("保存失败!");
		}
	}
}

BOOL JumpPointInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwStyle = m_jumpPointList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_ONECLICKACTIVATE;
	dwStyle |= LVS_EX_SUBITEMIMAGES;
	m_jumpPointList.SetExtendedStyle(dwStyle);

	// TODO:  在此添加额外的初始化
	m_jumpPointList.InsertColumn(0, "序号", LVCFMT_LEFT, 40);
	m_jumpPointList.InsertColumn(1, "目标地图", LVCFMT_LEFT, 200);
	m_jumpPointList.InsertColumn(2, "目标X坐标", LVCFMT_LEFT, 100);
	m_jumpPointList.InsertColumn(3, "目标Y坐标", LVCFMT_LEFT, 100);
	m_jumpPointList.InsertColumn(4, "脚本", LVCFMT_LEFT, 120);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void JumpPointInfoDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	POSITION pos = m_jumpPointList.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		int index = m_jumpPointList.GetNextSelectedItem(pos);
		CString strID = m_jumpPointList.GetItemText(index, 0);
		UINT uinId = atoi(strID);
		AfxGetMainWnd()->PostMessage(WM_MONDLG_SELECT_JUMPPOINT, 0, (LPARAM)uinId);
	}
}

void JumpPointInfoDlg::OnHdnItemdblclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	// 没有用
}

void JumpPointInfoDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	UpdateData();

	POSITION pos = m_jumpPointList.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		int index = m_jumpPointList.GetNextSelectedItem(pos);
		CString strID = m_jumpPointList.GetItemText(index, 0);
		UINT uinId = atoi(strID);
		JumpPoint* pJumpPoint = g_pJumpPointMgr->FindJumpPoint(uinId);
		if (pJumpPoint == NULL)
			return;

		CCreateJumpPointDlg dlg;
		dlg.EditJumpPoint(pJumpPoint);
		dlg.DoModal();
	}
}