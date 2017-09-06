#include "Tank.h"
#include "Bullet.h"
#include "Music.h"
#include "GameLayer.h"
#include "MenuLayer.h"

Tank* Tank::create(TYPE t, int damage, int hp, STATUS s, DIR d)
{
	Tank* tank = new Tank;
	tank->setType(t);
	tank->setDamage(damage);
	tank->setHp(hp);
	tank->setStatus(s);
	tank->setDir(d);
	tank->setSpeed(250);//初始化速度为280

	if (tank->getType() == FRIEND)
		tank->initWithFile(IMAGE_playerU);
	else
	{
		//int rand = CCRANDOM_0_1() * 4 + 1;//"tank/enemy/enemy1D.png"
		static int rand = 0;
		tank->initWithFile(Util::format((rand++) % 6 + 1, "tank/enemy/enemy", "D.png"));

		/*设置不同的难度*/
		int level = MenuLayer::_mapIndex / 5;
		if (level == 0)
			tank->schedule(schedule_selector(Tank::fire), 1.8);
		else if (level == 1)
			tank->schedule(schedule_selector(Tank::fire), 1);
		else if (level == 2)
			tank->schedule(schedule_selector(Tank::fire), 0.5);
		else if (level == 3)
			tank->schedule(schedule_selector(Tank::fire), 0.2);

	}

	tank->autorelease();
	return tank;
}
void Tank::setDir(DIR dir)
{
	_dir = dir;
	setRotation(dir * 90);//Rotation 旋转
}
Tank::DIR Tank::getDir()
{
	return _dir;
}

void Tank::moveTank(DIR dir, AI* ai, Tank* hero)
{
	if (dir == NONE && getType() == Tank::ENEMY)//增加这一句，是因为，我要在 停止敌方坦克 的时候会用到
		return;

	setDir(dir);//改变tank头的方向

	if (canMove(dir,ai,hero))
	{
		float off = 1.0 / 60 * getSpeed();
		switch (dir)
		{
		case U:
			setPositionY(getPositionY() + off);
			break;
		case R:
			setPositionX(getPositionX() + off);
			break;
		case D:
			setPositionY(getPositionY() - off);
			break;
		case L:
			setPositionX(getPositionX() - off);
			break;
		}
	}
	else if (getType() == ENEMY)//首先，没能移动，加上是敌方tank  才执行下面代码
	{
		// 要自动掉头，随机找一个方向，掉头
		
		//while(dir = CCRANDOM_0_1() * 4 == 4);//获取 0 1 2 3 这几个随机数，
		int dir = rand() % 4;
		setDir((DIR)dir);//掉头
	}
}

bool Tank::canMove(DIR dir, AI* ai, Tank* hero)
{
	//获取tank的boundingbox
	CCRect rt = boundingBox();

	float off = 1.0 / 60 * getSpeed();//获取每一帧移动的距离

	CCPoint pt[3];
	switch (dir)
	{
	case U:
		rt.origin.y += off;//把矩形向上移动
		//检查上面上个点
		pt[0] = ccp(rt.getMinX(), rt.getMaxY());
		pt[1] = ccp(rt.getMidX(), rt.getMaxY());
		pt[2] = ccp(rt.getMaxX(), rt.getMaxY());
		break;
	case R:
		rt.origin.x += off;
		pt[0] = ccp(rt.getMaxX(), rt.getMinY());
		pt[1] = ccp(rt.getMaxX(), rt.getMidY());
		pt[2] = ccp(rt.getMaxX(), rt.getMaxY());
		break;
	case D:
		rt.origin.y -= off;
		pt[0] = ccp(rt.getMinX(), rt.getMinY());
		pt[1] = ccp(rt.getMidX(), rt.getMinY());
		pt[2] = ccp(rt.getMaxX(), rt.getMinY());
		break;
	case L:
		rt.origin.x -= off;
		pt[0] = ccp(rt.getMinX(), rt.getMinY());
		pt[1] = ccp(rt.getMinX(), rt.getMidY());
		pt[2] = ccp(rt.getMinX(), rt.getMaxY());
		break;
	default:
		return false;
	}

	//获取地图
	CCTMXTiledMap* map = getMap();
	
	//获取层
	CCTMXLayer* layer0 = map->layerNamed("layer_0");

	for (int i = 0; i < 3; i++)
	{
		//判断是否在地图内
		if (!Util::isContent(pt[i], map))
			return false;

		//判断是否撞墙
		CCPoint mapPt = Util::GL2map(pt[i], map);
		int gid = layer0->tileGIDAt(mapPt);//获取gid
		TILE_TYPE type = tileGidType[gid];
		//if (type == tileKing || type == tileRiver || type == tileSteel || type == tileWall)
		if (type != tileNone && type != tileGrass)
			return false;

		//看我方tank是否和对方tank碰撞
		if (getType() == FRIEND)//我方和敌人碰撞，我方不能走
		{
			for (int j = ai->_tanks->count() - 1; j >= 0; j--)//遍历敌方Tank
			{
				Tank* tank = (Tank*)ai->_tanks->objectAtIndex(j);
				if (tank->boundingBox().containsPoint(pt[i]))
					return false;
			}
		}
		else//敌方和我方碰撞，敌方不能走
		{
			if (hero->boundingBox().containsPoint(pt[i]))
				return false;
		}
	}
	return true;
}

