#ifndef AboutScene_h__
#define AboutScene_h__

#include "cocos2d.h"

class AboutScene : public cocos2d::CCLayer
{
public:
	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(AboutScene);

	void addBackground();

	void addAboutInfo();

	void addBackButton();

	virtual void update(float dt);

	void backButtonCallBack(cocos2d::CCObject* sender);
private:
	cocos2d::CCSprite* about;
};
#endif // AboutScene_h__

