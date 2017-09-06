#ifndef __SceneScore_H__
#define __SceneScore_H__

#include "Common.h"

class SceneScore : public CCLayer
{
	CCSprite* waitSprite;
public:
	static SceneScore* create(int score, const char* name)
	{
		SceneScore* s = new SceneScore();
		s->init(score, name);
		s->autorelease();
		return s;
	}
	bool init(int score, const char* name);

	void uploadScore(int score, const char* username);//上传分数
	void Response(CCHttpClient* client, CCHttpResponse* resp);//服务器回应函数

	void uploadwait();
	void uploadFalse();
};

#endif
