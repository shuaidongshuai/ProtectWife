#include "StartLayer.h"
#include "MenuLayer.h"
#include "Music.h"
#include "QuitDialog.h"
#include "GameAbout.h"

bool StartLayer::init(bool isFirst)
{
	CCLayer::init();

	//背景图片
	Util::addBackageground(IMAGE_StartScene,this);

	//增加按钮
	addMenuItem();

	//缓存音乐
	if (isFirst)
	{
		Music* music = Music::create();//只用缓冲一次
		Music::StartLayerMusic();//只用开启一次
	}

	//随机数种子
	srand(time(NULL));

	return true;
}
void StartLayer::addMenuItem()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);

	CCMenuItem* Item1 = CCMenuItemImage::create(IMAGE_GameStartItem, IMAGE_GameStartItem2);
	CCMenuItem* Item2 = CCMenuItemImage::create(IMAGE_GameExit, IMAGE_GameExit2);
	CCMenuItem* Item3 = CCMenuItemImage::create(IMAGE_GameIntroduce, IMAGE_GameIntroduce2);

	menu->addChild(Item1);
	menu->addChild(Item3);
	menu->addChild(Item2);
	
	//menu->alignItemsVertically();//没有间距
	menu->alignItemsVerticallyWithPadding(3);//间距8
	//menu->setPosition(ccp(winSize.width / 2 - 80, winSize.height / 2 - 80));


	//设置回调函数
	Item1->setTarget(this, menu_selector(StartLayer::enterNextLayer));
	Item2->setTarget(this, menu_selector(StartLayer::exitGame));
	Item3->setTarget(this, menu_selector(StartLayer::introduce));

	CCPoint old[3];
	old[0] = Item1->getPosition();
	old[1] = Item2->getPosition();
	old[2] = Item3->getPosition();

	Item1->setPosition(old[0] + ccp(0, winSize.height / 2));
	Item2->setPosition(old[1] + ccp(0, -winSize.height / 2));
	Item3->setPosition(old[2] + ccp(-winSize.height / 2,0));

	CCEaseElasticOut* bounce1 = CCEaseElasticOut::create(CCMoveTo::create(1.2, old[0]));
	CCEaseElasticOut* bounce2 = CCEaseElasticOut::create(CCMoveTo::create(1.2, old[1]));
	CCEaseElasticOut* bounce3 = CCEaseElasticOut::create(CCMoveTo::create(1.2, old[2]));
	CCSpawn* spawn1 = CCSpawn::create(bounce1, CCRotateBy::create(1.2, 720), NULL);
	CCSpawn* spawn2 = CCSpawn::create(bounce2, CCRotateBy::create(1.2, 720), NULL);
	CCSpawn* spawn3 = CCSpawn::create(bounce3, CCRotateBy::create(1.2, 720), NULL);

	//动起来
	Item1->runAction(spawn1);	
	Item2->runAction(spawn2);
	Item3->runAction(spawn3);


	//小人按钮
	CCMenuItem* boy = CCMenuItemImage::create(IMAGE_StartPeople, IMAGE_StartPeople);
	menu->addChild(boy);
	CCMenuItem* girl = CCMenuItemImage::create(IMAGE_StartPeople2, IMAGE_StartPeople2);
	menu->addChild(girl);

	boy->setScale(1.7);
	girl->setScale(1.7);

	boy->setTarget(this, menu_selector(StartLayer::gameAbout1));
	girl->setTarget(this, menu_selector(StartLayer::gameAbout2));

	//动起来
	CCEaseElasticOut* ease = CCEaseElasticOut::create(CCMoveTo::create(1, ccp(-winSize.width/4 + 40,0)));
	CCBlink* blink = CCBlink::create(0.5, 4);
	boy->runAction(CCSequence::create(ease, blink, NULL));
	ease = CCEaseElasticOut::create(CCMoveTo::create(2, ccp(-2 * winSize.width / 5 + 20, 0)));
	blink = CCBlink::create(0.5, 4);
	girl->runAction(CCSequence::create(ease, blink, NULL));

}

void StartLayer::enterNextLayer(CCObject*)
{
	changeScene(MenuLayer);
}
void StartLayer::exitGame(CCObject*)
{
	addChild(QuitDialog::create(true));
}
void StartLayer::introduce(CCObject*)
{
	changeScene(GameAbout, true,true);
}


void  StartLayer::gameAbout1(CCObject* sender)
{
	CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(StartLayer::goA));
	CCMenuItem* item = (CCMenuItem*)sender;
	item->runAction(CCSequence::create(CCMoveBy::create(0.6f, ccp(0, winSize.height / 2 + item->getContentSize().height)), func, NULL));
}
void  StartLayer::gameAbout2(CCObject* sender)
{
	CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(StartLayer::goB));
	CCMenuItem* item = (CCMenuItem*)sender;
	item->runAction(CCSequence::create(CCMoveBy::create(0.6f, ccp(0, -winSize.height / 2 - item->getContentSize().height)), func, NULL));

}

void StartLayer::goA()
{
	changeScene(GameAbout, true,false);
}
void StartLayer::goB()
{
	changeScene(GameAbout, false, false);
}