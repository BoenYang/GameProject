#include "RunTimeData.h"
#include "GameScene.h"

static RunTimeData *pRunTimeData = NULL;

RunTimeData::RunTimeData()
{
	passedLevel = 1;
	startNum = 0;
	hasMusic = true;

	selectPackage = 1;
	currentLevelStarNum = 0;
	selectLevel = 0;
	annihilatorNum = 0;
	layer = NULL;

	isFirstTime = true;
	levelStars = new char[33];
	for (int i = 0; i < 32; i++)
	{
		levelStars[i] = '0';
	}

	levelStars[32] = '\0';
}

RunTimeData* RunTimeData::getInstance()
{
	if (pRunTimeData == NULL){
		pRunTimeData = new RunTimeData();
	}
	return pRunTimeData;
}

