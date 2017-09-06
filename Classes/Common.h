#ifndef __Common_H__
#define __Common_H__

#include "Macro.h"
#include "Util.h"

enum TILE_TYPE {//tile 瓦片
	tileNone,
	tileGrass,
	tileSteel,
	tileWall,
	tileKing,
	tileRiver
};

static TILE_TYPE tileGidType[] = {
	tileNone,
	tileNone, tileNone, tileGrass, tileGrass, tileSteel, tileSteel,
	tileNone, tileNone, tileGrass, tileGrass, tileSteel, tileSteel,
	tileWall, tileWall, tileRiver, tileRiver, tileKing, tileKing,
	tileWall, tileWall, tileRiver, tileRiver, tileKing, tileKing
};

#if 0
//本来两点多既可以睡了，，结果这里出问题，，移植到android后  fread fwrite这两个函数不管用
//日了狗了，，还好我还有一套解决方案。。还是待到了5点，看能睡不
static int getLevel()
{
	FILE* fp = NULL;
	fp = fopen(TXT_level, "rb");
	if (!fp)
		return 1;

	char buf;
	fread(&buf, 1, 1, fp);//读到的是整数，但因为 char buf[0] 只占一个字节，那么就是字符对应的ascii码，但如果写入的是char 那么 就应该返回 buf[0]-'0'
	
	fclose(fp);
	fp = NULL;

	return buf;
}


static void setLevel(char newLevel)
{
	FILE* fp = NULL;
	fp = fopen(TXT_level, "wb+");
	if (!fp)
		return;

	char buf = newLevel;
	fwrite(&buf, 1, 1, fp);

	fclose(fp);
	fp = NULL;
}

#else

static int getLevel()
{
	//读取游戏，当没有读取到时返回1
	return CCUserDefault::sharedUserDefault()->getIntegerForKey("level", 1);
}
static int getFire1()
{
	return CCUserDefault::sharedUserDefault()->getIntegerForKey("fire1", 2);
}
static int getFire2()
{
	return CCUserDefault::sharedUserDefault()->getIntegerForKey("fire2", 2);
}
static int getFire3()
{
	return CCUserDefault::sharedUserDefault()->getIntegerForKey("fire3", 2);
}

static void setLevel(int newLevel)
{
	/*
	1、创建一个xml文件         也叫tiny databases
	2、将最高分写入xml文件中
	3、读取xml文件中的最高分
	*/
	//保存到内存中
	CCUserDefault::sharedUserDefault()->setIntegerForKey("level", newLevel);
	
	//写到硬盘中
	CCUserDefault::sharedUserDefault()->flush();
}
static void setFire1(int num)
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("fire1", num);
	CCUserDefault::sharedUserDefault()->flush();
}
static void setFire2(int num)
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("fire2", num);
	CCUserDefault::sharedUserDefault()->flush();
}
static void setFire3(int num)
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("fire3", num);
	CCUserDefault::sharedUserDefault()->flush();
}
#endif

#endif