#pragma once

#pragma warning(disable:4244)

// ��������
struct BirthRect 
{
	int x;	// ���õ������
	int y;
	int cx;	// x����ĸ�����
	int cy; // y����ĸ�����

	int getWidth(){ return cx; }
	int getHeight(){ return cy; }
};

// �ֲ���Ϣ�ṹ
struct MONINFO 
{
	int race;		// ��������
	int num;		// ����

	int mapId;		// ��ͼ���
	int ai;			// ai���
	int speed;		// �ƶ��ٶ�
	int view;		// ��Ұ
	BirthRect birthRect;	// ��������
	MonDropInfo dropInfo[MON_DROP_NUM];
	int reviveTime;	// ����ʱ��(��)
	int isSiegeWar;	// �Ƿ񹥳ǹ���
	int isEventMon;	// �Ƿ��¼�����
	bool isRelive;	// �Ƿ񸴻�

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

	// ��������ֲ�ͼ
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



