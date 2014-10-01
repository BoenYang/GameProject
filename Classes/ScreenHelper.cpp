#include "ScreenHelper.h"
#include "AppDelegate.h"

CCPoint screenCenter;
CCPoint leftBottom;
CCPoint leftTop;
CCPoint rightBottom;
CCPoint rightTop;
CCSize winSize;

float screenHeight;
float screenWidth;

void INIT_ADAPTER()
{
	screenWidth = CCDirector::sharedDirector()->getWinSize().width;
	screenHeight = CCDirector::sharedDirector()->getWinSize().height;
	screenCenter.x = screenWidth / 2;
	screenCenter.y = screenHeight / 2;
	leftBottom = CCPointZero;
	leftTop = ccp(0, screenHeight);
	rightBottom = ccp(screenWidth, 0);
	rightTop = ccp(screenWidth, screenHeight);
	winSize = CCDirector::sharedDirector()->getWinSize();
}

void SET_SCALE(CCNode * node)
{
	node->setScaleY(scaley);
}