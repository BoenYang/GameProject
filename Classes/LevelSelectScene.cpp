#include "LevelSelectScene.h"
#include "ScreenHelper.h"
#include "RunTimeData.h"
#include "GameScene.h"
#include "ShopScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;

static const char* backgroundFiles[] = {"background/level_select.png","background/level_select.png"};
static const char* level_star_file[] = { "button/level_star0.png", "button/level_star1.png", "button/level_star2.png", "button/level_star3.png" };
static const char* levelLabelFiles[] = { "label/Jungle.png", "label/Vally.png" };

CCScene* LevelSelectScene::scene()
{
	CCScene *scene = CCScene::create();

	LevelSelectScene *layer = LevelSelectScene::create();

	scene->addChild(layer);
	scene->setTag(LEVELSELECT_SCENE);

	return scene;
}

bool LevelSelectScene::init()
{
	CCLayer::init();
	currentPackage = 1;
	RunTimeData::getInstance()->selectPackage = currentPackage;
	setTouchEnabled(true);
	addBackground();
	addLevelButton();
	addLevelLabel();
	addLevelMark();
	addBackAndShopButton();
	addStarNumber();
	if (RunTimeData::getInstance()->hasMusic)
	{
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(AUDIO_BACKGROND);
	}
	return true;
}

void LevelSelectScene::addBackground()
{
	background = CCSprite::create(backgroundFiles[0]);
	background->setPosition(screenCenter);
	this->addChild(background);

	CCSprite* leftTopLeaf = CCSprite::create("background/lefttop_leaf.png");
	leftTopLeaf->setAnchorPoint(ccp(0.0f, 1.0f));
	leftTopLeaf->setPosition(ccp(leftTop.x - 10, leftTop.y + 17));
	this->addChild(leftTopLeaf);

	CCSprite* rightTopLeaf = CCSprite::create("background/righttop_leaf.png");
	rightTopLeaf->setAnchorPoint(ccp(1.0f, 1.0f));
	rightTopLeaf->setPosition(ccp(rightTop.x + 15, rightTop.y + 15));
	this->addChild(rightTopLeaf);

	CCSprite* leftBottomLeaf = CCSprite::create("background/leftbottom_leaf.png");
	leftBottomLeaf->setAnchorPoint(ccp(0.0f, 0.0f));
	leftBottomLeaf->setPosition(ccp(-15, -20));
	this->addChild(leftBottomLeaf);

	CCSprite* rightBottomLeaf = CCSprite::create("background/rightbottom_leaf.png");
	rightBottomLeaf->setAnchorPoint(ccp(1.0f, 0.0f));
	rightBottomLeaf->setPosition(ccp(rightBottom.x + 15 , rightBottom.y - 20));
	this->addChild(rightBottomLeaf);

	CCActionInterval* moveRight = CCMoveBy::create(3.0f, ccp(15, 0));
	CCActionInterval* moveLeft = moveRight->reverse();
	CCSequence* move = CCSequence::create(moveRight, moveLeft, NULL);
	leftBottomLeaf->runAction(CCRepeatForever::create(move));

	CCActionInterval* moveLeft1 = CCMoveBy::create(3.0f, ccp(-15, 0));
	CCActionInterval* moveRight1 = moveLeft1->reverse();
	CCSequence* move1 = CCSequence::create(moveLeft1, moveRight1, NULL);
	rightBottomLeaf->runAction(CCRepeatForever::create(move1));

	CCActionInterval* moveLeft2 = CCMoveBy::create(3.0f, ccp(-10, 0));
	CCActionInterval* moveRight2 = moveLeft2->reverse();
	CCSequence* move2 = CCSequence::create(moveLeft2, moveRight2, NULL);
	rightTopLeaf->runAction(CCRepeatForever::create(move2));

	CCActionInterval* rotationDown = CCRotateBy::create(3.0f, 10);
	CCActionInterval* rotationUp = rotationDown->reverse();
	CCSequence* rotation = CCSequence::create(rotationDown, rotationUp, NULL);
	leftTopLeaf->runAction(CCRepeatForever::create(rotation));


}

void LevelSelectScene::addLevelLabel()
{
	for (int i = 0; i < 2; i++)
	{
		CCMenuItemImage* levelLabel = CCMenuItemImage::create(levelLabelFiles[i], levelLabelFiles[i], levelLabelFiles[i], this, NULL);
		levelLabel->setPosition(ccp(screenWidth / 2 + screenWidth*i, screenHeight - 200));
		levelMenu->addChild(levelLabel);
	}
}

