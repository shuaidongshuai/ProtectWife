#ifndef __MENULAYER_H__
#define __MENULAYER_H__

#include "Common.h"

class MenuLayer:public CCLayer
{
	CCMenuItem* _item;
public:
	static int _mapIndex;//当前玩的是第几关
	CREATE_FUNC(MenuLayer);
	bool init();

	void Toggle(CCObject*);
	void Go(CCObject*);
	void quanTouchGame(CCObject*);
	void noquanTouchGame(CCObject*);
	void rCallback(CCObject*);
	void checkCallback(CCObject*);

	void initReturn();
	void initCheck();
	void initSwitchLevel();
	void initJumpSprite();
	void initcue();
	void initLightning();
	void initStore();

	void rotateItem(CCObject*);
	void AppStore(CCObject*);

};

#endif