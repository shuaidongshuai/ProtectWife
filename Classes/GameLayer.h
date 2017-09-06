#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "Common.h"
#include "Tank.h"
#include "AI.h"
#include "Prop.h"
#include "BigFire.h"
#include "Panel.h"

class GameLayer:public CCLayer
{
	int _level;
	CCTMXTiledMap* _map;
	Tank* _hero;
	Tank::DIR _heroDir;
	AI* _ai;
	CCLabelTTF* _blood;
	CCLabelTTF* _bigFireItem;
	CCLabelTTF* _bigFireItem1;
	CCLabelTTF* _bigFireItem2;
	CCLabelTTF* _killedTankItem;
	int _bigFireCount1;//正在使用的个数
	int _bigFireCount2;
	int _bigFireCount3;
	CCSprite* _wife;
	CCSprite* _smallWife;
	CCSprite* _xuanwo;

	BigFire* _b1;
	BigFire* _b2;
	BigFire* _b3;
	Panel* _menu;

	CCMenuItemImage* _fireitem;
	CCMenuItem* _returnItem;

	bool _isStrongFire;
	CCSprite* _sL;
	CCSprite* _sR;
public:
	static bool _isFullScreamTouch;
	static float winHeight;
	static int _canBigFire1;//技能可以存起来
	static int _canBigFire2;
	static int _canBigFire3;
	CCArray* _props;
	CCArray* _bigFires1;
	CCArray* _bigFires2;
	CCArray* _bigFires3;
	static GameLayer* create(int level, bool isFullScreamTouch);
	int _killedTank;//杀死的坦克数量
	int _needKilled;

	void onEnter();
	void onExit();

	void initMap();
	void initHero();

	void initfire();
	void initAI();
	void fireCallBack(CCObject*);
	void DirCallback(CCObject*);
	void update(float t);
	void check(CCArray* bullets);
	bool checkFireInMap(CCPoint &mapPt, CCArray* bullets, Bullet* b, int index);
	void checkTankBullets(CCArray* bullets);
	void checkHeroProp();
	void gameOver_lose();
	void gameOver_win();
	void checkWin();
	void checkBigFire();
	void checkWife();
	void checkMoveScream();

	void initReturn();
	void rCallback(CCObject*);
	void addProp();
	void eatProp(Prop::TYPE t);
	void eatLife();
	void eatKill();
	void eatBoss();
	void eatFire();
	void eatStop();
	void eatStrong();
	void showItem();
	void initshowItem();
	void continueMove(float);
	
	void shootBigFire1();
	void shootBigFire2();
	void shootBigFire3();

	void removeBigFire1();
	void removeBigFire2();
	void removeBigFire3();

	void win(CCObject*);
	void lose(CCObject*);

	void recoveryBricks(float);
	void initBigFire();
	void wifeRun();
	void initWife();
	void StrongFire(float);
	void setVisiable();
	void fullScreamTouch();
	void nofullScreamTouch();


	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

};

#endif