void LevelSelectScene::addBackAndShopButton()
{
	CCMenuItemImage* back = CCMenuItemImage::create("button/back_normal.png", "button/back_pressed.png", this, menu_selector(LevelSelectScene::backButtonCallback));
	CCMenuItemImage* shop = CCMenuItemImage::create("button/shop_normal.png", "button/shop_pressed.png", this, menu_selector(LevelSelectScene::shopButtonCallback));
	CCMenu* menu = CCMenu::create(back,shop, NULL);
	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);
	back->setPosition(ccp(100, 100));
	shop->setPosition(ccp(screenWidth - 100, 100));
	shop->setEnabled(false);
	this->addChild(menu);
}

void LevelSelectScene::addLevelButton()
{
	levelMenu = CCMenu::create();
	levelMenu->setAnchorPoint(CCPointZero);
	levelMenu->setPosition(CCPointZero);
	levelMenu->setTouchEnabled(false);
	levelMenuPosition = CCPointZero;
	for (int i = 0; i < TOTAL_LEVEL_NUM; i++)
	{
		int package = i / 16;
		int x = i % 4;
		int y = i / 4 - i / 16 * 4;
		int dynamicWidth = (1 - scaley) * 720;
		float px = 110 + x * 170 + screenWidth * package;
		float py = screenHeight - 440 - y * 155 + dynamicWidth;

		if (i < RunTimeData::getInstance()->passedLevel)
		//if (i < TOTAL_LEVEL_NUM)
		{
			int startNum = RunTimeData::getInstance()->levelStars[i] - '0';
			CCLOG("%d", startNum);
			CCSprite *levelOpen = CCSprite::create(level_star_file[startNum]);
			CCString* numStr = CCString::createWithFormat("%d", i + 1);
			CCLabelBMFont* levelNum = CCLabelBMFont::create(numStr->getCString(), "label/number.fnt");
			CCMenuItemSprite* levelButton = CCMenuItemSprite::create(levelOpen, levelOpen, this, NULL);
			levelButton->setTag(i);
			levelButton->setPosition(ccp(px, py));
			levelBox[i] = levelButton->boundingBox();
			levelButton->addChild(levelNum, 2);
			levelNum->setPosition(levelButton->getAnchorPointInPoints());
			levelMenu->addChild(levelButton);
		}
		else
		{
			CCSprite * levelClosed = CCSprite::create("button/level_lock.png");
			CCMenuItemSprite * levelButtonClosed = CCMenuItemSprite::create(levelClosed, levelClosed, this, NULL);
			levelButtonClosed->setPosition(ccp(px, py));
			levelMenu->addChild(levelButtonClosed);
		}
	}

	CCSprite* labelBackground = CCSprite::create("label/label_background.png");
	labelBackground->setPosition(ccp(screenCenter.x, screenHeight - 200));
	this->addChild(labelBackground, 1);
	CCMenuItemImage* left_arrow = CCMenuItemImage::create("label/left_arrow_normal.png", "label/left_arrow_pressed.png", this, NULL);
	CCMenuItemImage* right_arrow = CCMenuItemImage::create("label/right_arrow_normal.png", "label/right_arrow_pressed.png", this, NULL);
	CCMenu* controlMenu = CCMenu::create(left_arrow, right_arrow, NULL);
	controlMenu->setAnchorPoint(CCPointZero);
	controlMenu->setPosition(CCPointZero);
	left_arrow->setPosition(ccp(100, screenHeight - 200));
	right_arrow->setPosition(ccp(screenWidth - 100, screenHeight - 200));
	left_arrow->setScale(0.6f);
	right_arrow->setScale(0.6f);

	CCMenuItemImage* moreLevel = CCMenuItemImage::create("label/morelevel.png", "label/morelevel.png", this, NULL);
	levelMenu->addChild(moreLevel);
	moreLevel->setPosition(ccp(screenWidth*2.5f, screenHeight / 2));
	this->addChild(controlMenu, 1);
	this->addChild(levelMenu, 1);
}

