#pragma once

enum ENpcFunc
{
	NPC_Normal = 0,			// 普通对话npc
	NPC_BuySell = 1,		// NPC买卖商店
	NPC_Warehouse = 2,		// 仓库
	NPC_Marriage = 3,		// 婚姻管理 
	NPC_GuildMgr = 4,		// 公会管理
	NPC_Attrib = 5,			// 属性
	NPC_NationMgr = 6,		// 国家管理
	NPC_GuildWar = 7,		// 公会战
	NPC_RunEattach = 8,		// 符文
	NPC_Gamble = 9,			// 赌博

	NPC_NONE = 255,			// 无效类型(小狗，小鸟这些)
};

#pragma pack(1)

struct NPCINFO 
{
	UINT id;			// npc编号
	UINT mapX;			// 地图坐标X
	UINT mapY;			// 地图坐标Y
	UINT baseId;		// npc基础表的id
	UINT npcFunc;		// npc功能 (ENpcFunc)
	UINT storeId;		// 商店编号
	string scriptFile;	// NPC脚本文件

	NPCINFO()
	{
		id = 0;
		mapX = 0;
		mapY = 0;
		baseId = 0;
		npcFunc = 0;
		storeId = 0;
	}
};

#pragma pack()


class Npc
{
public:
	Npc();
	virtual ~Npc();

	void SetID(UINT id);
	UINT GetID();

	void SetBaseID(UINT id);
	UINT GetBaseID();

	POINT GetPoint();

	void SetName(const char* name);
	const char* GetName();

	void SetFunc(UINT func) { mFunc = func; }
	UINT GetFunc() { return mFunc; }

	void FillNpcInfo(NPCINFO& npcInfo);
	NPCINFO& GetNpcInfo();

	void SetGridPos(int x, int y);

	bool Load(UINT baseId);

	void Draw(POINT ptMap);

	void DrawPositionToMiniMap(FLOAT scalX, FLOAT scalY);

	void Update();

	void SetPoint(POINT pt);

	bool OnMouseMove(POINT ptMap, POINT mousePt);

	bool IsSelected();

	void SetSelected(BOOL flag);

protected:
	UINT m_id;
	UINT m_baseId;
	string m_name;
	UINT mFunc;
	POINT m_pt;
	SpriteSheet mSpriteSheet;
	NPCINFO m_info;
	UINT m_curFrm;
	UINT m_Effect;
	UINT m_nameColor;
	BOOL m_isMouseMove;	
	BOOL m_isSelected;

private:
};

class NpcManager
{
public:
	NpcManager(void);
	virtual ~NpcManager(void);

	void InitListCtrl(CListCtrl* listCtrl);

	void Release();

	UINT GetCount();

	bool LoadNpc(UINT mapId);

	void Draw(POINT ptMap);

	// 画出npc分布图
	void DrawNpcPosition(FLOAT scalX, FLOAT scalY);

	void Update();

	bool AddNpc(Npc* npc);

	void EditNpc(Npc* npc);

	void DeleteNpc(UINT uinID);

	void OnMouseMove(POINT ptMap, POINT mousePt);

	void UnSelectAll();

	Npc* FindSelectNpc(POINT ptMap, POINT mousePt);

	Npc* FindNpc(UINT id);

	Npc* GetSelectedNpc();

	bool SaveToFile();

private:
	UINT m_mapId;
	CListCtrl* m_pListCtrl;
	map<UINT,Npc*> m_objMap;
	UINT m_uinID;

	DWORD m_lastTick;
	BOOL m_isDrawNpcPostion;
};
