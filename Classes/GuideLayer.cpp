#include "GuideLayer.h"
#include "ScreenHelper.h"
#include "RunTimeData.h"
#include "LocalData.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

static const char* guides[GUIDE_PAGE] = { "background/guide1.png", "background/guide2.png" };

bool GuideLayer::init()
{
	CCLayer::init();
	setTouchEnabled(true);
	node = CCNode::create();
	node->setPosition(CCPointZero);


	for (int i = 0; i < GUIDE_PAGE; i++)
	{
		backgrounds[i] = CCSprite::create(guides[i]);
		backgrounds[i]->setAnchorPoint(ccp(0.0f, 1.0f));
		backgrounds[i]->setPosition(ccp(screenWidth*i,screenHeight));
		node->addChild(backgrounds[i]);
	}
	this->addChild(node, 3);

	CCMenuItemImage* getit = CCMenuItemImage::create("label/getit.png", "label/getit.png", "label/getit.png", this, menu_selector(GuideLayer::getItButton));
	CCMenu* menu = CCMenu::create(getit, NULL);
	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);
	getit->setPosition(ccp(screenWidth / 2, 150));
	backgrounds[GUIDE_PAGE-1]->addChild(menu);
	return true;
}

void GuideLayer::getItButton(CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic){
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	RunTimeData::getInstance()->isFirstTime = false;
	LocalData::writeRuntimeDataToLocal();
	getParent()->scheduleUpdate();
	this->removeFromParentAndCleanup(true);
	
}

bool GuideLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	startPoint = pTouch->getLocation();
	pos = node->getPosition();
	return true;
}


void GuideLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint currentPoint = pTouch->getLocation();
	float moveDistance = currentPoint.x - startPoint.x;
	if ( (pos.x + moveDistance < 0) && (pos.x + moveDistance > -720))
		node->setPositionX(pos.x + moveDistance);
}

void GuideLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void GuideLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

void GuideLayer::registerWithTouchDispatcher(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

