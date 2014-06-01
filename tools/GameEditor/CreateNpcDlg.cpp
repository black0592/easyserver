// CreateNpcDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameEditor.h"
#include "CreateNpcDlg.h"
#include "StoreItemDlg.h"

NPCINFO CCreateNpcDlg::m_lastInfo;

// CCreateNpcDlg �Ի���

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


// CCreateNpcDlg ��Ϣ�������


void CCreateNpcDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ����������
	UpdateData();

	int index = m_cmbNpcClass.GetCurSel();
	stNpcBase* base = (stNpcBase*)m_cmbNpcClass.GetItemData(index);
	if (base == NULL)
		return;

	NPCINFO* pInfo = new NPCINFO;

	pInfo->baseId = base->id;	// npc��ò
	pInfo->npcFunc = m_cmbNpcFunc.GetCurSel();	// npc���� (ENpcFunc)
	pInfo->storeId = m_storeId;	// �̵���
	pInfo->scriptFile  = m_scriptFile;

	// ����
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CCreateNpcDlg::OnBnClickedBtnStorelist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	CStoreItemDlg dlg;
	dlg.SetStoreId(m_storeId);
	dlg.DoModal();
}

BOOL CCreateNpcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �г�npc
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

	// �г�npc����
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
	// �쳣: OCX ����ҳӦ���� FALSE
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

	SetWindowText("�޸�NPC");

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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	

	CString strStoreName = "��ע��(null)";

	const ShopInfo* pShopInfo = GameTable::getShopInfo( m_storeId );
	if (pShopInfo) {
		strStoreName.Format("��ע��%s", Misc::utf8ToGbk(pShopInfo->pDesc).c_str());
	}

	m_storeName.SetWindowText(strStoreName);
}
