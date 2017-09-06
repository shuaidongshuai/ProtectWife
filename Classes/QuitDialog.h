#ifndef __QUITDIALOG_H__
#define __QUITDIALOG_H__

#include "Common.h"
#include "GameLayer.h"

class QuitDialog:public CCLayer
{
	bool _isStartLayer;
	bool _menuClick;
	CCMenu* _menu;
	CCEditBox* _box;
	const char* _name;
	GameLayer* _game;
public:
	static QuitDialog* create(bool isStartLayer, bool isGameOver = false, bool isWin = false,GameLayer *game = NULL)
	{
		QuitDialog* q = new QuitDialog;
		q->init(isStartLayer, isGameOver, isWin, game);
		q->autorelease();
		return q;
	}
	bool init(bool isStartLayer, bool isGameOver, bool isWin, GameLayer *game);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	void outGame();
	void winGame();
	void loseGame();
	void upScore(CCObject*);
	void Yes(CCObject*);
	void No(CCObject*);

	void checkName(float);

	void onEnter()
	{
		CCLayer::onEnter();
		CCDirector::sharedDirector()->pause();
	}
	void onExit()
	{
		CCLayer::onExit();
		CCDirector::sharedDirector()->resume();
	}
};

#endif