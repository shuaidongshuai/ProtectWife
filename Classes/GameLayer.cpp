#include "GameLayer.h"
#include "Bullet.h"
#include "MenuLayer.h"
#include "Music.h"
#include "QuitDialog.h"
#include "StartLayer.h"
#include "SceneScore.h"

float GameLayer::winHeight;
bool GameLayer::_isFullScreamTouch;

//int GameLayer::_canBigFire1 = getFire1();
//int GameLayer::_canBigFire2 = getFire2();
//int GameLayer::_canBigFire3 = getFire3();

int GameLayer::_canBigFire1;
int GameLayer::_canBigFire2;
int GameLayer::_canBigFire3;

GameLayer* GameLayer::create(int level, bool isFullScreamTouch)
{
	_isFullScreamTouch = isFullScreamTouch;
	GameLayer* g = new GameLayer;
	g->_level = level;
	g->autorelease();
	return g;
}

void GameLayer::onEnter()
{
	CCLayer::onEnter();

	//背景音乐
	Music::stopBackgroundMusic();
	Music::StartGameMusic();

	// 初始化地图
	initMap();

	//创建英雄
	initHero();

	//初始化老婆
	initWife();

	//初始化游戏按钮(看是那种模式开始游戏)
	if (_isFullScreamTouch)
		fullScreamTouch();
	else
		nofullScreamTouch();

	//初始化敌方tank和子弹
	initAI();

	//增加一个返回按钮
	initReturn();

	//启动定时器 用来转换tank方向，看游戏是否结束,检查子弹
	scheduleUpdate();

	//Label显示
	initshowItem();

	//道具
	_props = CCArray::create();
	_props->retain();

	//还需要杀死的敌人
	if (MenuLayer::_mapIndex + 1 <= 2)//前2关设置的简单些
		_needKilled = 10;
	else
		_needKilled = (MenuLayer::_mapIndex + 1) / 5 * 10 + 30;
	_killedTank = 666;//防止修改

	//大火力
	_isStrongFire = false;

	
	//初始化发射子弹按钮
	initfire();

	/*让所有控件，都在同一个位置上*/
	winHeight = winSize.height / 2;

	/*获取大招的个数*/
	_canBigFire1 = getFire1();
	_canBigFire2 = getFire2();
	_canBigFire3 = getFire3();

	//初始化大招  要在最后初始化啊，吃了好几次亏
	initBigFire();
}

void GameLayer::onExit()
{
	CCLayer::onExit();
	_props->release();
	_bigFires1->release();
	_bigFires2->release();
	_bigFires3->release();
	_hero->_bullets->release();
}

void GameLayer::initBigFire()
{
	//大招1容器
	_bigFires1 = CCArray::create();
	_bigFires1->retain();
	_bigFireCount1 = 0;

	//大招2容器
	_bigFires2 = CCArray::create();
	_bigFires2->retain();
	_bigFireCount2 = 0;

	//大招3容器
	_bigFires3 = CCArray::create();
	_bigFires3->retain();
	_bigFireCount3 = 0;

	//创建大招精灵
	CCSprite* s;
	for (int i = 0; i < _canBigFire1; ++i)
	{
		s = CCSprite::create(IMAGE_bigFire);
		_bigFires1->addObject(s);
	}

	for (int i = 0; i < _canBigFire2; ++i)
	{
		s = CCSprite::create(IMAGE_bigFire2);
		_bigFires2->addObject(s);
	}

	for (int i = 0; i < _canBigFire3; ++i)
	{
		s = CCSprite::create(IMAGE_bigFire3);
		_bigFires3->addObject(s);
	}

	//第二个大招左右有两个一样的闪电，在这里就提前加入当前场景，占时看不见而已，这样做的目的：节约加载图片时间
	CCTexture2D* text = CCTextureCache::sharedTextureCache()->addImage(IMAGE_bigFire2);
	_sL = CCSprite::createWithTexture(text);
	_sR = CCSprite::createWithTexture(text);
	addChild(_sL);
	addChild(_sR);
	_sL->setScale(2);
	_sR->setScale(2);
}

void GameLayer::initMap()
{
	//创建地图
	_map = CCTMXTiledMap::create(Util::format(_level, MAP_Round, ".tmx"));

	//把地图加入到Game层中
	addChild(_map);

	//创建旋涡
	_xuanwo = CCSprite::create(IMAGE_nextLevel);
	addChild(_xuanwo);
	int x, y;
	if (MenuLayer::_mapIndex + 1 <= 1)//1关设置的简单些
	{
		x = 2;
		y = 2;
	}
	else
	{
		x = 40 * CCRANDOM_0_1();
		y = 40 * CCRANDOM_0_1() + 20;
	}
	_xuanwo->setPosition(ccp(48 * x, 48 * y));
	_xuanwo->setZOrder(-1);
	_xuanwo->runAction(CCRepeatForever::create(CCRotateBy::create(1, 100)));
}

