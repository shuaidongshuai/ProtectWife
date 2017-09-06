#pragma once
#ifndef _STARTlAYER_H__
#define _STARTlAYER_H__

#include "Common.h"

class StartLayer:public CCLayer
{
public:
	static StartLayer* create(bool isFirst = true)
	{
		StartLayer* s = new StartLayer;
		s->init(isFirst);
		s->autorelease();
		return s;
	}
	bool init(bool isFirst);

	void addMenuItem();

	void enterNextLayer(CCObject*);
	void exitGame(CCObject*);
	void gameAbout1(CCObject*);
	void gameAbout2(CCObject*);
	void introduce(CCObject*);
	void goA();
	void goB();
};

#endif