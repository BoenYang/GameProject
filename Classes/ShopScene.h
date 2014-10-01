#ifndef ToolScene_h__
#define ToolScene_h__

#include "cocos2d.h"
USING_NS_CC;



class ShopScene : public cocos2d::CCLayer
{
public:
	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(ShopScene);

	void addBackground();
	
	void addButton();

	void addTitle();

	void addStarNumber();

	void backButtonCallback(cocos2d::CCObject* sender);

	void fireButtonCallback(cocos2d::CCObject* sender);

public:
	cocos2d::CCLabelBMFont* anniNum;
};

#endif // ToolScene_h__
