#include "GameAbout.h"
#include "StartLayer.h"

bool GameAbout::init(bool isboy, bool isAbout)
{
	CCLayer::init();

	if (!isAbout)
		boyGirl(isboy);
	else
		introduce();
	

	//增加一个返回按钮
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	CCMenuItem* item = CCMenuItemImage::create(IMAGE_back_normal_white, IMAGE_back_press_white);
	menu->addChild(item);
	item->setTarget(this, menu_selector(GameAbout::rCallback));
	item->setPosition(ccp(winSize.width / 2 - item->getContentSize().width / 2, -winSize.height / 2 + item->getContentSize().height / 2));

	return true;
}
void GameAbout::rCallback(CCObject*)
{
	changeScene(StartLayer);
}

void GameAbout::introduce()
{
	Util::addBackageground(IMAGE_GameAboutBackground, this);

	CCDictionary* dict = CCDictionary::createWithContentsOfFile(XML_levelItem);
	const CCString* str;
	CCLabelTTF* ttf;
	for (int i = 1; i <= 11; i++)
	{
		str = dict->valueForKey(Util::format(i, "howToGame"));

		ttf = CCLabelTTF::create(str->getCString(), "Arial", 60);
		addChild(ttf);

		// label的锚点，除了影响setPosition，还指定了文字的对齐方式(默认居中，现在是左对齐)
		ttf->setAnchorPoint(ccp(0, 1));
		ttf->setPosition(ccp(10, winSize.height - (i-1) * 100));
		ttf->setColor(ccc3(255, 0, 0));
		ttf->enableStroke(ccc3(255, 0, 0), 5);//只在ios和android中有效，，加粗
	}
}

void GameAbout::boyGirl(bool isboy)
{
	CCDictionary* dict;
	const CCString* str[10];
	if (isboy)
	{
		Util::addBackageground(IMAGE_GameAboutBackground, this);

		dict = CCDictionary::createWithContentsOfFile(XML_levelItem);
		for (int i = 1; i <= 6; i++)
			str[i - 1] = dict->valueForKey(Util::format(i, "boy"));//通过键值，获取内容  

		for (int i = 0; i < 6; i++)
		{
			CCLabelTTF* ttf = CCLabelTTF::create(str[i]->getCString(), "Arial", 60);
			addChild(ttf);

			// label的锚点，除了影响setPosition，还指定了文字的对齐方式(默认居中，现在是左对齐)
			ttf->setAnchorPoint(ccp(0, 1));
			ttf->setPosition(ccp(10, winSize.height - i * 100 - 30));
			ttf->setColor(ccc3(255, 0, 0));
			ttf->enableStroke(ccc3(255, 0, 0), 5);//只在ios和android中有效，，加粗
		}
	}
	else
	{
		Util::addBackageground(IMAGE_GameAboutBackground, this);

		dict = CCDictionary::createWithContentsOfFile(XML_levelItem);
		for (int i = 1; i <= 7; i++)
			str[i - 1] = dict->valueForKey(Util::format(i, "girl"));//通过键值，获取内容

		for (int i = 0; i < 7; i++)
		{
			CCLabelTTF* ttf = CCLabelTTF::create(str[i]->getCString(), "Arial", 60);
			addChild(ttf);

			// label的锚点，除了影响setPosition，还指定了文字的对齐方式(默认居中，现在是左对齐)
			ttf->setAnchorPoint(ccp(0, 1));
			ttf->setPosition(ccp(10, winSize.height - i * 100 - 30));
			ttf->setColor(ccc3(255, 0, 0));
			ttf->enableStroke(ccc3(255, 0, 0), 5);//只在ios和android中有效，，加粗
		}
	}
	
}