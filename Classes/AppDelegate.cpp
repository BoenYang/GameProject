#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "ScreenHelper.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;

float scalex;
float scaley;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
	scalex = scaley = 1;
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCEGLView * pEGlView = CCEGLView::sharedOpenGLView();
	pDirector->setOpenGLView(pEGlView);
	CCSize frameSize = CCEGLView::sharedOpenGLView()->getFrameSize();
	CCSize lsSize = CCSizeMake(720, 1280);
	float scaleX = (float)frameSize.width / lsSize.width;
	float scaleY = (float)frameSize.height / lsSize.height;
	if (scaleX <= scaleY)
	{
		float front = lsSize.width;
		lsSize.width = lsSize.height*frameSize.width / frameSize.height;
		scalex = lsSize.width / front;
	}
	else
	{
		float front = lsSize.height;
		lsSize.height = lsSize.width*frameSize.height / frameSize.width;
		scaley = lsSize.height / front;
	}
	pEGlView->setDesignResolutionSize(lsSize.width, lsSize.height, kResolutionShowAll);
	//pDirector->setDisplayStats(true);
	pDirector->setAnimationInterval(1.0 / 60);
	INIT_ADAPTER();
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	CCScene *pScene = MenuScene::scene();
	pDirector->runWithScene(pScene);
#else
	CCScene *pScene = HelloWorld::scene();
	pDirector->runWithScene(pScene);
#endif
	HelloWorld::loadAudio();
    return true;
}


void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
