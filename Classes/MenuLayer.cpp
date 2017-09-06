#include "MenuLayer.h"
#include "GameLayer.h"
#include "StartLayer.h"
#include "BigFire.h"
#include "PayLayer.h"

int MenuLayer::_mapIndex = 0;//第一个场景的默认值

bool MenuLayer::init()
{
	CCLayer::init();

	//这里必须再次设置为0当别人玩了10关，退出来选第一关进去_mapIndex是不会被改变的
	_mapIndex = 0;

	//背景
	Util::addBackageground(IMAGE_levelBackground,this);

	//增加跳动的精灵
	initJumpSprite();

	//增加一个闪电
	initLightning();

	//增加一个返回按钮
	initReturn();

	//创建选择关卡Item
	initSwitchLevel();

	//检查按钮
	initCheck();

	//设置提示
	initcue();

	//应用商店
	initStore();

	return true;
}

void MenuLayer::initLightning()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	menu->addChild(BigFire::create(0, false));
	menu->addChild(BigFire::create(1, false));
}

void MenuLayer::initcue()
{
	CCDictionary* dict = CCDictionary::createWithContentsOfFile(XML_levelItem);
	const CCString* str1 = dict->valueForKey("cue1");
	const CCString* str2 = dict->valueForKey("cue2");
	CCLabelTTF* ttf = CCLabelTTF::create(Util::format(getLevel()-1, str1->getCString(), str2->getCString()), "Arial", 60);
	addChild(ttf);
	ttf->setAnchorPoint(ccp(1, 1));
	ttf->setColor(ccc3(255, 0, 0));
	ttf->setPosition(ccp(winSize.width, winSize.height));
}
//Item的回调函数  
void MenuLayer::Toggle(CCObject* sender)
{
	CCMenuItemToggle* itemToggle = (CCMenuItemToggle*)sender;
	_mapIndex = itemToggle->getSelectedIndex();//获取选的  第几个关卡  从0开始的
}
void MenuLayer::Go(CCObject*)
{
	//先检查是否可以创建这个场景
	if (getLevel() >= _mapIndex + 1)//允许的Level必须大于等于_mapIndex + 1   因为 从0开始的
	{
		CCMenu* menu = CCMenu::create();
		addChild(menu);
		CCMenuItem* noquanItem = CCMenuItemImage::create(IMAGE_anniuTouch, IMAGE_blankItem);
		CCMenuItem* quanItem = CCMenuItemImage::create(IMAGE_quanTouch, IMAGE_blankItem);
		menu->addChild(noquanItem);
		menu->addChild(quanItem);

		menu->alignItemsVerticallyWithPadding(50);


		quanItem->setTarget(this, menu_selector(MenuLayer::quanTouchGame));
		noquanItem->setTarget(this, menu_selector(MenuLayer::noquanTouchGame));
	}
	else
		_item->setVisible(true);
}

void MenuLayer::quanTouchGame(CCObject*)
{
	changeScene(GameLayer, _mapIndex + 1, true);
}
void MenuLayer::noquanTouchGame(CCObject*)
{
	changeScene(GameLayer, _mapIndex + 1, false);
}

void MenuLayer::initCheck()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	_item = CCMenuItemImage::create(IMAGE_tishi, IMAGE_tishi2);
	menu->addChild(_item);
	_item->setTarget(this, menu_selector(MenuLayer::checkCallback));
	_item->setVisible(false);
}

void MenuLayer::checkCallback(CCObject* sender)
{
	_item->setVisible(false);
}

void MenuLayer::initReturn()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	CCMenuItem* item = CCMenuItemImage::create(IMAGE_back_normal_white, IMAGE_back_press_white);
	menu->addChild(item);
	item->setTarget(this, menu_selector(MenuLayer::rCallback));
	item->setPosition(ccp(winSize.width / 2 - item->getContentSize().width / 2, -winSize.height / 2 + item->getContentSize().height / 2));
}
void MenuLayer::rCallback(CCObject*)
{
	changeScene(StartLayer,false);
}

void MenuLayer::initSwitchLevel()
{
	//获取xml中的文件 createWithContentsOfFile  
	CCDictionary* dict = CCDictionary::createWithContentsOfFile(XML_levelItem);//创建字典 Dictionary字典

	//通过键值，获取内容  
	const CCString* string = dict->valueForKey("item");//键值也是const char*
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	CCArray* arr = CCArray::create();
	for (int i = 1; i <= 20; i++)
	{
		char* value = Util::format(i, string->getCString());
		CCMenuItemFont* subItem = CCMenuItemFont::create(value);//默认字体32
		/*
		这里是一个重点，最开始用  setFontSize（）结果每次第一个Item字体大小总是32，等到第二个字体，大小才会变
		开始还是为是Toggle出了问题，后来看了CCMenuItemFont源码才发现
		set default font size		是设置默认字体大小
		static void setFontSize(unsigned int s);  而且还是静态的，可想而知，这一次创建的字体肯定是32，下一次默认字体才会改变
		setFontSizeObj（）这个只是改变当前字体，没有改变默认字体
		*/
		subItem->setFontSizeObj(160);//32*5

		subItem->setColor(ccc3(255, 0, 0));
		//subItem->setPosition(ccp(0, 300));//这句话不顶用啊。。为啥，，是因为没有直接加入menu里面，而是加入数组中？

		arr->addObject(subItem);//在数组里面装入  Item
	}
	CCMenuItemToggle* itemToggle = CCMenuItemToggle::createWithTarget(this, menu_selector(MenuLayer::Toggle), arr);
	menu->addChild(itemToggle);

	//创建进去下一个场景的item
	menu->addChild(
		CCMenuItemImage::create(IMAGE_go, IMAGE_go2, this, menu_selector(MenuLayer::Go))
		);

	menu->alignItemsVerticallyWithPadding(50);
}

void MenuLayer::initJumpSprite()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	for (int i = 1; i <= 5; i++)
	{
		/*CCSprite* s = CCSprite::create(Util::format(i, "switchWord", ".png"));
		addChild(s);*/
		//CCMoveTo* move = CCMoveTo::create(1, ccp(winSize.width / 2 - 260 * 3 + i * 260, winSize.height - 160));
		//s->runAction(CCSequence::create(move, CCRotateBy::create(1, 2 * 360), NULL));

		CCMenuItem* item = CCMenuItemImage::create(Util::format(i, IMAGE_switchWord, ".png"), Util::format(i, IMAGE_switchWord, ".png"));
		menu->addChild(item);
		item->setTarget(this, menu_selector(MenuLayer::rotateItem));
		item->setPosition(ccp(-winSize.width / 2, -winSize.height / 2));

		CCMoveTo* move = CCMoveTo::create(1, ccp(- 260 * 3 + i * 260, winSize.height/2 - 160));

		item->runAction(CCSequence::create(move, CCRotateBy::create(1, 2 * 360), NULL));
	}
}

void MenuLayer::initStore()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	CCMenuItem* item = CCMenuItemImage::create(IMAGE_store1, IMAGE_store2);
	menu->addChild(item);
	item->setPosition(ccp(0, -winSize.height / 3));
	item->setTarget(this, menu_selector(MenuLayer::AppStore));
	item->setScale(1.2);
}

void MenuLayer::rotateItem(CCObject* sender)
{
	CCMenuItem* item = (CCMenuItem*)sender;
	item->runAction(CCRotateBy::create(1, 360));
}

void MenuLayer::AppStore(CCObject* sender)
{
	changeScene(PayLayer);
}