void GameLayer::initHero()
{
	// 创建一个友军坦克，类型FRIEND，杀伤力是1， 生命值是3， 状态普通，方向向上
	_hero = Tank::create(Tank::FRIEND, 1, 5, Tank::NORMAL, Tank::U);

	_hero->_bullets = CCArray::create();
	_hero->_bullets->retain();

	_hero->setTag(666);//为了ai能用这个英雄

	//加入到Map中  目的：不用传参数进tank  tank直接获取父节点就行了
	_map->addChild(_hero);

	_hero->setPosition(ccp(17 * 48, 1 * 48));
	_heroDir = Tank::NONE;
}

void GameLayer::initfire()
{
	Panel* menu = Panel::create(true);
	addChild(menu);
	//加入发射子弹的按钮
	CCMenuItemImage* fireitem = CCMenuItemImage::create(IMAGE_fire, IMAGE_fire2);
	menu->addChild(fireitem);
	_fireitem = fireitem;

	fireitem->setTarget(this, menu_selector(GameLayer::fireCallBack));
	fireitem->setScale(2.8);
	fireitem->setPosition(ccp(820, -410));

	//大招1
	_b1 = BigFire::create();
	menu->addChild(_b1);

	//大招2
	_b2 = BigFire::create(1);
	menu->addChild(_b2);

	//大招3
	_b3 = BigFire::create(2);
	menu->addChild(_b3);
}

void  GameLayer::fireCallBack(CCObject*)
{
	_hero->fire();

	//加入声音
	Music::shootedMusic();
}

void GameLayer::DirCallback(CCObject* sender)
{
	CCMenuItem* item = (CCMenuItem*)sender;
	_heroDir = (Tank::DIR)(item->getTag() - 1000);//把int 转化为 TYPE  赋值给——_heroDir  从而达到控制方向
}

void GameLayer::update(float t)
{
#ifdef WIN32
	SHORT status = GetKeyState('A');
	if (status < 0)
	{
		_heroDir = Tank::L;
	}
	status = GetKeyState('W');
	if (status < 0)
	{
		_heroDir = Tank::U;
	}
	status = GetKeyState('S');
	if (status < 0)
	{
		_heroDir = Tank::D;
	}
	status = GetKeyState('D');
	if (status < 0)
	{
		_heroDir = Tank::R;
	}
	status = GetKeyState('J');//只判断是否被按下
	if (status < 0)
	{
		_hero->fire();
	}
#endif
	if (_heroDir != Tank::NONE)
	{
		_hero->moveTank(_heroDir, _ai, _hero);//如果方向变了，就判断是否能走了

		if (!_isFullScreamTouch)
			_heroDir = Tank::NONE;//一定要归0  不然会一直向前走，因为一直满足_heroDir != Tank::NONE
	}
	

	//检查子弹  与地图  之间的碰撞  看是否飞出地图外
	check(_hero->_bullets);

	//检查我方子弹  和敌方坦克之间的碰撞  还加了一个老婆
	checkTankBullets(_hero->_bullets);

	//检查hero和prop
	checkHeroProp();

	//显示血量
	showItem();

	//检查bigFire
	checkBigFire();

	//检查老婆是否可以抱走
	wifeRun();

	//检查老婆是否被救走
	checkWife();

	//检查移动屏幕不
	checkMoveScream();

	//检查游戏是否已经胜利了
	checkWin();
}

void GameLayer::checkWife()
{
	if (_wife == NULL)
	{
		_smallWife->setVisible(true);
		CCPoint pt = _hero->getPosition();
		switch (_hero->getDir())
		{
		case Tank::U:
			_smallWife->setPosition(pt + ccp(48, 0));
			break;
		case Tank::R:
			_smallWife->setPosition(pt + ccp(0, -48));
			break;
		case Tank::D:		
			_smallWife->setPosition(pt + ccp(-48, 0));
			break;
		case Tank::L:			
			_smallWife->setPosition(pt + ccp(0, 48));
			break;
		default:
			break;
		}
	}
}

void GameLayer::checkBigFire()
{
	for (int i = _bigFireCount1 - 1; i >= 0; i--)
	{
		CCSprite* bigFire = (CCSprite*)_bigFires1->objectAtIndex(i);

		//敌方tank和bigFire
		for (int j = _ai->_tanks->count() - 1; j >= 0; j--)
		{
			Tank* tank = (Tank*)_ai->_tanks->objectAtIndex(j);
			//if (bigFire->boundingBox().intersectsRect(tank->boundingBox()))//本来接触面积就打。这样更大
			if (bigFire->boundingBox().containsPoint(tank->getPosition()))//这样要好一点
			{
				tank->shooted();
				_ai->_tanks->removeObjectAtIndex(j);
			}
		}	
	}
	for (int i = _bigFireCount2 - 1; i >= 0; i--)
	{
		CCSprite* bigFire1 = (CCSprite*)_bigFires2->objectAtIndex(i);
		//敌方tank和bigFire
		for (int j = _ai->_tanks->count() - 1; j >= 0; j--)
		{
			Tank* tank = (Tank*)_ai->_tanks->objectAtIndex(j);
			if (bigFire1->boundingBox().containsPoint(tank->getPosition()) || _sL->boundingBox().containsPoint(tank->getPosition()) || _sR->boundingBox().containsPoint(tank->getPosition())) //这样要好一点
			{
				tank->shooted();
				_ai->_tanks->removeObjectAtIndex(j);
			}
		}
	}
	for (int i = _bigFireCount3 - 1; i >= 0; i--)
	{
		CCSprite* bigFire2 = (CCSprite*)_bigFires3->objectAtIndex(i);

		//敌方tank和bigFire
		for (int j = _ai->_tanks->count() - 1; j >= 0; j--)
		{
			Tank* tank = (Tank*)_ai->_tanks->objectAtIndex(j);
			if (bigFire2->boundingBox().intersectsRect(tank->boundingBox()))//这样要好一点
			{
				tank->shooted();
				_ai->_tanks->removeObjectAtIndex(j);
			}
		}
	}
}

