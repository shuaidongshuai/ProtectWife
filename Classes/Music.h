#ifndef __MUSIC_H__
#define __MUSIC_H__

#include "Common.h"

class Music:public CCLayer
{
public:
	CREATE_FUNC(Music);
	bool init();

	static void StartLayerMusic();//¥”Õ∑≤•∑≈
	static void StartGameMusic();
	static void stopBackgroundMusic();

	static void shootedMusic();

	static void eat1();
	static void eat2();
	static void gameOver();
};

#endif
