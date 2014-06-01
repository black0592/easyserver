// JumpPointInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameEditor.h"
#include "JumpPointInfoDlg.h"
#include "JumpPointManager.h"
#include "CreateJumpPointDlg.h"


// JumpPointInfoDlg �Ի���

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


// JumpPointInfoDlg ��Ϣ�������

void JumpPointInfoDlg::OnBnClickedBtnCreateJumppoint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCreateJumpPointDlg dlg;
	dlg.DoModal();
}

void JumpPointInfoDlg::OnBnClickedBtnDelJumppoint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_jumpPointList.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		int index = m_jumpPointList.GetNextSelectedItem(pos);
		CString strID = m_jumpPointList.GetItemText(index, 0);
		UINT uinId = atoi(strID);
		g_pJumpPointMgr->DeleteJumpPoint(uinId);

		if ( !g_pJumpPointMgr->SaveToFile() ) {
			AfxMessageBox("����ʧ��!");
		}
	}
}

BOOL JumpPointInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	DWORD dwStyle = m_jumpPointList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_ONECLICKACTIVATE;
	dwStyle |= LVS_EX_SUBITEMIMAGES;
	m_jumpPointList.SetExtendedStyle(dwStyle);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_jumpPointList.InsertColumn(0, "���", LVCFMT_LEFT, 40);
	m_jumpPointList.InsertColumn(1, "Ŀ���ͼ", LVCFMT_LEFT, 200);
	m_jumpPointList.InsertColumn(2, "Ŀ��X����", LVCFMT_LEFT, 100);
	m_jumpPointList.InsertColumn(3, "Ŀ��Y����", LVCFMT_LEFT, 100);
	m_jumpPointList.InsertColumn(4, "�ű�", LVCFMT_LEFT, 120);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void JumpPointInfoDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	// û����
}

void JumpPointInfoDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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