void GameLayer::checkHeroProp()
{
	for (int i = _props->count() - 1; i >= 0; i--)
	{
		Prop* p = (Prop*)_props->objectAtIndex(i);
		if (p->boundingBox().intersectsRect(_hero->boundingBox()))
		{
			eatProp(p->getType());
			_props->removeObjectAtIndex(i);
			p->removeFromParent();
			return;
		}
	}
}

void GameLayer::checkWin()
{
	//if (_ai->_countTank >= 1)//测试用
	//if (_killedTank >= 1)
	//	gameOver_win();//测试用
	if (_wife == NULL && _xuanwo->boundingBox().containsPoint(_hero->getPosition()) && _needKilled <= 0)
	{
		gameOver_win();
		return;
	}
	if (_xuanwo->boundingBox().containsPoint(_hero->getPosition()))
	{
		CCSprite* s = CCSprite::create(IMAGE_tishiNoGo);
		addChild(s);
		s->setPosition(ccp(winSize.width / 2, winHeight + winSize.height / 2 - 105));
		stopAllActions();
		s->runAction(CCSequence::create(CCFadeOut::create(0.5), CCRemoveSelf::create(), NULL));
	}
}

void GameLayer::checkTankBullets(CCArray* bullets)
{
	int count = bullets->count();

	for (int i = count - 1; i >= 0; i--)//遍历子弹
	{
		Bullet* b = (Bullet*)bullets->objectAtIndex(i);

		//看老婆被打死没
		if (_wife != NULL&&_wife->boundingBox().containsPoint(b->getPosition()))
			gameOver_lose();

		if (b->getType() == Tank::FRIEND)//如果是我方子弹
		{
			int tanks = _ai->_tanks->count();//获取地方tank数量
			for (int j = tanks - 1; j >= 0; j--)//遍历对方坦克
			{
				Tank* tank = (Tank*)_ai->_tanks->objectAtIndex(j);//获取敌方某一架坦克
				if (b->boundingBox().intersectsRect(tank->boundingBox()))
				{
					//tank 效果
					tank->shooted();
					_ai->_tanks->removeObjectAtIndex(j);
					//子弹 效果
					b->removeFromParent();
					bullets->removeObjectAtIndex(i);
					break;
				}
			}
		}
		else//如果是地方子弹
		{
			if (b->boundingBox().intersectsRect(_hero->boundingBox()))//如果敌方子弹  打到我们坦克
			{
				_hero->shooted();
				//子弹 效果
				b->removeFromParent();
				bullets->removeObjectAtIndex(i);
				if (_hero->getHp() <= 0)
					gameOver_lose();
			}
		}
	}
}

