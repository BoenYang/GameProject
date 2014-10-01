#include "MenuScene.h"
#include "ScreenHelper.h"
#include "RunTimeData.h"
#include "LevelSelectScene.h"
#include "ShopScene.h"
#include "LocalData.h"
#include "SimpleAudioEngine.h"
#include "Macros.h"
#include "AboutScene.h"

USING_NS_CC;
using namespace CocosDenshion;

cocos2d::CCScene* MenuScene::scene()
{
	CCScene *scene = CCScene::create();

	MenuScene *layer = MenuScene::create();

	scene->addChild(layer);
	scene->setTag(MENU_SCENE);

	return scene;
}

bool MenuScene::init()
{
	CCLayer::init();
	setKeypadEnabled(true);
	addBackground();
	addTittle();
	addButton();
	playBackgroundMusic();
	return true;
}

void MenuScene::addBackground()
{
	CCSprite* background = CCSprite::create("background/home_background.png");
	background->setPosition(screenCenter);
	CCSprite* bottom = CCSprite::create("background/leftbottom_leaf.png");
	bottom->setAnchorPoint(CCPointZero);
	bottom->setPosition(ccp(-15,0));
	CCSprite* lefttopLeaf = CCSprite::create("background/lefttop_leaf.png");
	lefttopLeaf->setAnchorPoint(ccp(0, 1.0f));
	lefttopLeaf->setPosition(ccp(leftTop.x,leftTop.y + 15));
	this->addChild(bottom,1);
	this->addChild(lefttopLeaf,1);
	this->addChild(background);

	CCActionInterval* moveRight = CCMoveBy::create(2.5f, ccp(15, 0));
	CCActionInterval* moveLeft = moveRight->reverse();
	CCSequence* move = CCSequence::create(moveRight, moveLeft,NULL);
	bottom->runAction(CCRepeatForever::create(move));

	CCActionInterval* rotationDown = CCRotateBy::create(2.5f, 10);
	CCActionInterval* rotationUp = rotationDown->reverse();
	CCSequence* rotation = CCSequence::create(rotationDown, rotationUp, NULL);
	lefttopLeaf->runAction(CCRepeatForever::create(rotation));
}

void MenuScene::addTittle()
{
	CCSprite* title = CCSprite::create("label/title.png");
	title->setPosition(ccp(screenWidth / 2, screenHeight/2 + 750));
	CCActionInterval * fly = CCMoveTo::create(1.0f, ccp(screenWidth / 2, screenHeight / 2 + 240));
	title->runAction(fly);
	this->addChild(title);
}

void MenuScene::addButton()
{
	CCMenuItemImage* shop = CCMenuItemImage::create("button/home_shop_normal.png", "button/home_shop_pressed.png", this, menu_selector(MenuScene::shopButtonCallback));
	CCMenuItemImage* start = CCMenuItemImage::create("button/home_play_normal.png", "button/home_play_pressed.png", this, menu_selector(MenuScene::startButtonCallback));
	CCMenuItemImage* sound = CCMenuItemImage::create("button/home_sound_normal.png", "button/home_sound_pressed.png", this, menu_selector(MenuScene::soundButtonCallback));
	CCMenuItemImage* about = CCMenuItemImage::create("button/home_about_normal.png", "button/home_about_pressed.png", this, menu_selector(MenuScene::aboutButtonCallback));
	CCMenu* menu = CCMenu::create(start, sound, shop,about, NULL);
	menu->alignItemsHorizontally();
	
	start->setScale(1.5);

	start->setPosition(ccp(screenWidth / 2, 250));
	shop->setPosition(ccp(screenWidth - 100, 170));
	sound->setPosition(ccp(100, 170));

	about->setPosition(ccp(screenWidth - 40, 40));

	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);


	if (!RunTimeData::getInstance()->hasMusic){
		sound->selected();
	}

	this->addChild(menu,2);

	shop->setEnabled(false);
}

void MenuScene::playBackgroundMusic()
{
	if (RunTimeData::getInstance()->hasMusic && (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()))
	{
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.1f);
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(AUDIO_BACKGROND, true);
	}
}

void MenuScene::shopButtonCallback(CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic)
	{
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	CCScene* shopScene = ShopScene::scene();
	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, shopScene));
}

void MenuScene::startButtonCallback(CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic)
	{
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	CCScene* levelSelectScene = LevelSelectScene::scene();
	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, levelSelectScene));
}

void MenuScene::soundButtonCallback(CCObject* sender)
{
	CCMenuItemImage* sound = (CCMenuItemImage*)sender;
	if (RunTimeData::getInstance()->hasMusic){
		sound->selected();
		RunTimeData::getInstance()->hasMusic = false;
		//sound off code
		SimpleAudioEngine::sharedEngine()->pauseAllEffects();
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
	else{
		sound->unselected();
		RunTimeData::getInstance()->hasMusic = true;
		//sound on code
		if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(AUDIO_BACKGROND, true);
		}
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
	LocalData::writeRuntimeDataToLocal();
}

void MenuScene::aboutButtonCallback(CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic)
	{
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	CCScene* aboutScene = AboutScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, aboutScene));
}


void MenuScene::keyBackClicked(void)
{
}