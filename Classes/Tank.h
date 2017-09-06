#ifndef __TANK_H__
#define __TANK_H__

#include "Common.h"
#include "AI.h"

class Bullet;

class Tank:public CCSprite
{
public:
	CCArray* _bullets;

	enum TYPE{ FRIEND, ENEMY };
	enum DIR{ U, R, D, L, NONE };
	enum STATUS{ NORMAL, ABNORMAL };

	CC_SYNTHESIZE(TYPE, _type, Type);//synthesize 综合，合成
	CC_SYNTHESIZE(int, _damage, Damage);
	CC_SYNTHESIZE(int, _hp, Hp);
	CC_SYNTHESIZE(STATUS, _state, Status);
	CC_PROPERTY(DIR, _dir, Dir);// property 特性  //自定义dir函数，，因为要根据dir来旋转tank。。改变tank的朝向
	CC_SYNTHESIZE(int, _speed, Speed);

	static Tank* create(TYPE t, int damage, int hp, STATUS s, DIR d);

	void moveTank(DIR dir,AI* ai, Tank* hero);
	bool canMove(DIR dir, AI* ai, Tank* hero);

	CCTMXTiledMap* getMap()
	{
		return (CCTMXTiledMap*)getParent();
	}

	void fire(float t = 0);
	void shooted();

	//void onEnter()
	//{
	//	CCSprite::onEnter();

	//	_bullets = CCArray::create();//友方子弹数组
	//	_bullets->retain();
	//}
	//void onExit()
	//{
	//	CCSprite::onExit();
	//	_bullets->release();
	//}
};

#endif