void Tank::fire(float)
{
	Bullet* b = Bullet::create(getDamage(), getStatus(), getType());

	//如果没有这句话。子弹默认出现在（0,0）当我们没有禁止坦克动的时候不会有问题立马就发射出去了，但是如果被禁止，恰好又发射了子弹，子弹就会出现在（0，0）这样如果老婆在（0,0）就会死
	b->setPosition(ccp(-20, -20));

	_bullets->addObject(b);//加入到数组中

	//把子弹加入到map中,,用map来渲染子弹（要显示出来必须加入到渲染树中）
	getMap()->addChild(b);

	//让子弹飞
	CCPoint delta;
	switch (_dir)
	{
	case Tank::U:
		delta = ccp(0, winSize.height * 3);
		//设置位置
		b->setPosition(getPosition() + ccp(0,getContentSize().height/2));
		break;
	case Tank::R:
		delta = ccp(winSize.height * 3, 0);
		b->setPosition(getPosition() + ccp(getContentSize().height / 2,0));
		break;
	case Tank::D:
		delta = ccp(0, -winSize.height * 3);
		b->setPosition(getPosition() + ccp(0, -getContentSize().height / 2));
		break;
	case Tank::L:
		delta = ccp(-winSize.height * 3, 0);
		b->setPosition(getPosition() + ccp(-getContentSize().height / 2,0));
		break;
	}

	b->runAction(CCMoveBy::create(3.8, delta));
}

//被打中的效果
void Tank::shooted()
{
	if (getType() == ENEMY)
	{
		GameLayer* game = (GameLayer*)getParent()->getParent();
		game->_killedTank++;
		game->_needKilled--;
	}

	//减血
	setHp(getHp() - 1);

	if (getHp() <= 0)//发现有的坦克在爆炸时，后，还有可能发射子弹
	{
		unschedule(schedule_selector(Tank::fire));
	}

	//爆炸效果  // 用一个动画来表示坦克挂掉
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile(PLIST_blast);
	CCArray* arr = CCArray::create();
	for (int i = 0; i < 8; i++)
	{
		CCSpriteFrame* frame = cache->spriteFrameByName(Util::format(i + 1, IMAGE_blast, ".gif"));
		arr->addObject(frame);
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(arr, .1f);
	CCAnimate* animate = CCAnimate::create(animation);


	//敌方英雄和我方英雄被打到，代码不一样
	if (getType() == FRIEND)//如果是我方英雄被打
	{
		CCSprite* s = CCSprite::create();
		addChild(s);
		s->setPosition(ccp(getContentSize().width / 2, getContentSize().height / 2));
		s->runAction(CCSequence::create(animate, CCRemoveSelf::create(), NULL));//只毁灭精灵
	}
	else
	{
		//敌方英雄被打死，还要随机增加一个道具

		//设置一个自定义动作函数
		CCCallFunc* callFunc = CCCallFunc::create(getParent()->getParent(), callfunc_selector(GameLayer::addProp));

		runAction(CCSequence::create(animate, callFunc, CCRemoveSelf::create(), NULL));


		//上面的做法和下面的做法，差别在于，，上面是死了以后才有可能产生道具，，，下面这个在第一帧爆炸时就会有道具产生
		//runAction(CCSequence::create(animate, CCRemoveSelf::create(), NULL));
		//getParent()->getParent();
		//因为cocos是每一帧都要执行完全部代码，爆炸在下一帧才会产生，而道具也是在下一帧产生，但是爆炸有8张图，会持续8帧
	}
}
