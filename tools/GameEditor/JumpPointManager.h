#pragma once


struct JumpPointInfo
{
	UINT mapX;
	UINT mapY;

	// 目标地图和点
	UINT targetMapId;
	UINT targetMapX;
	UINT targetMapY;
	string scriptFile;	// NPC脚本文件

	JumpPointInfo()
	{
		mapX = mapY = 0;

		targetMapId = 0;
		targetMapX = 0;
		targetMapY = 0;
	}
};



class JumpPoint
{
public:
	JumpPoint();
	virtual ~JumpPoint();

	void SetID(UINT id);
	UINT GetID();

	POINT GetPoint();

	const char* GetName();

	void FillInfo(JumpPointInfo& info);

	JumpPointInfo& GetInfo();

	void SetGridPos(int x, int y);

	bool Load(UINT looks = 0);

	void Draw(POINT ptMap);

	void DrawPositionToMiniMap(FLOAT scalX, FLOAT scalY);

	void Update();

	void SetPoint(POINT pt);

	bool OnMouseMove(POINT ptMap, POINT mousePt);

	bool IsSelected();

	void SetSelected(BOOL flag);

protected:
	UINT m_id;
	string m_name;
	POINT m_pt;
	SpriteSheet mSpriteSheet;
	JumpPointInfo m_info;
	UINT m_curFrm;
	UINT m_Effect;
	UINT m_nameColor;
	BOOL m_isMouseMove;	
	BOOL m_isSelected;

private:
};

class JumpPointManager
{
public:
	JumpPointManager(void);
	virtual ~JumpPointManager(void);

	void InitListCtrl(CListCtrl* listCtrl);

	void Release();

	UINT GetCount();

	bool LoadJumpPoint(UINT mapId);

	void Draw(POINT ptMap);

	// 画出npc分布图
	void DrawPosition(FLOAT scalX, FLOAT scalY);

	void Update();

	bool AddJumpPoint(JumpPoint* obj);

	void EditJumpPoint(JumpPoint* obj);

	void DeleteJumpPoint(UINT uinID);

	void OnMouseMove(POINT ptMap, POINT mousePt);

	void UnSelectAll();

	JumpPoint* FindSelectJumpPoint(POINT ptMap, POINT mousePt);

	JumpPoint* FindJumpPoint(UINT id);

	JumpPoint* GetSelectedJumpPoint();

	bool SaveToFile();

private:
	UINT m_mapId;
	CListCtrl* m_pListCtrl;
	map<UINT,JumpPoint*> m_objMap;
	UINT m_uinID;

	DWORD m_lastTick;
	BOOL m_isDrawPostion;
};
