#include "QuitDialog.h"
#include "MenuLayer.h"
#include "Music.h"
#include "GameLayer.h"
#include "SceneScore.h"

bool QuitDialog::init(bool isStartLayer, bool isGameOver, bool isWin,GameLayer *game)
{
	CCLayer::init();

	_isStartLayer = isStartLayer;
	_game = game;

	_menu = CCMenu::create();
	addChild(_menu);

	if (isGameOver)
	{
		if (isWin)
		{
			winGame();
		}
		else
		{
			loseGame();
		}
	}
	else
	{
		outGame();
	}
	
	return true;
}

void QuitDialog::outGame()
{
	CCMenuItem* itemYes = CCMenuItemImage::create(IMAGE_quitGame, IMAGE_quitGame2);
	CCMenuItem* itemNo = CCMenuItemImage::create(IMAGE_continueGame, IMAGE_continueGame2);

	_menu->addChild(itemYes);
	_menu->addChild(itemNo);


	if (_isStartLayer)//没正真开始游戏 winHeight 是等于0 的
	{
		itemYes->setPosition(ccp(-420, 0));
		itemNo->setPosition(ccp(420, 0));
	}
	else
	{
		itemYes->setPosition(ccp(-420, GameLayer::winHeight - winSize.height / 2));
		itemNo->setPosition(ccp(420, GameLayer::winHeight - winSize.height / 2));
	}

	itemYes->setTarget(this, menu_selector(QuitDialog::Yes));
	itemNo->setTarget(this, menu_selector(QuitDialog::No));

	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	setTouchPriority(-140);//正常的好像是-120....越小越先处理   如果不加这一句话。其他的菜单项点中了也会触发
}

void QuitDialog::winGame()
{
	CCSprite* s = CCSprite::create(IMAGE_winTitle);
	addChild(s);
	s->setPosition(ccp(winSize.width / 2, GameLayer::winHeight + winSize.height / 2 - s->getContentSize().height / 2));

	CCMenuItem* upscore = CCMenuItemImage::create(IMAGE_upScore1, IMAGE_upScore2);
	_menu->addChild(upscore);
	upscore->setTarget(this, menu_selector(QuitDialog::upScore));
	upscore->setPosition(ccp(0, GameLayer::winHeight - winSize.height / 3));
	upscore->setTag(1000);

	CCMenuItem* next = CCMenuItemImage::create(IMAGE_noLeaveName1, IMAGE_noLeaveName2);
	_menu->addChild(next);
	next->setTarget(this, menu_selector(GameLayer::win));
	next->setPosition(ccp(0, GameLayer::winHeight - winSize.height / 2 - next->getContentSize().height));
	next->setTag(1001);

	/*下面这个判断是由于全屏模式下，不能这个Touch覆盖主界面的Touch，不然box点击不了*/
	if (GameLayer::_isFullScreamTouch)
		_game->setTouchEnabled(false);
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	setTouchPriority(-140);
}
void QuitDialog::loseGame()
{
	CCMenuItem* lose = CCMenuItemImage::create(IMAGE_shuButton, IMAGE_blankItem);
	_menu->addChild(lose);

	lose->setTarget(this, menu_selector(GameLayer::lose));

	lose->setPosition(ccp(0, GameLayer::winHeight - winSize.height / 2));

	//这个一定要还原
	GameLayer::winHeight = winSize.height / 2;

	Music::gameOver();

	Music::stopBackgroundMusic();

	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	setTouchPriority(-140);//正常的好像是-120....越小越先处理   如果不加这一句话。其他的菜单项点中了也会触发
}

void QuitDialog::upScore(CCObject*)
{
	CCSprite *s = CCSprite::create(IMAGE_hintEnter);
	s->setPosition(ccp(winSize.width / 2, GameLayer::winHeight + s->getContentSize().height / 2));
	addChild(s);

	CCEditBox* box = CCEditBox::create(CCSize(900, 120), CCScale9Sprite::create(IMAGE_editBox));
	addChild(box);
	box->setPosition(ccp(winSize.width / 2, GameLayer::winHeight - box->getContentSize().height / 2));


	//增加提示
	box->setPlaceHolder("Please input your name!");
	//设置提示字体
	box->setFont("Arial", 82);
	//设置输入的字体颜色
	box->setFontColor(ccc3(255, 0, 0));
	//设置可以输入的长度
	box->setMaxLength(6);
	//设置软键盘中回车按钮的样子
	box->setReturnType(kKeyboardReturnTypeGo);

	_box = box;
	setTouchEnabled(false);//如果不取消box就点击不到了

	_menu->getChildByTag(1000)->setVisible(false);//把upscore按钮关闭
	_menu->getChildByTag(1001)->setVisible(false);//把进入下一关的按钮关闭

	CCDirector::sharedDirector()->resume();
	schedule(schedule_selector(QuitDialog::checkName));
}
void QuitDialog::checkName(float)
{
	_name = _box->getText();
	if (strcmp(_name, ""))
	{
		unschedule(schedule_selector(QuitDialog::checkName));

		//这个一定要还原
		GameLayer::winHeight = winSize.height / 2;

		changeScene(SceneScore, _game->_killedTank - 666, _name);//-666为了防止修改
	}
}

void QuitDialog::Yes(CCObject*)
{
	if (_isStartLayer)//这个是真正的退出
		exit(0);

	//这个只是退出 到 菜单项
	Music::stopBackgroundMusic();//首先的关音乐
	Music::StartLayerMusic();//再打开另一个音乐

	changeScene(MenuLayer);
}
void QuitDialog::No(CCObject*)
{
	removeFromParent();
}

bool QuitDialog::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	_menuClick = _menu->ccTouchBegan(pTouch, pEvent);//如果这个菜单项被点中，那么就处理这个菜单项
	return true;
	//return _menuClick = _menu->ccTouchBegan(pTouch, pEvent);千万不能这样写，我们利用的是这个类先处理setTouchPriority(-130);
															//如果reuan false 那么cocos 会处理下一个优先级的菜单项，达不到我们想要的效果
}
void QuitDialog::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if (_menuClick)
		_menu->ccTouchMoved(pTouch, pEvent);
}
void QuitDialog::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if (_menuClick)
		_menu->ccTouchEnded(pTouch, pEvent);

}
void QuitDialog::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	if (_menuClick)
		_menu->ccTouchCancelled(pTouch, pEvent);
}
