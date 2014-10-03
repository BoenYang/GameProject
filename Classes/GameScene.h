#ifndef GameScene_h__
#define GameScene_h__

#include "RayCastCallback.h"
#include "cocos2d.h"
#include "CCBlade.h"
#include <vector>
#include "Bomb.h"
#include "Hero.h"

#define TOOLS_NUM 2


class GameScene : public cocos2d::CCLayer
{
public:
	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(GameScene);

	bool isPausing;

	virtual void keyBackClicked(void);

	void update(float delta);
private:

	void addGameControlButton();

	void addToolsMenu();

	void setToolsMenuVisible();

	void setToolsMenuInvisible();

	void toolsMenuButtonCallback(cocos2d::CCObject* sender);

	void toolButtonCallBack(cocos2d::CCObject* sender);

	void pauseButtonCallback(cocos2d::CCObject* sender);

	void resetButtonCallback(cocos2d::CCObject* sender);

	bool splitPolygonSprite(PolygonSprite* sprite);

	void checkAndSliceObjects();

	void initPhysicsWord();

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void registerWithTouchDispatcher(void);

	void addNewSpriteAtPosition(cocos2d::CCPoint p);

	void loadLevelFromFile(const char* levelFile);

	ElementType getElementTypeByString(const char* str);

	long millisecondNow(){
		cocos2d::cc_timeval now;
		cocos2d::CCTime::gettimeofdayCocos2d(&now, NULL);
		return (now.tv_sec * 1000 + now.tv_usec / 1000);
	}


	void addDashLine();

	void addBlades();

	void addBackground();

	void playBackgroundMusic();

	void gameLogic();

	void guideLogic();

	void winSchedule(float dt);

	void addStarBord();

	void resetLevelSchedule(float dt);

	bool isOutOfScreen(b2Body* body);

	void cloudSchedule(float dt);

private:

	RaycastCallback* _raycastCallback;

	b2World* world;

	cocos2d::CCPoint startTouchPoint;

	cocos2d::CCPoint endTouchPoint;

	cocos2d::CCProgressTimer* line;

	cocos2d::CCArray* blades;

	CCBlade* blade;

	bool isTouchBegan;

	bool isTouchMoving;

	bool isUsingTools;



	int usingToolNum;		//使用的道具编号	1	灭火器		2	金手指

	float deltaRemainder;

	cocos2d::CCArray bombs;

	cocos2d::CCArray stars;

	cocos2d::CCSprite* starBords[3];

	int starNum;

	Hero* hero;

	virtual ~GameScene();

	cocos2d::CCSprite* bar;

	cocos2d::CCMenu* toolMenu;

	cocos2d::CCLabelBMFont* anniNum;

	cocos2d::CCLabelBMFont* fingerNum;

	cocos2d::CCMenuItemImage* finger;
	
	cocos2d::CCMenuItemImage* fire_extinguisher;

	cocos2d::CCSprite* cloud;

	cocos2d::CCSprite* topcloud;
public:
	bool isShowResult;
};

#endif // GameScene_h__
