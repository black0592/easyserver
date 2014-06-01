#pragma once
#include "BaseTypes.h"
#include "TraversalNote.h"
#include <vector>
using namespace std;
using namespace easygame;

class IMapModel
{
public:
	// ��ͼ����
	virtual byte** getMap(void) = 0;
	virtual void setMap(byte** map, int width, int height) = 0;
	// ��ʼ��
	virtual void reset(void) = 0;
	// ����Node
	virtual void setNode(POINT& v, TraversalNote* node) = 0;
	// ȡ��Node
	virtual TraversalNote* getNode(POINT& v) = 0;
	/**
	* �ṩ�ɱ����Ľڵ�
	* �����ṩ���ǰ˷����ƶ�
	* @param p	��ǰ�ڵ�
	* @return 
	*/
	virtual void getArounds(POINT& v, vector<POINT>& result) = 0;

	/**
	* �ж��Ƿ����
	* 
	* @param cur	��ǰ�ڵ�
	* @param end	�յ�
	* @return 	�Ƿ���ͬ�Ĳ���ֵ
	* 
	*/
	virtual bool reachEnd(POINT& cur, POINT& end) = 0;

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
	virtual int getCostAddon(POINT& start, POINT& cur, POINT& next, POINT& end) = 0;

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
	virtual int getScoreAddon(POINT& start, POINT& cur, POINT& next, POINT& end) = 0;
};


