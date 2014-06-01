// CreateNpcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GameEditor.h"
#include "CreateJumpPointDlg.h"
#include "StoreItemDlg.h"

JumpPointInfo CCreateJumpPointDlg::m_lastInfo;

// CCreateNpcDlg 对话框

IMPLEMENT_DYNAMIC(CCreateJumpPointDlg, CDialog)

CCreateJumpPointDlg::CCreateJumpPointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateJumpPointDlg::IDD, pParent)
{
	m_targetMapX = 1;
	m_targetMapY = 1;
	m_isEditMode = FALSE;
	m_pJumpPointEdit = NULL;
}

CCreateJumpPointDlg::~CCreateJumpPointDlg()
{
}

void CCreateJumpPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TARGET_MAP, m_cmbTargetMap);
	DDX_Text(pDX, IDC_EDIT_TARGETX, m_targetMapX);
	DDX_Text(pDX, IDC_EDIT_TARGETY, m_targetMapY);
	DDX_Text(pDX, IDC_EDIT_SCRIPT_FILE2, m_scriptFile);
}


BEGIN_MESSAGE_MAP(CCreateJumpPointDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CCreateNpcDlg 消息处理程序


void CCreateJumpPointDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	// 检查出生区域
	UpdateData();

	int index = m_cmbTargetMap.GetCurSel();
	const stMapInfo* mapInfo = (const stMapInfo*)m_cmbTargetMap.GetItemData(index);
	if (mapInfo == NULL)
		return;

	JumpPointInfo* pInfo = new JumpPointInfo;
	pInfo->targetMapId = mapInfo->id;
	pInfo->targetMapX = m_targetMapX;
	pInfo->targetMapY = m_targetMapY;
	pInfo->scriptFile  = m_scriptFile;

	// 保存
	m_lastInfo = *pInfo;

	if (m_isEditMode) {
		AfxGetMainWnd()->PostMessage(WM_NEWMONDLG_EDIT_JUMPPOINT, (WPARAM)m_pJumpPointEdit->GetID(), (LPARAM)pInfo);
	} else {
		AfxGetMainWnd()->PostMessage(WM_NEWMONDLG_CREATE_JUMPPOINT, 0, (LPARAM)pInfo);
	}

	OnOK();
}

void CCreateJumpPointDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


BOOL CCreateJumpPointDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 列出地图
	// TODO:  在此添加额外的初始化
	vector<stMapInfo*> list;
	GameTable::getAllMapInfo(list);
	for (size_t i=0; i<list.size(); i++) {
		stMapInfo* info = list[i];
		CString strMapName;
		const char* countryName = Misc::countrName(info->country);
		strMapName.Format("[%d]%s-%s", info->id, countryName, Misc::utf8ToGbk(info->name).c_str());
		m_cmbTargetMap.AddString(strMapName);

		int index = m_cmbTargetMap.GetCount();
		m_cmbTargetMap.SetItemData(index-1, (DWORD_PTR)info);
	}
	m_cmbTargetMap.SetCurSel(0);

	InitEditModeControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCreateJumpPointDlg::InitEditModeControls()
{
	if (!m_isEditMode)
		return;

	SetWindowText("修改跳转点");

	JumpPoint& jumpPoint = *m_pJumpPointEdit;
	JumpPointInfo& info = jumpPoint.GetInfo();

	for (int i=0; i<m_cmbTargetMap.GetCount(); ++i) {
		stMapInfo* mapInfo = (stMapInfo*)m_cmbTargetMap.GetItemData(i);
		if (mapInfo == NULL) continue;
		if (mapInfo->id == info.targetMapId) {
			m_cmbTargetMap.SetCurSel(i);
			break;
		}
	}

	m_targetMapX = info.targetMapX;
	m_targetMapY = info.targetMapY;
	m_scriptFile = info.scriptFile.c_str();

	UpdateData(FALSE);
}

void CCreateJumpPointDlg::EditJumpPoint(JumpPoint* pObj)
{
	m_isEditMode = TRUE;
	m_pJumpPointEdit = pObj;
}
