#include "Panel.h"

bool Panel::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	//判断是否被按下
	bool bTouch = CCMenu::ccTouchBegan(touch, event); //直接调用  父类的ccTouchBegan 这样我们只需要加功能就可以了。这样就不会影响CCMenu的功能了
	if (!bTouch) return false;

	//判断是否是发射子弹
	if (_isBullet)
	{
		m_pSelectedItem->activate();
	}
	else
		scheduleUpdate();//开启定时器

	return true;
}
void Panel::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	if (_isBullet)//是子弹 把按钮选中的效果给取消，变成“没有选中”  状态也要变成“等待”
	{
		if (m_pSelectedItem!=NULL)
			m_pSelectedItem->unselected();
		m_eState = kCCMenuStateWaiting;
		return;
	}

	unscheduleUpdate();
	CCMenu::ccTouchEnded(touch, event);
}
void Panel::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
	unscheduleUpdate();
	CCMenu::ccTouchCancelled(touch, event);
}

void Panel::update(float)
{
	if (!m_pSelectedItem)//不加这个判断也是一个bug   当由按钮移动到空地时，，没有任何按钮被选中，这时，就不能再m_pSelectedItem->activate();
	{																		//此时 m_pSelectedItem为NULL
		unscheduleUpdate();
		return;
	}

	//CCMenu 执行触发函数。。只需要 m_pSelectedItem->activate(); 即可
	m_pSelectedItem->activate();//m_pSelectedItem是保护成员。子类可以访问
}