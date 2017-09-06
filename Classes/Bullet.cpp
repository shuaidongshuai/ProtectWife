#include "Bullet.h"

Bullet* Bullet::create(int damage, Tank::STATUS status, Tank::TYPE type)
{
	Bullet* b = new Bullet;
	b->setDamage(damage);
	b->setStatus(status);
	b->setType(type);

	if (b->getType() == Tank::FRIEND)
		b->initWithFile(IMAGE_heroBullet);
	else
		b->initWithFile(IMAGE_enemyBullet);
	return b;
}
