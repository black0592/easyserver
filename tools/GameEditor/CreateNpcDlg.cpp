// CreateNpcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GameEditor.h"
#include "CreateNpcDlg.h"
#include "StoreItemDlg.h"

NPCINFO CCreateNpcDlg::m_lastInfo;

// CCreateNpcDlg 对话框

IMPLEMENT_DYNAMIC(CCreateNpcDlg, CDialog)

CCreateNpcDlg::CCreateNpcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateNpcDlg::IDD, pParent)
	, m_storeId(0)
{
	m_isEditMode = FALSE;
	m_pNpcEdit = NULL;
}

CCreateNpcDlg::~CCreateNpcDlg()
{
}

void CCreateNpcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_RACE, m_cmbNpcClass);
	DDX_Control(pDX, IDC_COMBO_FUNC, m_cmbNpcFunc);
	DDX_Text(pDX, IDC_EDIT_STORE_ID, m_storeId);
	DDX_Text(pDX, IDC_EDIT_SCRIPT_FILE, m_scriptFile);
	DDX_Control(pDX, IDC_EDIT_STORE_NAME, m_storeName);
}


BEGIN_MESSAGE_MAP(CCreateNpcDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_STORE_LIST, OnBnClickedBtnStorelist)
	ON_EN_CHANGE(IDC_EDIT_STORE_ID, &CCreateNpcDlg::OnEnChangeEditStoreId)
END_MESSAGE_MAP()


// CCreateNpcDlg 消息处理程序


void CCreateNpcDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检查出生区域
	UpdateData();

	int index = m_cmbNpcClass.GetCurSel();
	stNpcBase* base = (stNpcBase*)m_cmbNpcClass.GetItemData(index);
	if (base == NULL)
		return;

	NPCINFO* pInfo = new NPCINFO;

	pInfo->baseId = base->id;	// npc外貌
	pInfo->npcFunc = m_cmbNpcFunc.GetCurSel();	// npc功能 (ENpcFunc)
	pInfo->storeId = m_storeId;	// 商店编号
	pInfo->scriptFile  = m_scriptFile;

	// 保存
	m_lastInfo = *pInfo;

	if (m_isEditMode) {
		AfxGetMainWnd()->PostMessage(WM_NEWMONDLG_EDIT_NPC, (WPARAM)m_pNpcEdit->GetID(), (LPARAM)pInfo);
	} else {
		AfxGetMainWnd()->PostMessage(WM_NEWMONDLG_CREATE_NPC, 0, (LPARAM)pInfo);
	}

	OnOK();
}

void CCreateNpcDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CCreateNpcDlg::OnBnClickedBtnStorelist()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CStoreItemDlg dlg;
	dlg.SetStoreId(m_storeId);
	dlg.DoModal();
}

BOOL CCreateNpcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 列出npc
	vector<stNpcBase*> list;
	GameTable::getAllNpcBase(list);
	for (size_t i=0; i<list.size(); i++) {
		const stNpcBase* obj = list[i];
		CString strMonName;
		strMonName.Format("[%d]%s", obj->id, Misc::utf8ToGbk(obj->name).c_str());
		m_cmbNpcClass.AddString(strMonName);
		int index = m_cmbNpcClass.GetCount();
		m_cmbNpcClass.SetItemData(index-1, (DWORD_PTR)obj);
	}
	m_cmbNpcClass.SetCurSel(0);

	// 列出npc功能
	for (UINT i=0; i<=NPC_NONE; i++) {
		CString strFunc = Misc::getNpcFuncStr(i);
		if (strFunc.GetLength() == 0)
			continue;

		m_cmbNpcFunc.AddString(strFunc);
		int index = m_cmbNpcClass.GetCount();
		m_cmbNpcFunc.SetItemData(index-1, (DWORD_PTR)i);
	}
	m_cmbNpcFunc.SetCurSel(0);

	InitLastNpcControls();
	InitEditModeControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCreateNpcDlg::InitLastNpcControls()
{
	if (m_lastInfo.baseId == 0)
		return;

	NPCINFO& info = m_lastInfo;

	for (int i=0; i<m_cmbNpcClass.GetCount(); ++i) {
		stMonBase* base2 = (stMonBase*)m_cmbNpcClass.GetItemData(i);
		if (base2 == NULL) continue;
		if (base2->id == m_lastInfo.baseId) {
			m_cmbNpcClass.SetCurSel(i);
			break;
		}
	}

	m_cmbNpcFunc.SetCurSel(info.npcFunc);
	m_storeId = info.storeId;
	m_scriptFile = "test";

	UpdateData(FALSE);

	OnEnChangeEditStoreId();
}

void CCreateNpcDlg::InitEditModeControls()
{
	if (!m_isEditMode)
		return;

	SetWindowText("修改NPC");

	Npc& npc = *m_pNpcEdit;

	const stNpcBase* base = GameTable::getNpcBase(npc.GetBaseID());
	if (base == NULL) {
		return;
	}

	NPCINFO& info = npc.GetNpcInfo();

	for (int i=0; i<m_cmbNpcClass.GetCount(); ++i) {
		stNpcBase* base2 = (stNpcBase*)m_cmbNpcClass.GetItemData(i);
		if (base2 == NULL) continue;
		if (base2->id == base->id) {
			m_cmbNpcClass.SetCurSel(i);
			break;
		}
	}

	m_cmbNpcFunc.SetCurSel(info.npcFunc);
	m_storeId = info.storeId;
	m_scriptFile = info.scriptFile.c_str();

	UpdateData(FALSE);

	OnEnChangeEditStoreId();
}

void CCreateNpcDlg::EditNpc(Npc* pNpc)
{
	m_isEditMode = TRUE;
	m_pNpcEdit = pNpc;
}

void CCreateNpcDlg::OnEnChangeEditStoreId()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	

	CString strStoreName = "备注：(null)";

	const ShopInfo* pShopInfo = GameTable::getShopInfo( m_storeId );
	if (pShopInfo) {
		strStoreName.Format("备注：%s", Misc::utf8ToGbk(pShopInfo->pDesc).c_str());
	}

	m_storeName.SetWindowText(strStoreName);
}
