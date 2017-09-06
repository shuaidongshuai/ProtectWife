#ifndef __PANEL_H__
#define __PANEL_H__

#include "Common.h"

//为什么要这个类呢？
//因为cocos默认 MenuItem 只有当TouchEnd 之后才触发菜单项 函数
//目的：改变CCMenu 使他按下时就执行触发函数，直到松开

//这个类现在就是  自己的CCMenu了，，自定义了功能，，以后要这个功能就不要再CCMenu了   直接Panel* menu
//就相当于给CCMenu 改变了一个功能
class Panel:public CCMenu
{
	bool _isBullet;//判断是不是子弹按钮  按下的
public:
	static Panel* create(bool isBullet = false)
	{
		Panel* p = new Panel;
		p->_isBullet = isBullet;
		p->init();				//注意我们都是用的父类的东西。这句话千万不能少。平时都没加，这次不加就会出现MenuItem都消失
		p->autorelease();
		return p;
	}
	//CREATE_FUNC(Panel);

	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);

	void update(float);
};

#endif