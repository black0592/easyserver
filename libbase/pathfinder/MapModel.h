#pragma once
#include "IMapModel.h"

#define COST_STRAIGHT 10	//ˮƽ��ֵ
#define COST_DIAGONAL 14	//б���ֵ

class MapModel :
	public IMapModel
{
public:
	MapModel();
	virtual ~MapModel(void);

	void release();

	// ��ͼ����
	virtual byte** getMap(void) { return _map; }
	virtual void setMap(byte** map, int width, int height);
	// ��ʼ��
	virtual void reset(void);
	// ����Node
	virtual void setNode(POINT& v, TraversalNote* node);
	// ȡ��Node
	virtual TraversalNote* getNode(POINT& v);
	/**
	* �ṩ�ɱ����Ľڵ�
	* �����ṩ���ǰ˷����ƶ�
	* @param p	��ǰ�ڵ�
	* @return 
	*/
	virtual void getArounds(POINT& v, vector<POINT>& result);

	/**
	* �ж��Ƿ����
	* 
	* @param cur	��ǰ�ڵ�
	* @param end	�յ�
	* @return 	�Ƿ���ͬ�Ĳ���ֵ
	* 
	*/
	virtual bool reachEnd(POINT& cur, POINT& end);

	/**
	* ���Cost���ڸ��ڵ�Cost�ļ�ֵ
	* 
	* @param start	�׽ڵ�
	* @param cur	���ڵ�
	* @param next	��ǰ�ڵ�
	* @param end	�յ�
	* @return 
	* 
	*/
	virtual int getCostAddon(POINT& start, POINT& cur, POINT& next, POINT& end);

/**
	* ���Score����Cost�ļ�ֵ
	*  
	* @param start	�׽ڵ�
	* @param cur	���ڵ�
	* @param next	��ǰ�ڵ�
	* @param end	�յ�
	* @return 
	* 
	*/
	virtual int getScoreAddon(POINT& start, POINT& cur, POINT& next, POINT& end);

	bool isBlock(POINT& end, POINT& cur);
	
public:
	bool diagonal;	// �Ƿ�����б���ƶ�

private:
	TraversalNote*** noteMap;	//node����
	byte** _map;
	int mWidth;
	int mHeight;
};
