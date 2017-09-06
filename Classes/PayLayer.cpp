#include "PayLayer.h"
#include "MenuLayer.h"
#include "GameLayer.h"

#if defined(ANDROID)  
#include <jni.h> 
#include "platform/android/jni/JniHelper.h"  
#endif  

PayLayer *PayLayer::payInstance;

bool PayLayer::init()
{
	CCLayer::init();

	/*的到当前对象实例*/
	payInstance = this;

	//现在啥都还没买
	fireNum = 0;
	price = 0;

	//背景
	Util::addBackageground(IMAGE_GameAboutBackground, this);

	//增加一个返回按钮
	initReturn();

	//下面给出几个可选的支付
	CCSprite *s = CCSprite::create(IMAGE_payTitle);
	addChild(s);
	s->setPosition(ccp(winSize.width / 2 - 10, winSize.height - s->getContentSize().height - 50));
	s->setScale(2.5);

	s = CCSprite::create(IMAGE_titlePayFire1);
	addChild(s);
	s->setScale(1.2);
	s->setPosition(ccp(s->getContentSize().width / 2 + 60, winSize.height - 3 * s->getContentSize().height));

	s = CCSprite::create(IMAGE_titlePayFire2);
	addChild(s);
	s->setPosition(ccp(s->getContentSize().width / 2 + 60, winSize.height - 5 * s->getContentSize().height));
	s->setScale(1.2);

	s = CCSprite::create(IMAGE_titlePayFire3);
	addChild(s);
	s->setPosition(ccp(s->getContentSize().width / 2 + 60, winSize.height - 7 * s->getContentSize().height));
	s->setScale(1.2);

	//item
	CCMenu *menu = CCMenu::create();
	addChild(menu);

	CCMenuItem* item1_1 = CCMenuItemImage::create(IMAGE_payFire1_1, IMAGE_blankItem);
	CCMenuItem* item1_2 = CCMenuItemImage::create(IMAGE_payFire1_2, IMAGE_blankItem);
	CCMenuItem* item1_3 = CCMenuItemImage::create(IMAGE_payFire1_3, IMAGE_blankItem);
	CCMenuItem* item2_1 = CCMenuItemImage::create(IMAGE_payFire2_1, IMAGE_blankItem);
	CCMenuItem* item2_2 = CCMenuItemImage::create(IMAGE_payFire2_2, IMAGE_blankItem);
	CCMenuItem* item2_3 = CCMenuItemImage::create(IMAGE_payFire2_3, IMAGE_blankItem);
	CCMenuItem* item3_1 = CCMenuItemImage::create(IMAGE_payFire3_1, IMAGE_blankItem);
	CCMenuItem* item3_2 = CCMenuItemImage::create(IMAGE_payFire3_2, IMAGE_blankItem);
	CCMenuItem* item3_3 = CCMenuItemImage::create(IMAGE_payFire3_3, IMAGE_blankItem);
	
	
	item1_1->setPosition(ccp(s->getContentSize().width / 2 + 3 * s->getContentSize().width / 2 - winSize.width / 2, winSize.height / 2 - 3 * s->getContentSize().height));
	item1_2->setPosition(ccp(s->getContentSize().width / 2 + 5 * s->getContentSize().width / 2 + 115 - winSize.width / 2, winSize.height / 2 - 3 * s->getContentSize().height));
	item1_3->setPosition(ccp(s->getContentSize().width / 2 + 8 * s->getContentSize().width / 2 + 60 - winSize.width / 2, winSize.height / 2 - 3 * s->getContentSize().height - 2));
	
	item2_1->setPosition(ccp(s->getContentSize().width / 2 + 3 * s->getContentSize().width / 2 - winSize.width / 2, winSize.height / 2 - 5 * s->getContentSize().height));
	item2_2->setPosition(ccp(s->getContentSize().width / 2 + 5 * s->getContentSize().width / 2 + 115 - winSize.width / 2, winSize.height / 2 - 5 * s->getContentSize().height));
	item2_3->setPosition(ccp(s->getContentSize().width / 2 + 8 * s->getContentSize().width / 2 + 60 - winSize.width / 2, winSize.height / 2 - 5 * s->getContentSize().height - 2));

	item3_1->setPosition(ccp(s->getContentSize().width / 2 + 3 * s->getContentSize().width / 2 - winSize.width / 2, winSize.height / 2 - 7 * s->getContentSize().height));
	item3_2->setPosition(ccp(s->getContentSize().width / 2 + 5 * s->getContentSize().width / 2 + 115 - winSize.width / 2, winSize.height / 2 - 7 * s->getContentSize().height));
	item3_3->setPosition(ccp(s->getContentSize().width / 2 + 8 * s->getContentSize().width / 2 + 60 - winSize.width / 2, winSize.height / 2 - 7 * s->getContentSize().height - 2));

	item1_1->setScale(0.8);
	item1_2->setScale(0.8);
	item1_3->setScale(0.8);

	item2_1->setScale(0.8);
	item2_2->setScale(0.8);
	item2_3->setScale(0.8);

	item3_1->setScale(0.8);
	item3_2->setScale(0.8);
	item3_3->setScale(0.8);

	item1_1->setTarget(this, menu_selector(PayLayer::judgePayLayer));
	item1_2->setTarget(this, menu_selector(PayLayer::judgePayLayer));
	item1_3->setTarget(this, menu_selector(PayLayer::judgePayLayer));

	item2_1->setTarget(this, menu_selector(PayLayer::judgePayLayer));
	item2_2->setTarget(this, menu_selector(PayLayer::judgePayLayer));
	item2_3->setTarget(this, menu_selector(PayLayer::judgePayLayer));

	item3_1->setTarget(this, menu_selector(PayLayer::judgePayLayer));
	item3_2->setTarget(this, menu_selector(PayLayer::judgePayLayer));
	item3_3->setTarget(this, menu_selector(PayLayer::judgePayLayer));

	item1_1->setTag(1);
	item1_2->setTag(2);
	item1_3->setTag(3);

	item2_1->setTag(4);
	item2_2->setTag(5);
	item2_3->setTag(6);

	item3_1->setTag(7);
	item3_2->setTag(8);
	item3_3->setTag(9);
	
	menu->addChild(item1_1);
	menu->addChild(item1_2);
	menu->addChild(item1_3);
	menu->addChild(item2_1);
	menu->addChild(item2_2);
	menu->addChild(item2_3);
	menu->addChild(item3_1);
	menu->addChild(item3_2);
	menu->addChild(item3_3);

	return true;
}

