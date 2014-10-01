#ifndef ResultLayer_h__
#define ResultLayer_h__


#include "cocos2d.h"
#include "Macros.h"

class ResultLayer : public cocos2d::CCLayerColor {

public:
	virtual bool init();

	CREATE_FUNC(ResultLayer);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void registerWithTouchDispatcher(void);
private:
	void addGameControlButton();

	void addScore();

	void addStar();

	void addRole();

	void addLabel();

	void menuButtonCallback(cocos2d::CCObject* sender);

	void nextButtonCallback(cocos2d::CCObject* sender);

	void resetButtonCallback(cocos2d::CCObject* sender);

	cocos2d::CCSprite* starBord[MAX_STAR_NUM];
};

#endif