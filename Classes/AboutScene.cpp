#include "AboutScene.h"
#include "ScreenHelper.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "RunTimeData.h"
#include "Macros.h"


using namespace CocosDenshion;

USING_NS_CC;



const int upSpeed = 5;

cocos2d::CCScene* AboutScene::scene()
{
	CCScene *scene = CCScene::create();

	AboutScene *layer = AboutScene::create();

	scene->addChild(layer);
	scene->setTag(ABOUT_SCENE);

	return scene;
}

bool AboutScene::init()
{
	CCLayer::init();
	addBackground();
	addAboutInfo();
	addBackButton();
	return true;
}

void AboutScene::addBackground()
{
	CCSprite* background = CCSprite::create("background/about_background.png");
	background->setPosition(screenCenter);
	CCSprite* bottomTrees = CCSprite::create("background/about_bottom.png");
	bottomTrees->setAnchorPoint(CCPointZero);
	bottomTrees->setPosition(CCPointZero);
	CCSprite* leftTopLeaf = CCSprite::create("background/lefttop_leaf.png");
	leftTopLeaf->setAnchorPoint(ccp(0, 1.0f));
	leftTopLeaf->setPosition(leftTop);
	CCSprite* rightTopLeaf = CCSprite::create("background/righttop_leaf.png");
	rightTopLeaf->setAnchorPoint(ccp(1.0f, 1.0f));
	rightTopLeaf->setPosition(rightTop);
	this->addChild(background, 0);
	this->addChild(bottomTrees, 2);
	this->addChild(leftTopLeaf, 2);
	this->addChild(rightTopLeaf, 2);
}

void AboutScene::addAboutInfo()
{
	about = CCSprite::create("about.png");
	about->setScale(2.0f);
	about->setAnchorPoint(ccp(0.5f, 1.0f));
	about->setPosition(ccp(screenWidth / 2, 0));
	this->addChild(about, 1);
	scheduleUpdate();
}

void AboutScene::addBackButton()
{
	CCMenuItemImage* back = CCMenuItemImage::create("button/back_normal.png", "button/back_pressed.png", this, menu_selector(AboutScene::backButtonCallBack));
	CCMenu* menu = CCMenu::create(back, NULL);
	menu->alignItemsHorizontally();

	back->setPosition(ccp(100, 100));

	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 3);
}

void AboutScene::update(float dt)
{
	if (about->getPosition().y >= (screenHeight + about->getContentSize().height*about->getScale())){
		about->setPosition(ccp(screenWidth / 2, 0));
	}
	else{
		about->setPositionY(about->getPositionY() + upSpeed);
	}
}

void AboutScene::backButtonCallBack(CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic)
	{
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	CCScene* menuScene = MenuScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, menuScene));
}