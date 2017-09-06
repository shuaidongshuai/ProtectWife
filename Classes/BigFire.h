#ifndef __BIGFIRE_H__
#define __BIGFIRE_H__

#include "Common.h"

class BigFire:public CCMenuItemImage
{
	bool _inGame;
public:
	static BigFire* create(int num = 0, bool inGame = true)
	{
		BigFire* b = new BigFire;
		b->init(num, inGame);
		b->autorelease();
		return b;
	}
	bool init(int num,bool inGame);
	void fire(CCObject*);
	void fire1(CCObject*);
	void fire2(CCObject*);
};

#endif