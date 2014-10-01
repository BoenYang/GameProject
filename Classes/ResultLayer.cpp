#include "ResultLayer.h"
#include "RunTimeData.h"
#include "LocalData.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "LevelSelectScene.h"
#include "ScreenHelper.h"

USING_NS_CC;

using namespace CocosDenshion;

bool ResultLayer::init()
{
	CCLayerColor::initWithColor(ccc4(0, 0, 0, 100));
	setTouchEnabled(true);
	addGameControlButton();
	addScore();
	addStar();
	addRole();
	addLabel();
	return true;
}

void ResultLayer::addGameControlButton()
{
	CCMenuItemImage* levelSelect = CCMenuItemImage::create("button/home_menu_normal.png", "home_menu_pressed.png", this, menu_selector(ResultLayer::menuButtonCallback));
	CCMenuItemImage* reset = CCMenuItemImage::create("button/home_restart_normal.png", "home_restart_pressed.png", this, menu_selector(ResultLayer::resetButtonCallback));
	CCMenuItemImage* next = CCMenuItemImage::create("button/home_next_normal.png", "home_next_pressed.png", this, menu_selector(ResultLayer::nextButtonCallback));
	CCMenu* menu = CCMenu::create(levelSelect, reset, next, NULL);
	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);
	menu->alignItemsHorizontally();
	levelSelect->setPosition(ccp(screenCenter.x - 120, 300));
	
	if (RunTimeData::getInstance()->selectLevel < 32)
	{
		reset->setPosition(ccp(screenCenter.x, 300));
		next->setPosition(ccp(screenCenter.x + 120, 300));
	}
	else{
		reset->setPosition(ccp(screenCenter.x + 120, 300));
		next->setPosition(ccp(-300, -300));
	}


	this->addChild(menu);
}

void ResultLayer::addScore()
{

}

void ResultLayer::addStar()
{
	float startX = screenWidth / 2 - 80;
	for (int i = 0; i < MAX_STAR_NUM; i++)
	{
		starBord[i] = CCSprite::create("label/starIcon.png");
		starBord[i]->setPosition(ccp(startX + i * 80, 420));
		starBord[i]->setScale(0.4f);
		this->addChild(starBord[i],1);
	}

	for (int i = 0; i < RunTimeData::getInstance()->currentLevelStarNum; i++)
	{
		CCSprite* star = CCSprite::create("label/star.png");
		star->setPosition(starBord[i]->getPosition());
		this->addChild(star,1);
		CCSprite* light = CCSprite::create("label/starLight.png");
		light->setPosition(starBord[i]->getPosition());
		//light->setScale(0.4f);
		star->setScale(0.4f);
		CCActionInterval* r = CCRotateTo::create(10.0f, 180);
		this->addChild(light);
		light->runAction(CCRepeatForever::create(r));
	}

}

void ResultLayer::addRole()
{
	CCSprite* role = CCSprite::create("label/win.png");
	role->setPosition(ccp(screenCenter.x, screenCenter.y - 30));
	this->addChild(role);
}

void ResultLayer::addLabel()
{
	CCLabelBMFont* levelClear = CCLabelBMFont::create("Level cleared", "label/letter.fnt");
	levelClear->setPosition(ccp(screenWidth/2,screenHeight/2 + 200));
	levelClear->setScale(1.6f);
	this->addChild(levelClear);
}

bool ResultLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void ResultLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
}

void ResultLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
}

void ResultLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

void ResultLayer::registerWithTouchDispatcher(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}



void ResultLayer::menuButtonCallback(cocos2d::CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic){
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	CCScene* levelScene = LevelSelectScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, levelScene));
}

void ResultLayer::nextButtonCallback(cocos2d::CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic){
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	RunTimeData::getInstance()->selectLevel++;
	RunTimeData::getInstance()->selectPackage = (RunTimeData::getInstance()->selectLevel - 1) / 16 + 1;
	CCScene* gameScene = GameScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, gameScene));
}

void ResultLayer::resetButtonCallback(cocos2d::CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic){
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	CCScene* gameScene = GameScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f,gameScene));
}