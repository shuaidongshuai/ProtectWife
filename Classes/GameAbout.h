#ifndef __GAMEABOUT_H__
#define __GAMEABOUT_H__

#include "Common.h"

class GameAbout:public CCLayer
{
public:
	static GameAbout* create(bool isboy,bool isAbout)
	{
		GameAbout* g = new GameAbout;
		g->init(isboy, isAbout);
		g->autorelease();
		return g;
	}
	bool init(bool isboy, bool isAbout);
	void rCallback(CCObject*);
	void boyGirl(bool);
	void introduce();
};

#endif