// CreateMonDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameEditor.h"
#include "CreateMonDlg.h"
#include "MonsterManager.h"
#include "DropItemDlg.h"


MONINFO CCreateMonDlg::m_lastInfo;

// CCreateMonDlg �Ի���

IMPLEMENT_DYNAMIC(CCreateMonDlg, CDialog)
CCreateMonDlg::CCreateMonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateMonDlg::IDD, pParent)
	, m_monNum(1)
	, m_monAI(0)
	, m_mvSpeed(100)
	, m_monView(3)
	, m_monDropId(0)
	, m_monDropProb(0)
	, m_monReviveTime(1)
	, m_birthX(0)
	, m_birthY(0)
	, m_birthW(2)
	, m_birthH(4)
	, m_isBossDrop(FALSE)
{
	m_isEditMode = FALSE;
	m_pMonEdit = NULL;
}

CCreateMonDlg::~CCreateMonDlg()
{
}

void CCreateMonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_RACE, m_cmbMonClass);
	DDX_Text(pDX, IDC_EDIT_NUM, m_monNum);
	DDX_Text(pDX, IDC_EDIT_AI, m_monAI);
	DDX_Text(pDX, IDC_EDIT_MVSPEED, m_mvSpeed);
	DDX_Text(pDX, IDC_EDIT_VIEW, m_monView);
	DDX_Text(pDX, IDC_EDIT_DROPID, m_monDropId);
	DDX_Text(pDX, IDC_EDIT_DROPPROB, m_monDropProb);
	DDX_Text(pDX, IDC_EDIT_REVIVE_TIME, m_monReviveTime);
	DDX_Text(pDX, IDC_EDIT_X1, m_birthW);
	DDX_Text(pDX, IDC_EDIT_Y1, m_birthH);
	DDX_Check(pDX, IDC_CHECK_BOSS_DROP, m_isBossDrop);
	DDX_Control(pDX, IDC_EDIT_DROP_NAME, m_dropName);
}


BEGIN_MESSAGE_MAP(CCreateMonDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_DROPLIST, OnBnClickedBtnDroplist)
	ON_EN_CHANGE(IDC_EDIT_DROPID, &CCreateMonDlg::OnEnChangeEditDropid)
END_MESSAGE_MAP()


// CCreateMonDlg ��Ϣ�������

void CCreateMonDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	// ����������
	UpdateData();

	if (m_birthW < 1 || m_birthH < 1) {
		AfxMessageBox("�����������ݴ���!");
		return;
	}

	int index = m_cmbMonClass.GetCurSel();
	stMonBase* base = (stMonBase*)m_cmbMonClass.GetItemData(index);
	if (base == NULL)
		return;

	MONINFO* pInfo = new MONINFO;

	pInfo->race = base->id;					// ����
	pInfo->num = m_monNum;					// ����
	pInfo->mapId = 0;						// ��ͼ���	(����һ�����̵Ĵ���������)
	pInfo->ai = m_monAI;					// ai���
	pInfo->speed = m_mvSpeed;				// �ƶ��ٶ�
	pInfo->view = m_monView;				// ��Ұ
	pInfo->dropInfo[0].dropId = m_monDropId;			// ������
	pInfo->dropInfo[0].dropProb = m_monDropProb;		// �������(��ֱ�)
	pInfo->dropInfo[0].dropType = m_isBossDrop ? 1 : 0;	// ��������(0 - ��ѡһ, 1 - ȫ���䣬����80%����)
	pInfo->reviveTime = m_monReviveTime;	// ����ʱ��(����)
	pInfo->birthRect.cx = m_birthW;				// ��������
	pInfo->birthRect.cy = m_birthH;

	// ����
	m_lastInfo = *pInfo;

	if (m_isEditMode) {
		AfxGetMainWnd()->PostMessage(WM_NEWMONDLG_EDIT_MONSTER, (WPARAM)m_pMonEdit->GetID(), (LPARAM)pInfo);
	} else {
		AfxGetMainWnd()->PostMessage(WM_NEWMONDLG_CREATE_MONSTER, 0, (LPARAM)pInfo);
	}

	OnOK();
}

void CCreateMonDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CCreateMonDlg::OnBnClickedBtnDroplist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	CDropItemDlg dlg;
	dlg.SetDropId(m_monDropId);
	dlg.DoModal();
}

BOOL CCreateMonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	vector<stMonBase*> list;
	GameTable::getAllMonBase(list);

	for (size_t i=0; i<list.size(); i++) {
		const stMonBase* obj = list[i];
		CString strMonName;
		strMonName.Format("[%d]%s(%d��)", obj->id, Misc::utf8ToGbk(obj->name).c_str(), obj->level);
		m_cmbMonClass.AddString(strMonName);
		int index = m_cmbMonClass.GetCount();
		m_cmbMonClass.SetItemData(index-1, (DWORD_PTR)obj);
	}
	m_cmbMonClass.SetCurSel(0);

	InitLastMonsterControls();
	InitEditModeControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CCreateMonDlg::InitLastMonsterControls()
{
	if (m_lastInfo.race == 0)
		return;

	MONINFO& info = m_lastInfo;

	for (int i=0; i<m_cmbMonClass.GetCount(); ++i) {
		stMonBase* base2 = (stMonBase*)m_cmbMonClass.GetItemData(i);
		if (base2 == NULL) continue;
		if (base2->id == m_lastInfo.race) {
			m_cmbMonClass.SetCurSel(i);
			break;
		}
	}

	m_monNum = info.num;
	m_monAI = info.ai;
	m_mvSpeed = info.speed;
	m_monView = info.view;
	m_isBossDrop = info.dropInfo[0].dropType;
	m_monDropId = info.dropInfo[0].dropId;
	m_monDropProb = info.dropInfo[0].dropProb;
	m_monReviveTime = info.reviveTime;
	m_birthW = info.birthRect.getWidth();
	m_birthH = info.birthRect.getHeight();

	UpdateData(FALSE);

	OnEnChangeEditDropid();
}

void CCreateMonDlg::InitEditModeControls()
{
	if (!m_isEditMode)
		return;

	SetWindowText("�޸Ĺ���");

	Monster& mon = *m_pMonEdit;
	MONINFO& info = mon.GetMonInfo();

	for (int i=0; i<m_cmbMonClass.GetCount(); ++i) {
		stMonBase* base2 = (stMonBase*)m_cmbMonClass.GetItemData(i);
		if (base2 == NULL) continue;
		if (base2->id == mon.GetBaseID()) {
			m_cmbMonClass.SetCurSel(i);
			break;
		}
	}

	m_monNum = info.num;
	m_monAI = info.ai;
	m_mvSpeed = info.speed;
	m_monView = info.view;
	m_isBossDrop = info.dropInfo[0].dropType;
	m_monDropId = info.dropInfo[0].dropId;
	m_monDropProb = info.dropInfo[0].dropProb;
	m_monReviveTime = info.reviveTime;
	m_birthW = info.birthRect.getWidth();
	m_birthH = info.birthRect.getHeight();

	UpdateData(FALSE);

	OnEnChangeEditDropid();
}

void CCreateMonDlg::EditMonster(Monster* pMon)
{
	m_isEditMode = TRUE;
	m_pMonEdit = pMon;
}




void CCreateMonDlg::OnEnChangeEditDropid()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);



	CString strStoreName = "��ע��(null)";

	const DropItem* pInfo = GameTable::getDropItem( m_monDropId );
	if (pInfo) {
		strStoreName.Format("��ע��%s", Misc::utf8ToGbk(pInfo->pDesc).c_str());
	}

	m_dropName.SetWindowText(strStoreName);
}
