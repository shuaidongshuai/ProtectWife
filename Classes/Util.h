#ifndef __Util_H__
#define __Util_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define winSize CCDirector::sharedDirector()->getWinSize()
#define changeScene(__layer_name__,...) \
	CCDirector::sharedDirector()->replaceScene(Util::createScene(__layer_name__::create(__VA_ARGS__)))

class Util
{
public:
	//创建一个场景，把传过来的层，加入到这个场景里面
	static CCScene* createScene(CCLayer* layer)
	{
		CCScene* s = CCScene::create();
		s->addChild(layer);
		return s;
	}
	//创建背景图片
	static CCSprite* addBackageground(const char* picFile, CCNode* parent)
	{
		CCSprite* s = CCSprite::create(picFile);
		parent->addChild(s);
		s->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		return s;
	}
	//相对移动节点
	static void MoveNode(CCNode* node, CCPoint& delta)
	{
		node->setPosition(node->getPosition() + delta);
	}
	//拼接数字和字符串
	static char* format(int value, const char* str1 = "", const char* str2 = "")
	{
		static char buf[1024];
		sprintf(buf, "%s%d%s", str1, value, str2);
		return buf;
	}
	//判断一个 点 是否在 这个 map内
	static bool isContent(const CCPoint& pt, CCNode* node)
	{
		CCRect rt = CCRect(0, 0, node->getContentSize().width, node->getContentSize().height);//获取这个Node 的 Rect
		return rt.containsPoint(pt);//看是否包含这个点
	}
	//把openGL坐标  转化为  map坐标
	static CCPoint GL2map(const CCPoint& pt, CCTMXTiledMap* map)
	{
		int mapX = pt.x / 48;
		int mapY = map->getMapSize().height - (int)pt.y / 48 - 1;//注意强制类型转化

		return ccp(mapX, mapY);
	}
};



#endif