void GameLayer::check(CCArray* bullets)
{
	int count = bullets->count();
	for (int i = count - 1; i >= 0; i--)
	{
		Bullet* b = (Bullet*)bullets->objectAtIndex(i);
		
		//子弹飞出窗外
		//if (!Util::isContent(b->getPosition(), _map))
		//{
		//	b->removeFromParent();
		//	//_heroBullets->removeObject(bullet);  两行代码是一样的，但是下一行效率应该更高
		//	bullets->removeObjectAtIndex(i);
		//	continue;
		//}
		//CCPoint mapPt = Util::GL2map(b->getPosition(), _map);
		//checkFireInMap(mapPt, bullets, b, i);

		float size = b->getContentSize().width / 6;
		float width = b->getPositionX();
		float height = b->getPositionY();
		/*
			加了子弹上下左右判断，就不能像上面那样简单的b->getPosition()判断在_map里面了
			因为有可能子弹跑出去一点点没有完全出去，但是我们没有把它removeFromParent
			还用它取获取地图的坐标，这样地图中就没有这样的坐标，导致越界
		*/
		if (!Util::isContent(ccp(width, height + size), _map) || !Util::isContent(ccp(width, height - size), _map)
			|| !Util::isContent(ccp(width - size, height), _map) || !Util::isContent(ccp(width + size, height), _map))
		{
			b->removeFromParent();
			//_heroBullets->removeObject(bullet);  两行代码是一样的，但是下一行效率应该更高
			bullets->removeObjectAtIndex(i);
			continue;
		}
		//对子弹上下左右都进行判断
		CCPoint mapPt = Util::GL2map(ccp(width, height + size), _map);
		if (!checkFireInMap(mapPt, bullets, b, i))
		{
			mapPt = Util::GL2map(ccp(width, height - size), _map);
			if (!checkFireInMap(mapPt, bullets, b, i))
			{
				mapPt = Util::GL2map(ccp(width - size, height), _map);
				if (!checkFireInMap(mapPt, bullets, b, i))
				{
					mapPt = Util::GL2map(ccp(width + size, height), _map);
					checkFireInMap(mapPt, bullets, b, i);
				}
			}
		}
	}

}
bool GameLayer::checkFireInMap(CCPoint &mapPt, CCArray* bullets, Bullet* b,int index)
{
	CCTMXLayer* layer0 = _map->layerNamed("layer_0");
	int gid = layer0->tileGIDAt(mapPt);
	//int gid = Util::getMapGID(_map, "layer_0", bullet->getPosition());
	TILE_TYPE type = tileGidType[gid];

	//子弹与墙
	if (type == tileWall)
	{
		b->removeFromParent();
		bullets->removeObjectAtIndex(index);
		layer0->setTileGID(0, mapPt);
		return true;
	}
	//子弹与金转
	if (type == tileSteel)
	{
		b->removeFromParent();
		bullets->removeObjectAtIndex(index);

		if (_isStrongFire && b->getType() == Tank::FRIEND && gid != 11)
			layer0->setTileGID(0, mapPt);
		return true;
	}
	return false;
}

void GameLayer::gameOver_lose()
{
	unscheduleUpdate();//这句话一定得加。。不然这个函数会被执行两次，在继续游戏时
	_ai->unscheduleUpdate();//如果是输了，敌方检察子弹的 定时器也必须关闭

	addChild(QuitDialog::create(false,true,false));
}

void GameLayer::gameOver_win()
{
	unscheduleUpdate();
	_ai->unscheduleUpdate();

	//首先判断当前关卡是否等于已经通过的最大关卡.....总不至于，玩到第10关，然后玩一下第一关，就变成了1吧
	if (MenuLayer::_mapIndex + 1 == getLevel())//+ 1 因为 _mapIndex 从0开始的
	{
		setLevel(getLevel() + 1);
	}

	addChild(QuitDialog::create(false,true,true,this));
}

void GameLayer::win(CCObject*)
{
	//顺利进去下一关
	MenuLayer::_mapIndex++;//标志也要跟着加1
	//changeScene(GameLayer,getLevel());//注意这里不能用getLevel，，当我们已通过10关，玩第一关的时候，岂不是第二句就是11关，而且像我前面设置的是_mapIndex是等于getLevel才下一关，这样要在11关玩10次
	changeScene(GameLayer, MenuLayer::_mapIndex + 1,_isFullScreamTouch);

//	_hero->_bullets->release();
}
void GameLayer::lose(CCObject*)
{
	changeScene(MenuLayer);
	Music::StartLayerMusic();

//	_hero->_bullets->release();
}


void GameLayer::initAI()
{
	_ai = AI::create();
	_map->addChild(_ai);//只是为了在AI里面好获取地图罢了
}

void GameLayer::initReturn()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	CCMenuItem* item = CCMenuItemImage::create(IMAGE_back_normal_white, IMAGE_back_press_white);
	menu->addChild(item);
	item->setTarget(this, menu_selector(GameLayer::rCallback));
	item->setPosition(ccp(winSize.width / 2 - item->getContentSize().width / 2, winSize.height / 2 - item->getContentSize().height / 2));

	_returnItem = item;
}

void GameLayer::rCallback(CCObject*)
{
	addChild(QuitDialog::create(false));
}

//增加道具
void GameLayer::addProp()
{
	//r = Prop::SUM_TYPE * 10 * CCRANDOM_0_1();//随机产生道具，cocos底层也是用的rand()
	int r = rand() % 6;//随机产生道具

	//kill道具  是 25分之1的机会  其他的都是5分之1 机会
	if ((Prop::TYPE)r == Prop::KILL)
	{
		r = rand() % 6;
	}
	//CCLog("-------%d---------", r);

	Prop* prop = Prop::create((Prop::TYPE)r);
	addChild(prop);

	_props->addObject(prop);

	float x = winSize.width - prop->getContentSize().width;
	x *= CCRANDOM_0_1();
	//float x = h - prop->getContentSize().width;
	float y = winSize.height - prop->getContentSize().height;
	y *= CCRANDOM_0_1();
	prop->setPosition(ccp(x + prop->getContentSize().width / 2, y + prop->getContentSize().height / 2 + winHeight - winSize.height / 2));
}

