#include "LocalData.h"
#include "cocos2d.h"
#include "RunTimeData.h"
USING_NS_CC;

void LocalData::readLocalDataToRunTime()
{

	CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
	RunTimeData::getInstance()->hasMusic = userDefault->getBoolForKey("hasmusic", true);
	std::string levelStr = userDefault->getStringForKey("levelStarStr", "");
	RunTimeData::getInstance()->passedLevel = userDefault->getIntegerForKey("passedLevelNum", 1);
	RunTimeData::getInstance()->startNum = userDefault->getIntegerForKey("totalStarNum", 0);
	RunTimeData::getInstance()->annihilatorNum = userDefault->getIntegerForKey("annihilator", 0);
	RunTimeData::getInstance()->isFirstTime = userDefault->getBoolForKey("isFirstTime", true);
	if (levelStr.length() != 0){
		strcpy(RunTimeData::getInstance()->levelStars, levelStr.c_str());
	}
	RunTimeData::getInstance()->isFirstTime = userDefault->getBoolForKey("guiderecord", true);
}

void LocalData::writeRuntimeDataToLocal()
{
	CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
	userDefault->setBoolForKey("hasmusic", RunTimeData::getInstance()->hasMusic);
	userDefault->setBoolForKey("guiderecord", RunTimeData::getInstance()->isFirstTime);
	userDefault->setStringForKey("levelStarStr", RunTimeData::getInstance()->levelStars);
	userDefault->setIntegerForKey("passedLevelNum", RunTimeData::getInstance()->passedLevel);
	userDefault->setIntegerForKey("totalStarNum", RunTimeData::getInstance()->startNum);
	userDefault->setIntegerForKey("annihilator", RunTimeData::getInstance()->annihilatorNum);
	userDefault->setBoolForKey("isFirstTime", RunTimeData::getInstance()->isFirstTime);
	userDefault->flush();
}
