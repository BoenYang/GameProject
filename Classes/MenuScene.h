#ifndef MenuScene_h__
#define MenuScene_h__

#include "cocos2d.h"

class MenuScene : public cocos2d::CCLayer
{
public:
	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(MenuScene);

	void addBackground();

	void addTittle();

	void addButton();

	void shopButtonCallback(cocos2d::CCObject* sender);

	void startButtonCallback(cocos2d::CCObject* sender);

	void soundButtonCallback(cocos2d::CCObject* sender);

	void aboutButtonCallback(cocos2d::CCObject* sender);
	
	void playBackgroundMusic();

	virtual void keyBackClicked(void);

};

#endif // MenuScene_h__