void GameLayer::eatProp(Prop::TYPE t)
{
	//enum TYPE{ LIFE, KILL, BOSS, FIRE, STOP, SUM_TYPE };
	switch (t)
	{
	case Prop::LIFE:
		eatLife();
		break;
	case Prop::KILL:
		eatKill();
		break;
	case Prop::BOSS:
		eatBoss();
		break;
	case Prop::FIRE:
		eatFire();
		break;
	case Prop::STOP:
		eatStop();
		break;
	case Prop::STRONG:
		eatStrong();
		break;
	}
}

void GameLayer::eatStrong()
{
	_isStrongFire = true;

	scheduleOnce(schedule_selector(GameLayer::StrongFire), 10);
}
void GameLayer::eatLife()
{
	_hero->setHp(_hero->getHp() + 1);

	Music::eat1();
}
void GameLayer::eatKill()
{
	int count = _ai->_tanks->count();
	for (int i = count - 1; i >= 0; i--)
	{
		Tank* tank = (Tank*)_ai->_tanks->objectAtIndex(i);
		tank->shooted();
		_ai->_tanks->removeObjectAtIndex(i);
	}
	Music::eat2();
}
void GameLayer::eatBoss()
{
	//设置砖位置
	CCPoint pts[] = {
		ccp(0, 51), ccp(1, 51), ccp(2, 51), ccp(3, 51),
		ccp(0, 52), ccp(3, 52),
		ccp(0, 53), ccp(3, 53),
		ccp(0, 54), ccp(1, 54), ccp(2, 54), ccp(3, 54),
	};
	//获取层
	CCTMXLayer* layer0 = _map->layerNamed("layer_0");
	//设置层里面的GID  把老家变成普通砖
	for (int i = 0; i < 12; i++)
	{
		layer0->setTileGID(14, pts[i]);
	}

	//设置定时器。15秒后自动还原普通砖
	scheduleOnce(schedule_selector(GameLayer::recoveryBricks),15);

	Music::eat2();
}

void GameLayer::recoveryBricks(float)
{
	CCRect rt(0, 48 * 5, 48 * 4, 48 * 4);
	if (rt.containsPoint(_hero->getPosition()))
		return;

	//设置砖位置
	CCPoint pts[] = {
		ccp(0, 51), ccp(1, 51), ccp(2, 51), ccp(3, 51),
		ccp(0, 52), ccp(3, 52),
		ccp(0, 53), ccp(3, 53),
		ccp(0, 54), ccp(1, 54), ccp(2, 54), ccp(3, 54),
	};
	//获取层
	CCTMXLayer* layer0 = _map->layerNamed("layer_0");
	//设置层里面的GID  把老家变金砖
	for (int i = 0; i < 12; i++)
	{
		layer0->setTileGID(11, pts[i]);
	}

	Music::eat1();
}

void GameLayer::eatFire()
{
	//大招2 4分之1的几率获得  大招3 20分之1的几率获得
	CCSprite* s;
	int r = rand() % 20;
	if (r == 2)
	{
		//创建大招3
		s = CCSprite::create(IMAGE_bigFire3);
		_bigFires3->addObject(s);
		/*加个判断就是为了防止就修改器修改*/
		if (getFire3() == _canBigFire3)
			setFire3(++_canBigFire3);
	}
	else if (r % 4 == 0)
	{
		//创建大招2
		s = CCSprite::create(IMAGE_bigFire2);
		_bigFires2->addObject(s);
		if (getFire2() == _canBigFire2)
			setFire2(++_canBigFire2);
	}
	else
	{
		//创建大招1
		s = CCSprite::create(IMAGE_bigFire);
		_bigFires1->addObject(s);
		if (getFire1() == _canBigFire1)
			setFire1(++_canBigFire1);
	}

	Music::eat2();
}
void GameLayer::eatStop()
{
	int count = _ai->_tanks->count();
	for (int i = count - 1; i >= 0; i--)
	{
		Tank* tank = (Tank*)_ai->_tanks->objectAtIndex(i);
		tank->setDir(Tank::NONE);
	}

	_ai->unschedule(schedule_selector(AI::changeDir));
	scheduleOnce(schedule_selector(GameLayer::continueMove),5);

	Music::eat1();
}

void GameLayer::StrongFire(float)
{
	_isStrongFire = false;
}

