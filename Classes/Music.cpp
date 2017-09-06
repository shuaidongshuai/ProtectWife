#include "Music.h"
#include "SimpleAudioEngine.h"


bool Music::init()
{
	CCLayer::init();

	// 预加载
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MUSIC_background);//加载到内存中去，但不会执行
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MUSIC_startMusic);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(MUSIC_shoot);//加载到内存中去，但不会执行
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(MUSIC_eat1);//加载到内存中去，但不会执行
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(MUSIC_eat2);//加载到内存中去，但不会执行
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(MUSIC_gameover);//加载到内存中去，但不会执行

	return true;
}

void Music::StartLayerMusic()
{
	//设置音量大小
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_startMusic,true);
}
//void Music::resumeStartMusic()
//{
//	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
//}
//void Music::pauseStartMusic()
//{
//	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();//background只能有一个，所以不要参数
//}
void Music::StartGameMusic()
{
	//设置音量大小
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_background, true);
}

void Music::stopBackgroundMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void Music::shootedMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_shoot);
}

void Music::eat1()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_eat1);
}
void Music::eat2()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_eat2);
}

void Music::gameOver()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_gameover);
}