void LevelSelectScene::addLevelMark()
{
	float startX;
	if (MAX_PACKAGE_NUM % 2 == 0){
		startX = screenWidth / 2 - MAX_PACKAGE_NUM / 2 * 80 + 40;
	}
	else{
		startX = screenWidth / 2 - MAX_PACKAGE_NUM / 2 * 80;
	}
	
	for (int i = 0; i < MAX_PACKAGE_NUM; i++)
	{
		points[i] = CCSprite::create("label/point.png");
		points[i]->setPosition(ccp(startX + 80 * i, 200));
		this->addChild(points[i]);
	}

	point_checked = CCSprite::create("label/point_checked.png");
	point_checked->setPosition(points[0]->getPosition());
	this->addChild(point_checked, 1);
}

void LevelSelectScene::addStarNumber()
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

bool LevelSelectScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	touchStartPoint = pTouch->getLocation();
	return true;
}

void LevelSelectScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint currentPoint = pTouch->getLocation();
	float moveDistance = currentPoint.x - touchStartPoint.x;
	levelMenu->setPositionX(levelMenuPosition.x + moveDistance);
}

void LevelSelectScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint touchEndPoint = pTouch->getLocation();
	float touchMoveDistance = ccpDistance(touchStartPoint, touchEndPoint);
	CCActionInterval* fadeOut = CCFadeOut::create(0.2f);
	CCActionInterval* fadeIn = CCFadeIn::create(0.2f);
	CCAction* faceInAndOut = CCSequence::create(fadeOut,
		CCCallFunc::create(this, callfunc_selector(LevelSelectScene::changeBackgroundCallback)),
		fadeIn,
		NULL);
	if (touchMoveDistance > 50)
	{
		if (touchStartPoint.x > touchEndPoint.x)
		{
			if (currentPackage < 3)
			{
				currentPackage++;
				levelMenuPosition.x -= screenWidth;
				CCActionInterval* move = CCMoveTo::create(0.3f, ccp(levelMenuPosition.x, 0));
				CCEaseBackOut* flyIn = CCEaseBackOut::create(move);
				CCActionInterval* moveBy = CCMoveTo::create(0.2f, points[currentPackage - 1]->getPosition());
				point_checked->runAction(moveBy);
				levelMenu->runAction(flyIn);
				return;
			}
		}
		else
		{
			if (currentPackage > 1)
			{
				currentPackage--;
				levelMenuPosition.x += screenWidth;
				CCActionInterval* move = CCMoveTo::create(0.3f, ccp(levelMenuPosition.x, 0));
				CCEaseBackOut* flyIn = CCEaseBackOut::create(move);
				CCActionInterval* moveBy = CCMoveTo::create(0.2f, points[currentPackage - 1]->getPosition());
				point_checked->runAction(moveBy);
				levelMenu->runAction(flyIn);
				return;
			}
		}

	}
	else
	{
 		for (int i = 0; i < 16; i++)
 		{
 			CCRect box = levelBox[i];
 			if (box.containsPoint(touchEndPoint) && currentPackage <= 2)
 			{
				
				int currentLevel = 16 * (currentPackage - 1) + i + 1;
				CCLog("%d", currentLevel);
				if (RunTimeData::getInstance()->hasMusic)
				{
					CCLOG("click!");
					SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
				}
				if (currentLevel <= RunTimeData::getInstance()->passedLevel){
					RunTimeData::getInstance()->selectPackage = currentPackage;
					RunTimeData::getInstance()->selectLevel = currentLevel;
					//关卡进入回调函数

					if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
						SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
					}

					CCScene *pScene = GameScene::scene();
					CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.2f, pScene));
				}
 			}
 		}
	}
	CCActionInterval* move = CCMoveTo::create(0.3f, levelMenuPosition);
	CCEaseBackOut* flyIn = CCEaseBackOut::create(move);
	levelMenu->runAction(flyIn);
}

void LevelSelectScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

void LevelSelectScene::registerWithTouchDispatcher(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

void LevelSelectScene::changeBackgroundCallback()
{
	//background->initWithFile(backgroundFiles[currentPackage - 1]);
}

void LevelSelectScene::backButtonCallback(CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic){
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	CCScene* menuScene = MenuScene::scene();
	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, menuScene));
}

void LevelSelectScene::shopButtonCallback(cocos2d::CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic){
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	CCScene* shopScene = ShopScene::scene();
	CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, shopScene));
}