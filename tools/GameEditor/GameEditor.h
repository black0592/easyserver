// GameEditor.h : GameEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CGameEditorApp:
// �йش����ʵ�֣������ GameEditor.cpp
//

class CGameEditorApp : public CWinApp
{
public:
	CGameEditorApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGameEditorApp theApp;
