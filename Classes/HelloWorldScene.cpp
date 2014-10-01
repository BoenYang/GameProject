#include "HelloWorldScene.h"
#include "ScreenHelper.h"
#include "MenuScene.h"
#include "LocalData.h"
#include "RunTimeData.h"
#include "Macros.h"


USING_NS_CC;

using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::create();
    
    HelloWorld *layer = HelloWorld::create();

    scene->addChild(layer);
	scene->setTag(HELLOWORLD_SCENE);

    return scene;
}

bool HelloWorld::init()
{
	if (!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
    {
        return false;
    }
	setTouchEnabled(true);
	CCSprite* logo = CCSprite::create("logo.png");
	CCActionInterval* fadeOut = CCFadeOut::create(1.0f);
	logo->setAnchorPoint(ccp(0.5f, 0.5f));
	logo->setPosition(screenCenter);
	logo->runAction(CCSequence::create(fadeOut,CCCallFunc::create(this,callfunc_selector(HelloWorld::logoFadeOutCallback)),NULL));
	this->addChild(logo);
	logo->setScale(1.5f);
	LocalData::readLocalDataToRunTime();
	CCLOG("win height %f win width %f", CCDirector::sharedDirector()->getWinSize().height, CCDirector::sharedDirector()->getWinSize().width);
    return true;
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void HelloWorld::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
}

void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCScene* menuScene = MenuScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, menuScene));
}

void HelloWorld::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

void HelloWorld::registerWithTouchDispatcher(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

void HelloWorld::logoFadeOutCallback()
{
	CCScene* menuScene = MenuScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, menuScene));
}

void HelloWorld::loadAudio()
{
	SimpleAudioEngine::sharedEngine()->preloadEffect(AUDIO_CLICK);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(AUDIO_BACKGROND);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(AUDIO_SUCCESS);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(AUDIO_GAME);
	SimpleAudioEngine::sharedEngine()->preloadEffect(AUDIO_WATER);
	SimpleAudioEngine::sharedEngine()->preloadEffect(AUDIO_CUTTING);
	SimpleAudioEngine::sharedEngine()->preloadEffect(AUDIO_FAIL);
	SimpleAudioEngine::sharedEngine()->preloadEffect("audio/catch_star1.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("audio/catch_star1.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("audio/catch_star1.wav");
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.1f);
}







