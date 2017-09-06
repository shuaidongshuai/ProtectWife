#ifndef __AI_H__
#define __AI_H__

#include "Common.h"

// 管理敌军和敌军的子弹
class AI:public CCLayer
{
public:
	CREATE_FUNC(AI);

	int _countTank;
	CCArray* _tanks;
	CCArray* _bullets;

	void onEnter();
	void onExit();

	//用于检测子弹
	void update(float);
	//用于检测tank
	void addTanks(float);
	//转换方向
	void changeDir(float);

	CCTMXTiledMap* getMap()
	{
		return (CCTMXTiledMap*)getParent();
	}
};

#endif