void GameLayer::initshowItem()
{
	CCDictionary* dict = CCDictionary::createWithContentsOfFile(XML_levelItem);
	const CCString* str = dict->valueForKey("blood");
	_blood = CCLabelTTF::create(Util::format(_hero->getHp(), str->getCString()), "Arial", 40);
	addChild(_blood);

	str = dict->valueForKey("bigFire");
	_bigFireItem = CCLabelTTF::create(Util::format(_canBigFire1, str->getCString()), "Arial", 40);
	addChild(_bigFireItem);

	str = dict->valueForKey("bigFire1");
	_bigFireItem1 = CCLabelTTF::create(Util::format(0, str->getCString()), "Arial", 40);
	addChild(_bigFireItem1);

	str = dict->valueForKey("bigFire2");
	_bigFireItem2 = CCLabelTTF::create(Util::format(0, str->getCString()), "Arial", 40);
	addChild(_bigFireItem2);

	str = dict->valueForKey("killedTank");
	_killedTankItem = CCLabelTTF::create(Util::format(0, str->getCString()), "Arial", 40);
	addChild(_killedTankItem);

	_blood->setAnchorPoint(ccp(0, 1));
	_blood->setPosition(ccp(10, winSize.height));
	_blood->enableStroke(ccc3(0, 0, 255), 5);

	_killedTankItem->setAnchorPoint(ccp(0, 1));
	_killedTankItem->setPosition(ccp(10, winSize.height - 40));
	_killedTankItem->enableStroke(ccc3(0, 0, 255), 5);

	_bigFireItem->setAnchorPoint(ccp(0, 1));
	_bigFireItem->setPosition(ccp(10, winSize.height - 80));
	_bigFireItem->enableStroke(ccc3(0, 0, 255), 5);

	_bigFireItem1->setAnchorPoint(ccp(0, 1));
	_bigFireItem1->setPosition(ccp(10, winSize.height - 120));
	_bigFireItem1->enableStroke(ccc3(0, 0, 255), 5);

	_bigFireItem2->setAnchorPoint(ccp(0, 1));
	_bigFireItem2->setPosition(ccp(10, winSize.height - 160));
	_bigFireItem2->enableStroke(ccc3(0, 0, 255), 5);
}

void GameLayer::showItem()
{
	//这里不知道为什么，必须重新加载xml文件。。不然中文根本存不下来。。每次都得重新加载。。感觉挺浪费的。。或许我可以不用TTF。。懒得弄了
	CCDictionary* dict = CCDictionary::createWithContentsOfFile(XML_levelItem);
	const CCString* str = dict->valueForKey("blood");
	_blood->setString(Util::format(_hero->getHp(), str->getCString()));
	
	str = dict->valueForKey("bigFire");
	_bigFireItem->setString(Util::format(_canBigFire1, str->getCString()));

	str = dict->valueForKey("bigFire1");
	_bigFireItem1->setString(Util::format(_canBigFire2, str->getCString()));

	str = dict->valueForKey("bigFire2");
	_bigFireItem2->setString(Util::format(_canBigFire3, str->getCString()));

	str = dict->valueForKey("killedTank");
	if (_needKilled < 0)
		_needKilled = 0;
	_killedTankItem->setString(Util::format(_needKilled, str->getCString()));
}

void GameLayer::continueMove(float)
{
	int count = _ai->_tanks->count();
	for (int i = count - 1; i >= 0; i--)
	{
		Tank::DIR dir = (Tank::DIR)(int)(Tank::NONE * CCRANDOM_0_1());//无法从float转化为Tank::dir   所以加了一个int
		Tank* tank = (Tank*)_ai->_tanks->objectAtIndex(i);
		tank->setDir(dir);
	}
	_ai->schedule(schedule_selector(AI::changeDir),2);
}

void GameLayer::shootBigFire1()
{
	/*为了防止修改器修改_canBigFire的值，所以从磁盘读取数据检验*/
	if (getFire1() != _canBigFire1)
	{
		setFire1(0);
		setFire2(0);
		setFire3(0);
		exit(0);
	}

	//_bigFireCount1代表场上的大招  数量
	CCSprite* s = (CCSprite*)_bigFires1->objectAtIndex(_bigFireCount1++);//加_bigFireCount1这个的原因是为了解决，不能同时发两个大招
	addChild(s);
	s->setPosition(_hero->getPosition());

	//还可以按的大招量  减少一个
	setFire1(--_canBigFire1);

	//让大招动起来
	Tank::DIR dir = _hero->getDir();
	CCMoveBy* move;
	switch (dir)
	{
	case Tank::U:
		move = CCMoveBy::create(8, ccp(0, winSize.width));
		break;
	case Tank::R:
		move = CCMoveBy::create(8, ccp(winSize.width, 0));
		break;
	case Tank::D:
		move = CCMoveBy::create(8, ccp(0, -winSize.width));
		break;
	case Tank::L:
		move = CCMoveBy::create(8, ccp(-winSize.width, 0));
		break;
	default:
		break;
	}

	//让大招旋转
	CCRotateBy* rotate = CCRotateBy::create(8, 360);

	//让大招闪烁
	CCBlink* blink = CCBlink::create(6, 5);//下面没有加闪烁

	//让一个精灵。同时做多个动作
	CCSpawn* action = CCSpawn::create(move, rotate,NULL);

	//因为要做碰撞检测，不能在这里从大招弹夹里面删除，也不能从检测大招的时候删除，因为大招只有在停止的时候才删除
	CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(GameLayer::removeBigFire1));

	//执行完动作，自动删除自己
	s->runAction(CCSequence::create(action, callFunc, CCRemoveSelf::create(), NULL));
}

