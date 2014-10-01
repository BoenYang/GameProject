#ifndef RunTimeData_h__
#define RunTimeData_h__

#include "cocos2d.h"
#include "GameScene.h"

class RunTimeData{

private:
	RunTimeData();
public:
	static RunTimeData* getInstance();
public:
	int passedLevel;
	int startNum;
	bool hasMusic;

	bool isFirstTime;
	int selectPackage;
	int currentLevelStarNum;
	int selectLevel;
	int annihilatorNum;
	char* levelStars;
	
	GameScene* layer;
};


#endif // RunTimeData_h__

