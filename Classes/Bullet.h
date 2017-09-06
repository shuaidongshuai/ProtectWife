#ifndef __BULLET_H__
#define __BULLET_H__

#include "Tank.h"

class Bullet:public CCSprite
{
public:
	static Bullet* create(int damage, Tank::STATUS status,Tank::TYPE type);

	CC_SYNTHESIZE(int, _damage, Damage);
	CC_SYNTHESIZE(Tank::STATUS, _status, Status);
	CC_SYNTHESIZE(Tank::TYPE, _type, Type);

};

#endif