void GameLayer::shootBigFire2()
{
	/*为了防止修改器修改_canBigFire的值，所以从磁盘读取数据检验*/
	if (getFire2() != _canBigFire2)
	{
		setFire1(0);
		setFire2(0);
		setFire3(0);
		exit(0);
	}


	CCSprite* s = (CCSprite*)_bigFires2->objectAtIndex(_bigFireCount2++);
	addChild(s);
	s->setScale(2);

	setFire2(--_canBigFire2);

	s->setPosition(ccp(_hero->getPositionX(), winHeight + 30));
	_sL->setPosition(ccp(_hero->getPositionX() - 400, winHeight + 30));
	_sR->setPosition(ccp(_hero->getPositionX() + 400, winHeight + 30));
	_sL->setVisible(true);
	_sR->setVisible(true);


	CCArray* arr = CCArray::create();
	CCTexture2D* text;
	CCSpriteFrame* frame;
	for (int i = 0; i <= 5; i++)
	{
		text = CCTextureCache::sharedTextureCache()->addImage(Util::format(i, "tank/bullet/", ".png"));
		frame = CCSpriteFrame::createWithTexture(text, CCRect(0, 0, 150, 600));
		arr->addObject(frame);
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(arr, 0.1f);
	CCAnimate* animate1 = CCAnimate::create(animation);
	CCAnimate* animate2 = CCAnimate::create(animation);
	CCAnimate* animate3 = CCAnimate::create(animation);

	CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(GameLayer::removeBigFire2));
	CCCallFunc* callFunc2 = CCCallFunc::create(this, callfunc_selector(GameLayer::setVisiable));

	s->runAction(CCSequence::create(animate1, callFunc, CCRemoveSelf::create(), NULL));
	_sL->runAction(CCSequence::create(animate2, callFunc2,NULL));
	_sR->runAction(CCSequence::create(animate3, callFunc2, NULL));
}
void GameLayer::shootBigFire3()
{
	if (getFire3() != _canBigFire3)
	{
		setFire1(0);
		setFire2(0);
		setFire3(0);
		exit(0);
	}


	//_bigFireCount1代表场上的大招  数量
	CCSprite* s = (CCSprite*)_bigFires3->objectAtIndex(_bigFireCount3++);//加_bigFireCount1这个的原因是为了解决，不能同时发两个大招
	addChild(s);
	s->setScale(3);

	//还可以按的大招量  减少一个
	setFire3(--_canBigFire3);

	s->setPosition(ccp(winSize.width / 2, winHeight + 20));

	CCArray* arr = CCArray::create();
	CCTexture2D* text;
	CCSpriteFrame* frame;
	for (int i = 1; i <= 11; i++)
	{
		text = CCTextureCache::sharedTextureCache()->addImage(Util::format(i, "tank/bullet/1006-", ".png"));
		frame = CCSpriteFrame::createWithTexture(text, CCRect(0, 0, 540, 450));
		arr->addObject(frame);
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(arr, 0.15f);
	CCAnimate* animate = CCAnimate::create(animation);

	CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(GameLayer::removeBigFire3));

	s->runAction(CCSequence::create(animate, callFunc, CCRemoveSelf::create(), NULL));
}
void GameLayer::removeBigFire1()
{
	_bigFires1->removeObjectAtIndex(0);//每次清除的都是最先加入进来的  大招
	_bigFireCount1--;
}

void GameLayer::setVisiable()
{
	_sL->setVisible(false);
	_sR->setVisible(false);
}

void GameLayer::removeBigFire2()
{
	_bigFires2->removeObjectAtIndex(0);//每次清除的都是最先加入进来的  大招
	_bigFireCount2--;
}

void GameLayer::removeBigFire3()
{
	_bigFires3->removeObjectAtIndex(0);//每次清除的都是最先加入进来的  大招
	_bigFireCount3--;
}

void GameLayer::initWife()
{
	_wife = CCSprite::create(IMAGE_wife);
	addChild(_wife);
	_wife->setPosition(ccp(48*2, 48*7));
	_wife->setTag(1314);

	_smallWife = CCSprite::create(IMAGE_wife);
	addChild(_smallWife);
	_smallWife->setScale(0.4);
	_smallWife->setVisible(false);
}

void GameLayer::wifeRun()
{
	if (_wife!=NULL && _wife->boundingBox().intersectsRect(_hero->boundingBox()))
	{
		CCJumpTo* jump = CCJumpTo::create(0.3f, _hero->getPosition(), 30, 1);
		_wife->runAction(CCSequence::create(jump, CCRemoveSelf::create(), NULL));
		_wife = NULL;
		//_b2->setVisible(true);

		Music::eat2();
	}
}

