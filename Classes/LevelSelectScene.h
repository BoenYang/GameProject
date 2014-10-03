#ifndef LevelSelectScene_h__
#define LevelSelectScene_h__

#include "cocos2d.h"
#include "Macros.h"

class LevelSelectScene : public cocos2d::CCLayer
{
public:
	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(LevelSelectScene);

private:

	void addLevelLabel();

	void addLevelButton();

	void addLevelStar();

	void addLevelMark();

	void addBackground();

	void addBackAndShopButton();

	void addStarNumber();

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void registerWithTouchDispatcher(void);

	void changeBackgroundCallback();

	void backButtonCallback(cocos2d::CCObject* sender);

	void shopButtonCallback(cocos2d::CCObject* sender);

	virtual void keyBackClicked(void);

private:

	int currentPackage;

	cocos2d::CCMenu* levelMenu;

	cocos2d::CCPoint touchStartPoint;

	cocos2d::CCPoint levelMenuPosition;

	cocos2d::CCSprite* background;

	cocos2d::CCSprite* points[3];

	cocos2d::CCSprite* point_checked;

	cocos2d::CCRect levelBox[TOTAL_LEVEL_NUM];
};

#endif // LevelSelectScene_h__