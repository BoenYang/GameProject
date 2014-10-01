#include "ShopScene.h"
#include "ScreenHelper.h"
#include "RunTimeData.h"
#include "Macros.h"
#include "SimpleAudioEngine.h"
#include "LocalData.h"

using namespace CocosDenshion;


cocos2d::CCScene* ShopScene::scene()
{
	CCScene *scene = CCScene::create();

	ShopScene *layer = ShopScene::create();

	scene->addChild(layer);
	scene->setTag(SHOP_SCENCE);
	return scene;
}

bool ShopScene::init()
{
	CCLayer::init();
	addBackground();
	addButton();
	addTitle();
	addStarNumber();
	return true;
}

void ShopScene::addBackground()
{
	CCSprite* background = CCSprite::create("background/level_select.png");
	background->setPosition(screenCenter);
	this->addChild(background);
	CCSprite* labelBackground = CCSprite::create("label/label_background.png");
	labelBackground->setPosition(ccp(screenCenter.x, screenHeight - 200));
	this->addChild(labelBackground, 1);
}

void ShopScene::addButton()
{
	CCMenuItemImage* back = CCMenuItemImage::create("button/back_normal.png", "button/back_pressed.png", this, menu_selector(ShopScene::backButtonCallback));
	CCMenuItemImage* fire = CCMenuItemImage::create("button/fire_normal.png", "button/fire_pressed.png", this, menu_selector(ShopScene::fireButtonCallback));
	CCString* anniNumStr = CCString::createWithFormat("%d", RunTimeData::getInstance()->annihilatorNum);
	anniNum = CCLabelBMFont::create(anniNumStr->getCString(), "label/number.fnt");
	CCMenu* menu = CCMenu::create(back, fire, NULL);
	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);
	back->setPosition(ccp(100, 100));
	fire->setPosition(ccp(screenCenter.x,screenHeight - 400));
	fire->setScale(1.5f);
	fire->addChild(anniNum, 1);
	anniNum->setPosition(ccp(fire->getContentSize().width / 2, -5));
	anniNum->setScale(0.9f);
	
	this->addChild(menu);
}

void ShopScene::backButtonCallback(cocos2d::CCObject* sender)
{
	SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	CCDirector::sharedDirector()->popScene();
}

void ShopScene::fireButtonCallback(cocos2d::CCObject* sender)
{
	SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	if (RunTimeData::getInstance()->startNum >= 3)
	{
		RunTimeData::getInstance()->annihilatorNum++;
		RunTimeData::getInstance()->startNum -= 3;
		LocalData::writeRuntimeDataToLocal();
		CCString* numStr = CCString::createWithFormat("%d", RunTimeData::getInstance()->annihilatorNum);
		anniNum->setString(numStr->getCString());
	}
}

void ShopScene::addTitle()
{
	CCSprite* titleBackground = CCSprite::create("label/label_background.png");
	titleBackground->setPosition(ccp(screenCenter.x, screenHeight - 200));
	this->addChild(titleBackground);
	CCLabelBMFont* shop = CCLabelBMFont::create("shop", "label/letter.fnt");
	shop->setPosition(ccp(screenCenter.x, screenHeight - 200));
	this->addChild(shop,1);
}

void ShopScene::addStarNumber()
{
	CCString* starNumStr = CCString::createWithFormat("%d", RunTimeData::getInstance()->startNum);
	CCLabelBMFont* starNumLabel = CCLabelBMFont::create(starNumStr->getCString(), "label/number.fnt");
	starNumLabel->setPosition(ccp(180, screenHeight - 65));
	this->addChild(starNumLabel);
	CCSprite* star = CCSprite::create("label/star.png");
	star->setPosition(ccp(100, screenHeight - 60));
	star->setScale(0.4f);
	this->addChild(star);
}
