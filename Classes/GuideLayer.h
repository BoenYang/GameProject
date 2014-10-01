#ifndef GuideLayer_h__
#define GuideLayer_h__


#define GUIDE_PAGE 2

#include "cocos2d.h"

class GuideLayer : public cocos2d::CCLayer {

public:
	virtual bool init();

	CREATE_FUNC(GuideLayer);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void registerWithTouchDispatcher(void);

	void getItButton(cocos2d::CCObject* sender);

	cocos2d::CCPoint startPoint;

	cocos2d::CCSprite* backgrounds[GUIDE_PAGE];

	cocos2d::CCNode* node;

	cocos2d::CCPoint pos;

};


#endif // PauseLayer_h__