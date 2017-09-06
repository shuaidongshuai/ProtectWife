#ifndef __PROP_H__
#define __PROP_H__

#include "Common.h"

class Prop:public CCSprite
{
public:
	enum TYPE{ LIFE, KILL, BOSS, FIRE, STOP, STRONG, SUM_TYPE };
	CC_SYNTHESIZE(TYPE, _type, Type);
	static Prop* create(TYPE t)
	{
		Prop* p = new Prop;
		
		const char* files[] = {
			IMAGE_life,
			IMAGE_sha,
			IMAGE_boss,
			IMAGE_addfire,
			IMAGE_stop,
			IMAGE_strongFire
		};

		p->setType(t);
		p->initWithFile(files[t]);
		p->autorelease();

		// 启动定时器，定时不吃掉，道具消失
		p->scheduleOnce(schedule_selector(Prop::killSelf), 10);

		return p;
	}
	void killSelf(float);

};

#endif