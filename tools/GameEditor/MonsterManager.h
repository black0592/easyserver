#pragma once

#pragma warning(disable:4244)

// 出生区域
struct BirthRect 
{
	int x;	// 放置的坐标点
	int y;
	int cx;	// x方向的格子数
	int cy; // y方向的格子数

	int getWidth(){ return cx; }
	int getHeight(){ return cy; }
};

// 分布信息结构
struct MONINFO 
{
	int race;		// 怪物种类
	int num;		// 数量

	int mapId;		// 地图编号
	int ai;			// ai编号
	int speed;		// 移动速度
	int view;		// 视野
	BirthRect birthRect;	// 出生区域
	MonDropInfo dropInfo[MON_DROP_NUM];
	int reviveTime;	// 复活时间(秒)
	int isSiegeWar;	// 是否攻城怪物
	int isEventMon;	// 是否事件怪物
	bool isRelive;	// 是否复活

	MONINFO()
	{
		bzero(this,sizeof(*this));
		isRelive = true;
	}
};



class Monster
{
public:
	Monster();
	virtual ~Monster();

	void SetID(UINT id);
	UINT GetID();

	void SetBaseID(UINT id) { m_baseId = id; }
	UINT GetBaseID() { return m_baseId; }

	UINT GetLevel();
	string getBaseName();

	void SetNum(UINT num);
	UINT GetNum();

	POINT GetPoint();

	void SetName(const char* name);
	void SetName(const std::string& name);
	const char* GetName();

	void FillInfo(MONINFO& info);
	MONINFO& GetMonInfo();

	void SetGridPos(int x, int y);

	bool Load(UINT baseId);

	void Draw(POINT ptMap);

	void DrawToMiniMap(FLOAT scalX, FLOAT scalY);

	void DrawPositionToMiniMap(FLOAT scalX, FLOAT scalY);

	void Update();

	void SetPoint(POINT pt);

	bool OnMouseMove(POINT ptMap, POINT mousePt);

	bool IsSelected();
	
	void SetSelected(BOOL flag);
	

protected:
	UINT m_id;
	UINT m_baseId;
	UINT m_num;
	string m_name;
	POINT m_pt;
	SpriteSheet mSpriteSheet;
	MONINFO m_info;
	UINT m_curFrm;
	UINT m_Effect;
	UINT m_nameColor;
	BOOL m_isMouseMove;	
	BOOL m_isSelected;

private:
};


class MonsterManager
{
public:
	MonsterManager(void);
	virtual ~MonsterManager(void);

	void InitListCtrl(CListCtrl* listCtrl);

	void Release();

	bool LoadMonster(UINT mapId);

	bool SaveToFile();

	UINT GetCount();

	void Draw(POINT ptMap);

	void DrawToMiniMap(FLOAT scalX, FLOAT scalY);

	// 画出怪物分布图
	void DrawMonsterPosition(FLOAT scalX, FLOAT scalY);

	void Update();

	bool AddMonster(Monster* monster);

	void EditMonster(Monster* monster);

	void DeleteMonster(UINT uinID);

	void OnMouseMove(POINT ptMap, POINT mousePt);

	void UnSelectAll();

	Monster* FindSelectMonster(POINT ptMap, POINT mousePt);

	Monster* FindMonster(UINT id);

	Monster* GetSelectedMonster();

private:
	UINT m_mapId;
	CListCtrl* m_pListCtrl;
	map<UINT,Monster*> m_objMap;
	UINT m_uinID;

	DWORD m_lastTick;
	BOOL m_isDrawMonPostion;
};