void PayLayer::judgePayLayer(CCObject* sender)
{
	CCMenuItem *item = (CCMenuItem *)sender;
	int tag = item->getTag();
	switch (tag)
	{
	case 1:goPayLayer(FIRE1,10, 1); break;
	case 2:goPayLayer(FIRE1,30, 2); break;
	case 3:goPayLayer(FIRE1,100, 5); break;
	case 4:goPayLayer(FIRE2, 5, 1); break;
	case 5:goPayLayer(FIRE2, 15, 2); break;
	case 6:goPayLayer(FIRE2, 50, 5); break;
	case 7:goPayLayer(FIRE3, 2, 1); break;
	case 8:goPayLayer(FIRE3, 6, 2); break;
	case 9:goPayLayer(FIRE3, 20, 5); break;
	}
}

void PayLayer::goPayLayer(fireType ft,int fireNum, int price)
{
#if defined(ANDROID)
	JniMethodInfo minfo;
	/*看是否有返回一个对象的静态方法*/
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "com/dong/ProtectWife/ProtectWife", "getInstance", "()Ljava/lang/Object;");
	if (!isHave){
		return;
	}
	/*先调用一个静态方法，这个方法返回来，这个类的静态对象*/
	jobject jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	/*然后调用这个类的非静态方法*/
	isHave = JniHelper::getMethodInfo(minfo, "com/dong/ProtectWife/ProtectWife", "payV2", "(III)V");
	if (!isHave){
		return;
	}
	jint fireType = (int)ft;
	jint jfireNum = fireNum;
	jint jprice = price;
	minfo.env->CallVoidMethod(jobj, minfo.methodID,fireType, jfireNum, jprice);
#endif  
}

void PayLayer::paySuccess(int ft, int fireNum)
{
	fireType type = (fireType)ft;
	switch (type)
	{
	case FIRE1: setFire1(getFire1() + fireNum); break;
	case FIRE2: setFire2(getFire2() + fireNum); break;
	case FIRE3: setFire3(getFire3() + fireNum); break;
	}
}

#if defined(ANDROID)
extern "C"
{
	JNIEXPORT void JNICALL Java_com_dong_ProtectWife_ProtectWife_successPayCB(JNIEnv *env, jobject obj,jint fireType,jint fireNum)
	{
		int a = fireType;
		int b = fireNum;
		PayLayer::getPayInstance()->paySuccess(a,b);
	}
}
#endif

void PayLayer::initReturn()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);
	CCMenuItem* item = CCMenuItemImage::create(IMAGE_back_normal_white, IMAGE_back_press_white);
	menu->addChild(item);
	item->setTarget(this, menu_selector(PayLayer::rCallback));
	item->setPosition(ccp(winSize.width / 2 - item->getContentSize().width / 2, winSize.height / 2 - item->getContentSize().height / 2));
}
void PayLayer::rCallback(CCObject*)
{
	changeScene(MenuLayer);
}