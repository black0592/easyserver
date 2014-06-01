// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameEditor.h"
#include "MainDlg.h"
#include ".\maindlg.h"
#include "ChildView.h"

// CMainDlg �Ի���

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
}

CMainDlg::~CMainDlg()
{
	delete m_pMonsterDlg;
	delete m_pNpcInfoDlg;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MAPLIST, m_combMapList);
	DDX_Control(pDX, IDC_STATIC_POS_INFO, m_posInfo);
	DDX_Control(pDX, IDC_COMBO_BOARD, m_cmbBoard);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BT_LOADMAP, OnBnClickedBtLoadmap)
	ON_CBN_SELCHANGE(IDC_COMBO_BOARD, OnCbnSelchangeComboBoard)
	ON_BN_CLICKED(IDC_BTN_LOADDROPITEM, OnBnClickedBtnLoaddropitem)
END_MESSAGE_MAP()


// CMainDlg ��Ϣ�������

void CMainDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnOK();
}

void CMainDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnCancel();
}

void CMainDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CDialog::OnClose();
}

void CMainDlg::OnBnClickedBtLoadmap()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int curSel = m_combMapList.GetCurSel();
	CString strMapName;
	m_combMapList.GetWindowText(strMapName);
	strMapName = strMapName.Right( strMapName.GetLength() - 1 );
	int pos = strMapName.Find(']');
	CString strMapId = strMapName.Left(pos);
	AfxGetMainWnd()->PostMessage(WM_MAINDLG_LOADMAP, 0, atoi(strMapId));
}

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	vector<stMapInfo*> list;
	GameTable::getAllMapInfo(list);
	for (size_t i=0; i<list.size(); i++) {
		stMapInfo* info = list[i];
		CString strMapName;
		const char* countryName = Misc::countrName(info->country);
		strMapName.Format("[%d]%s-%s", info->id, countryName, Misc::utf8ToGbk(info->name).c_str());
		m_combMapList.AddString(strMapName);
	}

	m_combMapList.SetCurSel(0);

	// ���ù���ѡ��
	m_cmbBoard.AddString("�������");
	m_cmbBoard.AddString("NPC���");
	m_cmbBoard.AddString("���͵����");
	

	CRect rt;
	// �������
	m_pMonsterDlg = new CMonsterDlg;
	m_pMonsterDlg->Create(IDD_DLG_MONSTER, this);
	m_pMonsterDlg->GetWindowRect(&rt);
	m_pMonsterDlg->SetWindowPos(NULL, 10, 100, rt.Width(), rt.Height(), 0);
	m_pMonsterDlg->ShowWindow(SW_HIDE);
	
	// NPC���
	m_pNpcInfoDlg = new NpcInfoDlg;
	m_pNpcInfoDlg->Create(IDD_DLG_NPCINFO, this);
	m_pNpcInfoDlg->GetWindowRect(&rt);
	m_pNpcInfoDlg->SetWindowPos(NULL, 10, 100, rt.Width(), rt.Height(), 0);
	m_pNpcInfoDlg->ShowWindow(SW_HIDE);

	// ��ת�����
	m_pJumpPointDlg = new JumpPointInfoDlg;
	m_pJumpPointDlg->Create(IDD_DLG_JUMPPOINT, this);
	m_pJumpPointDlg->GetWindowRect(&rt);
	m_pJumpPointDlg->SetWindowPos(NULL, 10, 100, rt.Width(), rt.Height(), 0);
	m_pJumpPointDlg->ShowWindow(SW_HIDE);


	// Ĭ�Ͽ����������
	m_cmbBoard.SetCurSel(0);
	m_pNpcInfoDlg->ShowWindow(SW_HIDE);
	m_pMonsterDlg->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CMainDlg::OnCbnSelchangeComboBoard()
{
	m_pNpcInfoDlg->ShowWindow(SW_HIDE);
	m_pMonsterDlg->ShowWindow(SW_HIDE);
	m_pJumpPointDlg->ShowWindow(SW_HIDE);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UINT m_curSel = m_cmbBoard.GetCurSel();
	switch(m_curSel)
	{
	case 0:
		// �������
		m_pMonsterDlg->ShowWindow(SW_SHOW);
		break;
	case 1:
		// NPC���
		m_pNpcInfoDlg->ShowWindow(SW_SHOW);
	    break;
	case 2:
		// ���͵�
		m_pJumpPointDlg->ShowWindow(SW_SHOW);
		break;
	default:
	    break;
	}
}

void CMainDlg::OnBnClickedBtnLoaddropitem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

#if 0
	// ֻ�����̵�͵���
	ShopReader::Instance()->InitializeX();
	DropItemReader::Instance()->InitializeX();
#else
	// ���¼������б��
	FTableReaderBase::Reload();
#endif

	// ���������Ҫ���¼���
	DropItemManager::Instance()->load();
}
