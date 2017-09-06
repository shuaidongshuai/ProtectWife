#include "AI.h"
#include "Tank.h"
#include "GameLayer.h"

void AI::onEnter()
{
	CCLayer::onEnter();

	_tanks = CCArray::create();
	_tanks->retain();

	_bullets = CCArray::create();
	_bullets->retain();

	scheduleUpdate();
	schedule(schedule_selector(AI::addTanks), 3.5);

	//由于现在的坦克太笨了，实在受不了了，，特意加一个转头的定时器，就不让他撞墙才转头
	schedule(schedule_selector(AI::changeDir), 2);

	_countTank = 0;
}
void AI::onExit()
{
	CCLayer::onEnter();

	_tanks->release();
	_bullets->release();
}
//用于移动坦克  检查子弹
void AI::update(float)
{
	int count = _tanks->count();
	for (int i = count - 1; i >= 0; --i)//是根据坦克数量来遍历的
	{
		//检查每辆坦克
		Tank* tank = (Tank*)_tanks->objectAtIndex(i);
		tank->moveTank(tank->getDir(),this,(Tank*)getParent()->getChildByTag(666));
		/*
		本来是用下面这段注释的，，后来发现，不能这样做，当一个坦克死后，他发射出来的子弹。我们根本不能进行碰撞检测
		所以要把  敌方子弹全部放到一个弹夹里面，换句话说就是，全都用AI的弹夹。
		但是后来，又发现，每一颗子弹都不能进行碰撞检测。经过仔细发现，每一次AI的_bullets的地址都会变，
		这就奇怪了，AI就调用一次，为什么他的地址会变呢？
		结果只有一个，AI的_bullets指针给了其中一辆Tank，而这个Tank   最开始我加了 onEnter(在里面我进行了弹夹初始化)
		本来我也想过，创建坦克就调用onEnter也不会出错啊，_bullets在Tank里面被赋一个值，再到AI中又被赋一个值，也不会出错

		结果证明：真相就是，onEnter不是在创建类的时候调用的，具体在哪调用我也没弄明白，在网上搜出这一句话：

		onEnter是属于CCLayer的方法，每次当layer显示到屏幕的时候都会调用。相反onExit每次layer离开的时候都会调用。

		最后于我去掉了，Tank里面对_bullets初始化。。就没问题了
		*/
		//检查每辆坦克的子弹
		//GameLayer* gameLayer = (GameLayer*)getMap()->getParent();//获取GameLayer这个层，，要用这个层的检查子弹函数
		//gameLayer->check(tank->_bullets, getMap());
		//gameLayer->checkTankBullets(tank->_bullets);
	}
	//不需要遍历每一个Tank的子弹。。子弹一起遍历多好的
	GameLayer* gameLayer = (GameLayer*)getMap()->getParent();//获取GameLayer这个层，，要用这个层的检查子弹函数
	gameLayer->check(_bullets);
	gameLayer->checkTankBullets(_bullets);
}
//用于增加tank
void AI::addTanks(float)
{
	//创建的坦克不能多于 200
	if (_countTank++ >= 200)
		unschedule(schedule_selector(AI::addTanks));

	Tank* tank = Tank::create(Tank::ENEMY, 1, 1, Tank::NORMAL, Tank::D);

	tank->_bullets = _bullets;

	_tanks->addObject(tank);

	getMap()->addChild(tank);//这句话必要，，因为Tank中要用到自己的父类map。。没这句话，敌人坦克就没有父类，没有map
	
	static bool bLeft = true;
	if (bLeft)
		tank->setPosition(ccp(3 * 48, (19 + _countTank % 36) * 48));
	else
		tank->setPosition(ccp(37 * 48, (19 + _countTank % 36) * 48));
	bLeft = !bLeft;
}

void AI::changeDir(float)
{
	int count = this->_tanks->count();
	for (int i = count - 1; i >= 0; i--)
	{
		Tank* tank = (Tank*)_tanks->objectAtIndex(i);
		int dir = rand() % Tank::NONE;
		tank->setDir((Tank::DIR)dir);
	}
}