void GameLayer::checkMoveScream()
{
	//这个winSize.height是不会变的。。而我们需要这个类整体向下移动  或向上移动
	
	int curHeight = _hero->getPositionY();
	//static float winHeight = winSize.height / 2;//当前窗口的一半   我们用它来计数，看是否需要移动

	//从下面这个判断可知，从上，从下，移动，可以合并成一个，，代码是一样的
	//从视觉上，窗口向上移动
	//if (winHeight + winSize.height / 2  <= _map->getContentSize().height && curHeight > winHeight)//可以看到的最高地方，不能超过这个TiledMap
	//{
	//	setPosition(getPosition() - ccp(0, curHeight - winHeight));

	//	winHeight = curHeight;
	//}
	//从视觉上，窗口向下移动
	//else if (winHeight > winSize.height / 2 && curHeight < winHeight)
	//{
	//	setPosition(getPosition() - ccp(0, curHeight - winHeight));
	//	winHeight = curHeight;
	//}
	if (winHeight + winSize.height / 2 <= _map->getContentSize().height && curHeight > winHeight || winHeight > winSize.height / 2 && curHeight < winHeight)//可以看到的最高地方，不能超过这个TiledMap
	{
		setPosition(getPosition() - ccp(0, curHeight - winHeight));

		//移动按钮  上下左右
		if (!_isFullScreamTouch)
			for (int i = 0; i < 4; i++)
			{
				CCMenuItem* item = (CCMenuItem*)_menu->getChildByTag(1000 + i);
				item->setPosition(item->getPosition() + ccp(0, curHeight - winHeight));
			}

		//子弹按钮
		_b1->setPosition(_b1->getPosition() + ccp(0, curHeight - winHeight));
		_b2->setPosition(_b2->getPosition() + ccp(0, curHeight - winHeight));
		_b3->setPosition(_b3->getPosition() + ccp(0, curHeight - winHeight));
		_fireitem->setPosition(_fireitem->getPosition() + ccp(0, curHeight - winHeight));

		//还有CCLabelTTF
		_blood->setPosition(_blood->getPosition() + ccp(0, curHeight - winHeight));
		_bigFireItem->setPosition(_bigFireItem->getPosition() + ccp(0, curHeight - winHeight));
		_bigFireItem1->setPosition(_bigFireItem1->getPosition() + ccp(0, curHeight - winHeight));
		_bigFireItem2->setPosition(_bigFireItem2->getPosition() + ccp(0, curHeight - winHeight));
		_killedTankItem->setPosition(_killedTankItem->getPosition() + ccp(0, curHeight - winHeight));

		//返回按钮
		_returnItem->setPosition(_returnItem->getPosition() + ccp(0, curHeight - winHeight));

		winHeight = curHeight;
	}
}


void GameLayer::fullScreamTouch()
{
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
}
void GameLayer::nofullScreamTouch()
{
	//做表
	const char* panelFiles[] = {
		IMAGE_UP_1,
		IMAGE_UP_2,
		IMAGE_RIGHT_1,
		IMAGE_RIGHT_2,
		IMAGE_DOWN_1,
		IMAGE_DOWN_2,
		IMAGE_LEFT_1,
		IMAGE_LEFT_2
	};
	//CCMenu* menu = CCMenu::create();  //这个就不行
	Panel* menu = Panel::create();		//要用这个改过的 CCMenu
	addChild(menu);
	_menu = menu;

	//加入方向按钮
	for (size_t i = 0; i < sizeof(panelFiles) / sizeof(*panelFiles); i += 2)
	{
		CCMenuItemImage* item = CCMenuItemImage::create(panelFiles[i], panelFiles[i + 1]);
		item->setTag(1000 + i / 2);
		item->setTarget(this, menu_selector(GameLayer::DirCallback));
		item->setScale(2.5);
		menu->addChild(item);
	}

	//设置这四个按钮的位置
	CCPoint center = ccp(-700, -285);
	float off = 145;//距离中点

	CCMenuItem* item;

	item = (CCMenuItem*)menu->getChildByTag(1000);
	item->setPosition(center + ccp(0, off));

	item = (CCMenuItem*)menu->getChildByTag(1000 + 1);
	item->setPosition(center + ccp(off, 0));

	item = (CCMenuItem*)menu->getChildByTag(1000 + 2);
	item->setPosition(center + ccp(0, -off));

	item = (CCMenuItem*)menu->getChildByTag(1000 + 3);
	item->setPosition(center + ccp(-off, 0));
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}
void GameLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint move = pTouch->getDelta();
	float x = move.x;
	float y = move.y;

	float xx = x;
	float yy = y;

	if (xx < 0)
		xx = -xx;
	if (yy < 0)
		yy = -yy;

	if (xx < 3 && yy < 3)
		return;

	if (xx > yy)//这样我们认为是X轴移动，，因为我们只需要上下左右
	{
		if (x > 0)
			_heroDir = Tank::R;
		else
			_heroDir = Tank::L;

	}
	else//这样我们认为是Y轴移动
	{
		if (y > 0)
			_heroDir = Tank::U;
		else
			_heroDir = Tank::D;
	}
}

void  GameLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	//按下时的位置
	CCPoint start = pTouch->getStartLocation();
	//松开时的位置
	CCPoint end = pTouch->getLocation();//因为传过来的就是  鼠标离开时的touch

	// 两个点的距离小于5，那么我们认为用户在点击
	if (start.getDistanceSq(end) <= 25)
		_heroDir = Tank::NONE;
}