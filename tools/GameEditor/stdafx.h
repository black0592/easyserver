// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// ��Ŀ�ض��İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0403 	//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
#define _WIN32_WINDOWS 0x0410 //Ϊ Windows Me �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0400	//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ���ĺͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

#include <afxdtctl.h>		// Internet Explorer 4 �����ؼ��� MFC ֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <map>
#include <set>
#include <list>
#include <vector>
#include <assert.h>

// easy2d
#include "easy2d.h"
using namespace easy2d;
using namespace pugi;

// serverlib
#include "DataManager.h"
#include "BaseDataManager.h"

// gameeditor
#include "EditorConfig.h"
#include "Misc.h"
#include "CommData.h"
#include "SpriteSheet.h"
#include "MonsterManager.h"
#include "NpcManager.h"
#include "JumpPointManager.h"
#include "DropItemManager.h"

using namespace std;

//#define  MINIMAP_WIDTH		200
//#define  MINIMAP_HEIGHT		150

extern FLOAT g_curMiniMapScalX;
extern FLOAT g_curMiniMapScalY;
extern MonsterManager* g_pMonMgr;
extern NpcManager* g_pNpcMgr;
extern JumpPointManager* g_pJumpPointMgr;


#define EF_NORMAL 0
#define EF_BLUE 1
#define EF_RED 2

#define CXTILE	64
#define CYTILE	32

#define WHITE			0xFFFFFFFF
#define BLACK			0xFF000000
#define YELLOW			0xFFFFFF00
#define BLUE			0xFF0000FF
#define RED				0xFFFF0000
#define GREEN			0xFF00FF00
#define MINIMAP_BOARD	0xFFFF00FF



//////////////////////////////////////////////////////////////////////////
// �Զ�����Ϣ

// ���ص�ͼ
#define WM_MAINDLG_LOADMAP				WM_USER + 100




// ��������
#define WM_NEWMONDLG_CREATE_MONSTER		WM_USER + 200
// �޸Ĺ�������
#define WM_NEWMONDLG_EDIT_MONSTER		WM_USER + 201

// ѡ�й���
#define WM_MONDLG_SELECT_MONSTER		WM_USER + 300




// ����NPC
#define WM_NEWMONDLG_CREATE_NPC			WM_USER + 400
// �޸�NPC����
#define WM_NEWMONDLG_EDIT_NPC			WM_USER + 401

// ѡ��NPC
#define WM_MONDLG_SELECT_NPC			WM_USER + 500




// ������ת��
#define WM_NEWMONDLG_CREATE_JUMPPOINT	WM_USER + 600
// �޸���ת������
#define WM_NEWMONDLG_EDIT_JUMPPOINT		WM_USER + 701

// ѡ����ת��
#define WM_MONDLG_SELECT_JUMPPOINT		WM_USER + 700




