#include "SceneScore.h"
#include "GameLayer.h"
#include "MenuLayer.h"
#include "Util.h"
#include <vector>
#include <string>

bool SceneScore::init(int score, const char* name)
{
	CCLayer::init();

	//设置一个背景图片
	Util::addBackageground(IMAGE_GameAboutBackground, this);

	//增加一个返回按钮
	CCMenu *menu = CCMenu::create();
	addChild(menu);
	CCMenuItem* item = CCMenuItemImage::create(IMAGE_back_normal_white, IMAGE_back_press_white);
	item->setPosition(ccp(winSize.width / 2 - item->getContentSize().width / 2, winSize.height / 2 - item->getContentSize().height / 2));
	item->setTarget(this, menu_selector(GameLayer::win));
	menu->addChild(item);

	//打印一条等待消息
	uploadwait();

	//把分数上传到服务器，并从服务器获得分数的前10名
	uploadScore(score, name);

	//创建15个CCLabelTTF
	CCLabelTTF* ttf = CCLabelTTF::create("Ranking List", "", 80);
	addChild(ttf);
	ttf->setPosition(ccp(winSize.width / 2, winSize.height - 50));
	ttf->setColor(ccc3(255, 0, 0));
	for (int i = 1; i <= 16; i++)
	{
		CCLabelTTF* ttf = CCLabelTTF::create("No People", "", 80);
		addChild(ttf);
		ttf->setTag(i + 1000);
		if (i % 2 == 0)
		{
			ttf->setPosition(ccp(winSize.width / 2 + winSize.width / 8, winSize.height - (i / 2 - 1) * 120 - 110));
			ttf->setColor(ccc3(0, 0, 255));
		}
		else
		{
			ttf->setPosition(ccp(winSize.width / 8, winSize.height - i / 2 * 120 - 110));//Dimensions 面积
			ttf->setColor(ccc3(0, 255, 255));
		}
		// label的锚点，除了影响setPosition，还指定了文字的对齐方式(默认居中，现在是左对齐)
		ttf->setAnchorPoint(ccp(0, 1));
	}
	return true;
}

void SceneScore::uploadScore(int score, const char* name)
{
	char data[100];
	sprintf(data, "level=%d&name=%s&score=%d", MenuLayer::_mapIndex + 1, name, score);

	//二：post请求
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://139.199.26.222:8080/protectWife_server_game/servlet/Server");//设置url
	//request->setUrl("http://localhost:8080/protectWife_server_game/servlet/Server");//设置url
	request->setRequestType(CCHttpRequest::kHttpPost);
	request->setRequestData(data, strlen(data));
	request->setResponseCallback(this, httpresponse_selector(SceneScore::Response));//设置请求回调函数
	//request->setTag("Post_My_Data");
	CCHttpClient::getInstance()->send(request);
	request->release();
}
void SceneScore::Response(CCHttpClient* client, CCHttpResponse* resp)
{
	// 错误处理
	if (!resp->isSucceed())
	{
		uploadFalse();
		CCLog("Response Error: %s", resp->getErrorBuffer());
		return;
	}

	// 正确的处理，先只是打印服务器返回的内容
	std::vector<char>* data = resp->getResponseData();
	std::string str;

	for (std::vector<char>::iterator it = data->begin(); it != data->end(); it++)
	{
		char ch = *it;
		str += ch;
	}
	//CCLog("resp: %s", str.c_str());

	char* name;
	char* score;
	char* p = new char[str.size() + 1];
	strcpy(p, str.c_str());
	strtok(p, "|");
	for (int i = 1; i<= 16; i++)
	{
		name = strtok(NULL, "|");
		if (name == NULL)
			break;
		score = strtok(NULL, "|");

		CCLabelTTF* ttf = (CCLabelTTF*)getChildByTag(1000 + i);
		char show[1024];
		sprintf(show, "%s:%sK", name, score);
		ttf->setString(show);
	}

	waitSprite->setVisible(false);

	delete p;
}
void SceneScore::uploadwait()
{
	CCSprite* s = CCSprite::create(IMAGE_shangchuan);
	addChild(s);
	s->setScale(1.5);
	s->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	waitSprite = s;
}
void SceneScore::uploadFalse()
{
	CCSprite* s = CCSprite::create(IMAGE_upScoreFalse);
	addChild(s);
	s->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	waitSprite->setVisible(false);
}