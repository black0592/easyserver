// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用 Windows 95 和 Windows NT 4 或更高版本的特定功能。
#define WINVER 0x0400		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0403 	//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用 Windows 98 或更高版本的特定功能。
#define _WIN32_WINDOWS 0x0410 //为 Windows Me 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_IE			// 允许使用 IE 4.0 或更高版本的特定功能。
#define _WIN32_IE 0x0400	//为 IE 5.0 及更新版本改变为适当的值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常被安全忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类

#include <afxdtctl.h>		// Internet Explorer 4 公共控件的 MFC 支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 公共控件的 MFC 支持
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
// 自定义消息

// 加载地图
#define WM_MAINDLG_LOADMAP				WM_USER + 100




// 创建怪物
#define WM_NEWMONDLG_CREATE_MONSTER		WM_USER + 200
// 修改怪物属性
#define WM_NEWMONDLG_EDIT_MONSTER		WM_USER + 201

// 选中怪物
#define WM_MONDLG_SELECT_MONSTER		WM_USER + 300




// 创建NPC
#define WM_NEWMONDLG_CREATE_NPC			WM_USER + 400
// 修改NPC属性
#define WM_NEWMONDLG_EDIT_NPC			WM_USER + 401

// 选中NPC
#define WM_MONDLG_SELECT_NPC			WM_USER + 500




// 创建跳转点
#define WM_NEWMONDLG_CREATE_JUMPPOINT	WM_USER + 600
// 修改跳转点属性
#define WM_NEWMONDLG_EDIT_JUMPPOINT		WM_USER + 701

// 选中跳转点
#define WM_MONDLG_SELECT_JUMPPOINT		WM_USER + 700




