#ifndef PauseLayer_h__
#define PauseLayer_h__

#include "cocos2d.h"
class PauseLayer : public cocos2d::CCLayerColor {

public:
	virtual bool init();

	CREATE_FUNC(PauseLayer);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void registerWithTouchDispatcher(void);

	void addGameControlButtons();
	
	void continueButtonCallback(cocos2d::CCObject* sender);

	void levelSelectButtonCallback(cocos2d::CCObject* sender);

	void restartButtonCallback(cocos2d::CCObject* sender);

	void shopButtonCallback(cocos2d::CCObject* sender);

	void soundButtonCallback(cocos2d::CCObject* sender);

	virtual void keyBackClicked(void);

	bool backgroundMusicPaused;
};


#endif // PauseLayer_h__