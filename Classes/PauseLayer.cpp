#include "PauseLayer.h"
#include "ScreenHelper.h"
#include "LevelSelectScene.h"
#include "RunTimeData.h"
#include "SimpleAudioEngine.h"
#include "LocalData.h"
#include "ShopScene.h"
#include "GameScene.h"

using namespace CocosDenshion;
USING_NS_CC;

bool PauseLayer::init()
{
	CCLayerColor::initWithColor(ccc4(0, 0, 0, 100));
	setKeypadEnabled(true);
	setTouchEnabled(true);
	backgroundMusicPaused = false;
	addGameControlButtons();
	return true;
}

bool PauseLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void PauseLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
}

void PauseLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
}

void PauseLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

void PauseLayer::registerWithTouchDispatcher(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

void PauseLayer::addGameControlButtons()
{
	CCMenuItemImage* levelSelect = CCMenuItemImage::create("button/home_menu_normal.png", "button/home_menu_pressed.png", this, menu_selector(PauseLayer::levelSelectButtonCallback));
	CCMenuItemImage* go_on = CCMenuItemImage::create("button/home_resume_normal.png", "button/home_resume_pressed.png", this, menu_selector(PauseLayer::continueButtonCallback));
	CCMenuItemImage* reset = CCMenuItemImage::create("button/home_restart_normal.png", "button/home_restart_pressed.png", this, menu_selector(PauseLayer::restartButtonCallback));
	CCMenuItemImage* shop = CCMenuItemImage::create("button/home_shop_normal.png", "button/home_shop_pressed.png", this, menu_selector(PauseLayer::shopButtonCallback));
	CCMenuItemImage* sound = CCMenuItemImage::create("button/home_sound_normal.png", "button/home_sound_pressed.png", this, menu_selector(PauseLayer::soundButtonCallback));
	CCMenu* menu = CCMenu::create(levelSelect, go_on,reset,shop,sound, NULL);
	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);
	menu->alignItemsHorizontally();
	levelSelect->setPosition(ccp(screenCenter.x - 120, screenCenter.y));
	go_on->setPosition(ccp(screenCenter.x , screenCenter.y));
	reset->setPosition(ccp(screenCenter.x + 120, screenCenter.y));
	shop->setPosition(ccp(screenCenter.x - 70, screenCenter.y - 100));
	shop->setEnabled(false);
	sound->setPosition(ccp(screenCenter.x + 70, screenCenter.y - 100));
	if (!RunTimeData::getInstance()->hasMusic){
		sound->selected();
	}
	this->addChild(menu);
}

void PauseLayer::continueButtonCallback(cocos2d::CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic){
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
	getParent()->schedule(schedule_selector(GameScene::update));
}

void PauseLayer::levelSelectButtonCallback(cocos2d::CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic){
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	CCScene* levelSelectScene = LevelSelectScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, levelSelectScene));
}

void PauseLayer::restartButtonCallback(cocos2d::CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic){
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	CCScene* gameScene = GameScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, gameScene));
}

void PauseLayer::shopButtonCallback(cocos2d::CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic){
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	CCScene* shopScene = ShopScene::scene();
	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, shopScene));
}

void PauseLayer::soundButtonCallback(cocos2d::CCObject* sender)
{
	CCMenuItemImage* sound = (CCMenuItemImage*)sender;
	if (RunTimeData::getInstance()->hasMusic){
		sound->selected();
		RunTimeData::getInstance()->hasMusic = false;
		if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
			SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
			backgroundMusicPaused = true;
		}
	}
	else{
		sound->unselected();
		RunTimeData::getInstance()->hasMusic = true;

		if (backgroundMusicPaused){
			SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();	
		}
		else{
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(AUDIO_GAME);
		}
	}
	
	LocalData::writeRuntimeDataToLocal();
}

void PauseLayer::keyBackClicked(void)
{
	CCLOG("resunm click");
	if (RunTimeData::getInstance()->hasMusic){
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
	getParent()->schedule(schedule_selector(GameScene::update));
	//getParent()->scheduleUpdate();
}
