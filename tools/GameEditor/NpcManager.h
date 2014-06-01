#pragma once

enum ENpcFunc
{
	NPC_Normal = 0,			// ��ͨ�Ի�npc
	NPC_BuySell = 1,		// NPC�����̵�
	NPC_Warehouse = 2,		// �ֿ�
	NPC_Marriage = 3,		// �������� 
	NPC_GuildMgr = 4,		// �������
	NPC_Attrib = 5,			// ����
	NPC_NationMgr = 6,		// ���ҹ���
	NPC_GuildWar = 7,		// ����ս
	NPC_RunEattach = 8,		// ����
	NPC_Gamble = 9,			// �Ĳ�

	NPC_NONE = 255,			// ��Ч����(С����С����Щ)
};

#pragma pack(1)

struct NPCINFO 
{
	UINT id;			// npc���
	UINT mapX;			// ��ͼ����X
	UINT mapY;			// ��ͼ����Y
	UINT baseId;		// npc�������id
	UINT npcFunc;		// npc���� (ENpcFunc)
	UINT storeId;		// �̵���
	string scriptFile;	// NPC�ű��ļ�

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

	// ����npc�ֲ�ͼ
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
