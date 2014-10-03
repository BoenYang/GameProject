#include "GameScene.h"
#include "Box2dHelper.h"
#include "Element.h"
#include "Macros.h"
#include "ScreenHelper.h"
#include "TextureHelper.h"
#include "RunTimeData.h"
#include "PauseLayer.h"
#include "GB2ShapeCache-x.h"
#include "GLES-Render.h"
#include "SimpleAudioEngine.h"
#include "LocalData.h"
#include "ResultLayer.h"
#include "ShopScene.h"
#include "GuideLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

static const char* gameBackgrounds[] = { "background/pack1.png", "background/pack2.png" };
const char* star_catchAudios[] = { "audio/catch_star1.wav", "audio/catch_star2.wav", "audio/catch_star3.wav" };



/*
	游戏主场景类
*/
CCScene* GameScene::scene()
{
	CCScene *scene = CCScene::create();

	GameScene *layer = GameScene::create();

	scene->addChild(layer);

	scene->setTag(GAME_SCENE);

	return scene;
}

bool GameScene::init()
{
	CCLayer::init();
	setKeypadEnabled(true);
	isPausing = false;
	setTouchEnabled(true);
	isTouchBegan = false;
	isTouchMoving = false;
	isUsingTools = false;
	isShowResult = false;
	usingToolNum = 0;
	starNum = 0;
	RunTimeData::getInstance()->layer = this;
	RunTimeData::getInstance()->currentLevelStarNum = 0;
	bombs.removeAllObjects();
	hero = NULL;
	//addToolsMenu();
	addGameControlButton();
	addBackground();
	addDashLine();
	initPhysicsWord();
	addBlades();
	addStarBord();
	
	LocalData::writeRuntimeDataToLocal();
	playBackgroundMusic();

	this->schedule(schedule_selector(GameScene::update));
	//this->scheduleUpdate();
	CCString* str = CCString::createWithFormat("level/level%d.xml", RunTimeData::getInstance()->selectLevel);
	loadLevelFromFile(str->getCString());
	if (RunTimeData::getInstance()->isFirstTime){
		guideLogic();
	}
	return true;
}

void GameScene::initPhysicsWord()
{
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	world = new b2World(gravity);
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);

	this->_raycastCallback = new RaycastCallback;
}

void GameScene::addBackground()
{
	int packageNum = RunTimeData::getInstance()->selectPackage ;
	CCSprite* background = CCSprite::create(gameBackgrounds[packageNum-1]);
	background->setPosition(screenCenter);
	this->addChild(background);
	if (packageNum == 1){

		CCSprite* lefttopLeaf = CCSprite::create("background/lefttop_leaf.png");
		lefttopLeaf->setAnchorPoint(ccp(0, 1.0f));
		lefttopLeaf->setPosition(ccp(leftTop.x, leftTop.y + 15));
		this->addChild(lefttopLeaf,1);

		CCSprite* leftBottomLeaf = CCSprite::create("background/leftbottom_leaf.png");
		leftBottomLeaf->setAnchorPoint(CCPointZero);
		leftBottomLeaf->setPosition(ccp(-15, 0));
		this->addChild(leftBottomLeaf,2);

		CCSprite* rightBottomLeaf = CCSprite::create("background/rightbottom_leaf.png");
		rightBottomLeaf->setAnchorPoint(ccp(1.0f, 0.0f));
		rightBottomLeaf->setPosition(ccp(rightBottom.x + 15,rightBottom.y));
		this->addChild(rightBottomLeaf,1);

		CCSprite* rightTopLeaf = CCSprite::create("background/righttop_leaf.png");
		rightTopLeaf->setAnchorPoint(ccp(1.0f, 1.0f));
		rightTopLeaf->setPosition(ccp(rightTop.x + 10,rightTop.y));
		this->addChild(rightTopLeaf,1);

		topcloud = CCSprite::create("background/topcloud.png");
		topcloud->setAnchorPoint(ccp(0.0f, 1.0f));
		topcloud->setPosition(ccp(leftTop.x,leftTop.y + 20));
		this->addChild(topcloud,0);

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
		lefttopLeaf->runAction(CCRepeatForever::create(rotation));

		cloud = CCSprite::create("background/cloud.png");
		cloud->setAnchorPoint(ccp(0.0, 1.0f));
		cloud->setPosition(ccp(200,1120));
		this->addChild(cloud);
		this->schedule(schedule_selector(GameScene::cloudSchedule));
	}
	else if (packageNum == 2){
	
	}

}

//云的漂浮动画实现
void GameScene::cloudSchedule(float dt)
{
	if (cloud->getPosition().x < -cloud->getContentSize().width ){
		cloud->setPosition(ccp(screenWidth, cloud->getPosition().y));
	}
	else{
		cloud->setPosition(ccp(cloud->getPosition().x - 0.6, cloud->getPosition().y));
	}

	if (topcloud->getPosition().x < -topcloud->getContentSize().width){
		topcloud->setPosition(ccp(screenWidth, topcloud->getPosition().y));
	}
	else{
		topcloud->setPosition(ccp(topcloud->getPosition().x - 0.2, topcloud->getPosition().y));
	}
}

void GameScene::addStarBord()
{
	for (int i = 0; i < 3; i++)
	{
		starBords[i] = CCSprite::create("label/starIcon.png");
		starBords[i]->setPosition(ccp(100 + i * 50, screenHeight - 40));
		starBords[i]->setScale(0.3);
		this->addChild(starBords[i], 1); 
	}
}

void GameScene::playBackgroundMusic()
{
	if (RunTimeData::getInstance()->hasMusic && !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(AUDIO_GAME);
	}
}

void GameScene::addDashLine()
{
	line = CCProgressTimer::create(CCSprite::create("line.png"));
	line->setAnchorPoint(ccp(0, 0));
	line->setType(kCCProgressTimerTypeBar);
	line->setBarChangeRate(ccp(1, 0));
	line->setPercentage(0);
	line->setMidpoint(ccp(0, 0));
	line->setPosition(ccp(0, 0));
	this->addChild(line, 10);
}

void GameScene::addBlades()
{
	blade = NULL;
	blades = new CCArray;
	blades->initWithCapacity(3);
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("streak.png");
	for (int i = 0; i < 3; i++)
	{
		CCBlade* temp_blade = CCBlade::bladeWithMaximumPoint(50);
		temp_blade->setAutoDim(false);
		temp_blade->setTexture(texture);
		this->addChild(temp_blade, 2);
		blades->addObject(temp_blade);
	}
}

//添加道具菜单UI
void GameScene::addToolsMenu()
{
	bar = CCSprite::create("button/bar.png");
	bar->setAnchorPoint(ccp(0.0f, 0.5f));
	bar->setPosition(ccp(-260, 70));
	this->addChild(bar, 1);

	CCString* anniNumStr = CCString::createWithFormat("%d", RunTimeData::getInstance()->annihilatorNum);
	CCString* fingerNumStr = CCString::createWithFormat("%d", 0);
	anniNum = CCLabelBMFont::create(anniNumStr->getCString(), "label/number.fnt");
	fingerNum = CCLabelBMFont::create(fingerNumStr->getCString(), "label/number.fnt");

	CCMenuItemImage* arrow = CCMenuItemImage::create("button/bar_right_normal.png", "button/bar_left_normal.png", this, menu_selector(GameScene::toolsMenuButtonCallback));
	//可以将资源名称添加到数组中用循环生成
	CCMenuItemImage* shop = CCMenuItemImage::create("button/bar_shop_normal.png", "button/bar_shop_pressed.png", this, menu_selector(GameScene::toolButtonCallBack));
	finger = CCMenuItemImage::create("button/bar_finger_normal.png", "button/bar_finger_pressed.png", this, menu_selector(GameScene::toolButtonCallBack));
	fire_extinguisher = CCMenuItemImage::create("button/bar_fire_extinguisher_normal.png", "button/bar_fire_extinguisher_pressed.png", this, menu_selector(GameScene::toolButtonCallBack));
	shop->setTag(100);
	fire_extinguisher->setTag(101);
	toolMenu = CCMenu::create(arrow, finger, fire_extinguisher, shop, NULL);
	toolMenu->setPosition(CCPointZero);
	toolMenu->setAnchorPoint(CCPointZero);
	toolMenu->alignItemsHorizontally();
	
	arrow->setPosition(ccp(40, 70));
	finger->setPosition(ccp(-30, 70));
	fire_extinguisher->setPosition(ccp(-100, 70));
	shop->setPosition(ccp(-170, 70));

	finger->addChild(fingerNum, 1);
	fingerNum->setPosition(ccp(finger->getContentSize().width / 2, 10));
	fingerNum->setScale(0.7f);

	fire_extinguisher->addChild(anniNum, 1);
	anniNum->setPosition(ccp(fire_extinguisher->getContentSize().width/2,10));
	anniNum->setScale(0.7f);

	this->addChild(toolMenu,1);
}

//添加游戏控制控件UI
void GameScene::addGameControlButton()
{
	CCMenuItemImage* pause = CCMenuItemImage::create("button/pause_normal.png", "button/pause_pressed.png", this, menu_selector(GameScene::pauseButtonCallback));
	CCMenuItemImage* reset = CCMenuItemImage::create("button/restart_normal.png", "button/restart_pressed.png", this, menu_selector(GameScene::resetButtonCallback));
	CCMenu* gameControlMenu = CCMenu::create(pause, reset, NULL);
	gameControlMenu->setAnchorPoint(CCPointZero);
	gameControlMenu->setPosition(CCPointZero);
	gameControlMenu->alignItemsHorizontally();
	pause->setPosition(ccp(screenWidth - 60, screenHeight - 50));
	reset->setPosition(ccp(screenWidth - 140, screenHeight - 50));
	pause->setScale(0.7f);
	reset->setScale(0.7f);
	this->addChild(gameControlMenu, 2);
}

//游戏处理逻辑
void GameScene::update(float delta)
{
	world->Step(0.03f, 2, 10);
	//world->Step(0.01f, 2, 10);
	this->checkAndSliceObjects();
	//因为在暂停恢复后再次暂停时PauseLayer正在删除，导致this->scheduleupdate()报错，
	//所以将PauseLayer的移除移动到update()函数进行处理
	if (isPausing){
		isPausing = false;
		this->removeChildByTag(100);
		if (RunTimeData::getInstance()->hasMusic){
			SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		}
	}

	if (blade && blade->getPath()->count() > 3)
	{
		deltaRemainder = delta * 60 * 1.2f;
		int pop = (int)(deltaRemainder + 0.5f);
		deltaRemainder -= pop;
		blade->pop(pop);
	}
	gameLogic();
}

bool GameScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	startTouchPoint = pTouch->getLocation();
	CCObject* temp;
	CCARRAY_FOREACH(blades, temp)
	{
		CCBlade* temp_blade = (CCBlade*)temp;
		if (temp_blade->getPath()->count() == 0)
		{
			blade = temp_blade;
			blade->push(startTouchPoint);
			break;
		}
	}

	if (isUsingTools){
		//使用道具处理逻辑
		if (usingToolNum == 1)
		{
			for (int i = 0; i < bombs.count(); i++)
			{
				Bomb* b = (Bomb*)bombs.objectAtIndex(i);

				if (ccpDistance(b->getPhyPos(), startTouchPoint) < 60  && RunTimeData::getInstance()->annihilatorNum > 0)
				{
					CCSprite* smoke = CCSprite::create("label/fog.png");
					smoke->setPosition(b->getPhyPos());
					smoke->setScale(0.0f);
					this->addChild(smoke, 2);
					CCActionInterval* zoomOut = CCScaleTo::create(1.5f, 1.3f);
					CCActionInterval* fadeOut = CCFadeOut::create(1.5f);
					smoke->runAction(CCSpawn::create(zoomOut, fadeOut, NULL));
					world->DestroyBody(b->m_pBody);
					b->removeFromParentAndCleanup(true);
					bombs.removeObjectAtIndex(i);
					RunTimeData::getInstance()->annihilatorNum--;
					if (bombs.count() == 0){
						isShowResult = true;
						this->scheduleOnce(schedule_selector(GameScene::winSchedule), 2.0f);
					}
					
				}
			}
		
			usingToolNum = 0;
			fire_extinguisher->setScale(1.0f);
			fire_extinguisher->setPosition(ccp(-100, 70));
			CCString* numStr = CCString::createWithFormat("%d",RunTimeData::getInstance()->annihilatorNum);
			anniNum->setString(numStr->getCString());
			LocalData::writeRuntimeDataToLocal();
		}
	}

	line->setPosition(startTouchPoint);
	isTouchBegan = true;

	//addNewSpriteAtPosition(startTouchPoint);
	return true;
}

void GameScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint currentTouchPoint = pTouch->getLocation();
	float distance = ccpDistance(startTouchPoint, currentTouchPoint);
	float cosValue = (currentTouchPoint.x - startTouchPoint.x) / distance;
	float sinValue = (currentTouchPoint.y - startTouchPoint.y) / distance;
	float degree = asin(sinValue) * 180 / M_PI;
	if (startTouchPoint.x > currentTouchPoint.x)
	{
		line->setAnchorPoint(ccp(1.0, 0.5f));
		line->setMidpoint(ccp(1, 1));
		line->setRotation(degree);
	}
	else
	{
		line->setRotation(-degree);
		line->setAnchorPoint(ccp(0, 0.5f));
		line->setMidpoint(ccp(0, 0));
	}
	float full = sqrt(screenWidth * screenWidth + screenHeight * screenHeight);
	float pre = distance / 15.0f;
	line->setPercentage(pre);
	isTouchMoving = true;
}

void GameScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	endTouchPoint = pTouch->getLocation();
	if (ccpLengthSQ(ccpSub(startTouchPoint, endTouchPoint)) > 25){
		//回调_raycastCallback的ReportFixture，参数为触摸的起始点和结束点
		world->RayCast(_raycastCallback, b2Vec2(startTouchPoint.x / PTM_RATIO, startTouchPoint.y / PTM_RATIO),
			b2Vec2(endTouchPoint.x / PTM_RATIO, endTouchPoint.y / PTM_RATIO));
		world->RayCast(_raycastCallback, b2Vec2(endTouchPoint.x / PTM_RATIO, endTouchPoint.y / PTM_RATIO),
			b2Vec2(startTouchPoint.x / PTM_RATIO, startTouchPoint.y / PTM_RATIO));
		if (RunTimeData::getInstance()->hasMusic){
			SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CUTTING);
		}
	}

	pTouch->getLocation();
	blade->push(endTouchPoint);
	blade->dim(true);

	isTouchMoving = false;
	line->setPosition(-300, -300);
	line->setPercentage(0);
}

void GameScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

void GameScene::registerWithTouchDispatcher(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}


//切割效果主体函数
bool GameScene::splitPolygonSprite(PolygonSprite* sprite)
{
	bool flag = false;

	PolygonSprite* newSprite1;
	PolygonSprite* newSprite2;

	//获得刚体指针
	b2Fixture* originalFixture = sprite->getBody()->GetFixtureList();
	//获得形状指针
	b2PolygonShape* originalPolygon = (b2PolygonShape*)originalFixture->GetShape();
	//获取顶点数
	int vertexCount = originalPolygon->GetVertexCount();
	//
	float determinant;
	int i;

	//切割后第一个多边形的顶点坐标数组
	b2Vec2* sprite1Vertices = new b2Vec2[24];
	//切割后第二个多边形的顶点坐标数组
	b2Vec2* sprite2Vertices = new b2Vec2[24];
	//排序后的第一个顶点坐标数组
	b2Vec2* sprite1VerticesSorted;
	//排序后的第二个顶点坐标数组
	b2Vec2* sprite2VerticesSorted;

	int sprite1VertexCount = 0;
	int sprite2VertexCount = 0;

	//获得第一个多边形的顶点
	sprite1Vertices[sprite1VertexCount++] = sprite->getEntryPoint();
	sprite1Vertices[sprite1VertexCount++] = sprite->getExitPoint();
	sprite2Vertices[sprite2VertexCount++] = sprite->getEntryPoint();
	sprite2Vertices[sprite2VertexCount++] = sprite->getExitPoint();

	for (i = 0; i < vertexCount; i++){
		//得到每一个顶点
		b2Vec2 point = originalPolygon->GetVertex(i);

		//计算与光线进入点的插值
		b2Vec2 diffFromEntryPoint = point - sprite->getEntryPoint();
		//计算与光线退出点的插值
		b2Vec2 diffFromExitPoint = point - sprite->getExitPoint();

		//判断是否为顶点，如果为顶点则忽略
		if ((diffFromEntryPoint.x == 0 && diffFromEntryPoint.y == 0) || (diffFromExitPoint.x == 0 && diffFromExitPoint.y == 0)){

		}
		else{
			//计算当前顶点应该属于哪个新物体顶点数组
			determinant = calculate_determinant_2x3(sprite->getEntryPoint().x,
				sprite->getEntryPoint().y,
				sprite->getExitPoint().x,
				sprite->getExitPoint().y,
				point.x,
				point.y);
			if (determinant > 0){
				sprite1Vertices[sprite1VertexCount++] = point;
			}
			else{
				sprite2Vertices[sprite2VertexCount++] = point;
			}
		}
	}

	//对两个新的顶点数组进行重排
	sprite1VerticesSorted = Box2dHelper::arrangeVertices(sprite1Vertices, sprite1VertexCount);
	sprite2VerticesSorted = Box2dHelper::arrangeVertices(sprite2Vertices, sprite2VertexCount);

	//判断重排后的数组是否满足box2d的物体建立规则
	bool sprite1VerticesAcceptable = Box2dHelper::areVerticesAcceptable(sprite1VerticesSorted, sprite1VertexCount);
	bool sprite2VerticesAcceptable = Box2dHelper::areVerticesAcceptable(sprite2VerticesSorted, sprite2VertexCount);
	//满足建立规则则进行切割，以下是切割动作
	if (sprite1VerticesAcceptable && sprite2VerticesAcceptable){
		//_cuts++;
		//_cutsLabel->setString(CCString::createWithFormat("%d",_cuts)->getCString());
		b2Vec2 worldEntry = sprite->getBody()->GetWorldPoint(sprite->getEntryPoint());
		b2Vec2 worldExit = sprite->getBody()->GetWorldPoint(sprite->getExitPoint());
		float angle = ccpToAngle(ccpSub(ccp(worldExit.x, worldExit.y), ccp(worldEntry.x, worldEntry.y)));
		CCPoint vector1 = ccpForAngle(angle + 1.570796f);
		CCPoint vector2 = ccpForAngle(angle - 1.570796f);
		float midX = midpoint(worldEntry.x, worldExit.x);
		float midY = midpoint(worldEntry.y, worldExit.y);

		b2Body* body1 = Box2dHelper::createBodyX(world, sprite->getBody()->GetPosition(),
			sprite->getBody()->GetAngle(),
			sprite1VerticesSorted,
			sprite1VertexCount,
			originalFixture->GetDensity(),
			originalFixture->GetFriction(),
			originalFixture->GetRestitution());
		newSprite1 = PolygonSprite::spriteWithTexture(sprite->getTexture(), body1, false);
		newSprite1->setTag(enableCut);
		 		newSprite1->getBody()->ApplyLinearImpulse(b2Vec2(1.0 * body1->GetMass() * vector1.x, 1.0 * body1->GetMass() * vector1.y),
		 			b2Vec2(midX, midY));
		this->addChild(newSprite1);

		b2Body* body2 = Box2dHelper::createBodyX(world, sprite->getBody()->GetPosition(),
			sprite->getBody()->GetAngle(),
			sprite2VerticesSorted,
			sprite2VertexCount,
			originalFixture->GetDensity(),
			originalFixture->GetFriction(),
			originalFixture->GetRestitution());
		newSprite2 = PolygonSprite::spriteWithTexture(sprite->getTexture(), body2, false);
		 		newSprite2->getBody()->ApplyLinearImpulse(b2Vec2(1.0 * body2->GetMass() * vector2.x, 1.0 * body2->GetMass() * vector2.y),
		 			b2Vec2(midX, midY));
		newSprite2->setTag(enableCut);
		this->addChild(newSprite2);


		if (sprite->getOriginal()){
			b2Vec2 convertedWorldEntry = b2Vec2(worldEntry.x*PTM_RATIO, worldEntry.y*PTM_RATIO);
			b2Vec2 convertedWorldExit = b2Vec2(worldExit.x*PTM_RATIO, worldExit.y*PTM_RATIO);
			float midX = midpoint(convertedWorldEntry.x, convertedWorldExit.x);
			float midY = midpoint(convertedWorldEntry.y, convertedWorldExit.y);
			//sprite->getSplurt()->setPosition(ccp(midX, midY));
			//sprite->getSplurt()->resetSystem();
			sprite->deactivateCollisions();
			sprite->setPosition(ccp(-1000, -1000));
			sprite->setSliceEntered(false);
			sprite->setSliceExited(false);
			sprite->setEntryPoint(b2Vec2(0, 0));
			sprite->setExitPoint(b2Vec2(0, 0));
			// 			if(sprite->getType() == kTypeBomb){
			// 				//SimpleAudioEngine::sharedEngine()->playEffect("explosion.mp3");
			// 				//this->subtractLife();
			// 			}else{
			// 				//SimpleAudioEngine::sharedEngine()->playEffect("squash.mp3");
			// 			}
		}
		else{
			flag = true;
			//SimpleAudioEngine::sharedEngine()->playEffect("smallcut.mp3");
		}
	}
	else{
		sprite->setSliceEntered(false);
		sprite->setSliceExited(false);
	}

	delete[] sprite1VerticesSorted;
	delete[] sprite2VerticesSorted;
	delete[] sprite1Vertices;
	delete[] sprite2Vertices;
	return flag;
}

//检查切割的废弃对象进行删除
void GameScene::checkAndSliceObjects()
{
	double curTime = (double)millisecondNow();

	CCArray* _oldBodyToDel = CCArray::create();

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()){
		if (b->GetUserData() != NULL){
			CCNode* node = (CCNode*)b->GetUserData();

			int type = node->getTag();
			if (type == enableCut){
				PolygonSprite* sprite = (PolygonSprite*)node;
				if (sprite->getSliceEntered() && curTime > sprite->getSliceEntryTime()){
					sprite->setSliceEntered(false);
					sprite->setSliceExited(false);
				}
				else if (sprite->getSliceEntered() && sprite->getSliceExited()){
					bool flag = this->splitPolygonSprite(sprite);
					if (flag){
						_oldBodyToDel->addObject(sprite);
					}
				}
			}
		}
	}

	for (unsigned int i = 0; i < _oldBodyToDel->count(); i++){
		PolygonSprite* sprite = (PolygonSprite*)_oldBodyToDel->objectAtIndex(i);
		world->DestroyBody(sprite->getBody());
		this->removeChild(sprite, true);
	}
	_oldBodyToDel->removeAllObjects();
}

void GameScene::addNewSpriteAtPosition(CCPoint p)
{
	Wood* wood = Wood::createWithFileName(world, p, 10, "elements/wood64x64.png");
	this->addChild(wood);
}

//从xml文件中载入关卡
void GameScene::loadLevelFromFile(const char* levelFile)
{
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(levelFile);
	CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());
	CCArray* elementArr = (CCArray*)dict->objectForKey("Level");
	CCDictionary* metaData = (CCDictionary*)dict->objectForKey("Metadata");
	std::string bodyPlistFilePath = metaData->valueForKey("BodyPlist")->getCString();
	GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile(bodyPlistFilePath.c_str());
	CCDictElement* pElement = NULL;
	CCDictionary* frameDict = NULL;
	for (int i = 0; i < elementArr->count(); i++)
	{
		frameDict = (CCDictionary*)elementArr->objectAtIndex(i);
		CCPoint point = CCPointFromString(frameDict->valueForKey("Position")->getCString());
		const char* fileName = frameDict->valueForKey("FileName")->getCString();
		const char* type = frameDict->valueForKey("Type")->getCString();
		int rotation = frameDict->valueForKey("Rotation")->intValue();
		CCNode* sprite = NULL;
		ElementType t = getElementTypeByString(type);
		int r;
		switch (t)
		{
		case eHero:
			r = frameDict->valueForKey("Radius")->intValue();
			sprite = Hero::createWithFileName(world, fileName, point, (float)r);
			hero = (Hero*)sprite;
			break;
		case eBall:
			r = frameDict->valueForKey("Radius")->intValue();
			sprite = Ball::createWithFileName(world, fileName, point, (float)r);
			break;
		case eWood:
			sprite = SoftElement::createWithFilename(world, fileName, point, rotation);
			break;
		case eSurface:
			sprite = Surface::createWithFilename(world, fileName, point, rotation);
			break;
		case eBomb:
			r = frameDict->valueForKey("Radius")->intValue();
			sprite = Bomb::createWithFileName(world, fileName, point, (float)r);
			bombs.addObject(sprite);
			break;
		case eStar:
			sprite = CCSprite::create("label/star.png");
			sprite->setPosition(point);
			sprite->setScale(0.3f);
			stars.addObject(sprite);
			break;
		default:
			break;
		}

		if (sprite != NULL)
		{
			this->addChild(sprite, 2);
		}
	}
}

ElementType GameScene::getElementTypeByString(const char* str)
{
	if (strcmp(str, "wood") == 0)
	{
		return eWood;
	}
	else if (strcmp(str, "hero") == 0)
	{
		return eHero;
	}
	else if (strcmp(str, "ball") == 0)
	{
		return eBall;
	}
	else if (strcmp(str, "bomb") == 0)
	{
		return eBomb;
	}
	else if (strcmp(str, "surface") == 0)
	{
		return eSurface;
	}
	else if (strcmp(str, "star") == 0){
		return eStar;
	}

	return eWood;
}

void GameScene::setToolsMenuVisible()
{
	CCActionInterval* moveOut = CCMoveTo::create(0.3f, ccp(-20, 70));
	CCActionInterval* show = CCEaseElasticIn::create(moveOut);
	bar->runAction(moveOut);
	CCActionInterval* moveMenuIn = CCMoveBy::create(0.2f, ccp(230, 0));
	toolMenu->runAction(moveMenuIn);

}

void GameScene::setToolsMenuInvisible()
{
	CCActionInterval* moveIn = CCMoveTo::create(0.2f, ccp(-260, 70));
	CCActionInterval* show = CCEaseElasticOut::create(moveIn);
	bar->runAction(moveIn);
	CCActionInterval* menuMoveIn = CCMoveBy::create(0.2f, ccp(-230, 0));
	toolMenu->runAction(menuMoveIn);
}

void GameScene::toolsMenuButtonCallback(CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic)
	{
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	
	CCMenuItemImage* usingTools = (CCMenuItemImage*)sender;
	if (isUsingTools){
		isUsingTools = false;
		usingTools->unselected();
		setToolsMenuInvisible();
	}
	else{
		isUsingTools = true;
		usingTools->selected();
		setToolsMenuVisible();
	}
}

void GameScene::toolButtonCallBack(CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic){
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}
	
	int tag = ((CCNode*)sender)->getTag();
	if (tag == 100)
	{
		CCScene* shopScene = ShopScene::scene();
		CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, shopScene));
	}
	else if (tag == 101)
	{
		if (usingToolNum != 1)
		{
			fire_extinguisher->setScale(1.3f);
			fire_extinguisher->setPosition(ccp(-100, 90));
			usingToolNum = 1;		//开启灭火器模式
		}
		else if (usingToolNum == 1)
		{
			fire_extinguisher->setScale(1);
			fire_extinguisher->setPosition(ccp(-100, 70));
			usingToolNum = 0;		//关闭灭火器模式
		}
	}
	else if (tag = 102)
	{
		
	}
}

void GameScene::pauseButtonCallback(cocos2d::CCObject* sender)
{
	if (!isShowResult && !isPausing){
		CCMenuItemImage* pause = (CCMenuItemImage*)sender;
		isPausing = true;
		//pause game
		if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		{
			SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		}
		PauseLayer* layer = PauseLayer::create();
		layer->setTag(100);
		this->addChild(layer, 3);
		this->unschedule(schedule_selector(GameScene::update));
		//this->unscheduleUpdate();
	}
}

void GameScene::resetButtonCallback(cocos2d::CCObject* sender)
{
	if (RunTimeData::getInstance()->hasMusic){
		SimpleAudioEngine::sharedEngine()->playEffect(AUDIO_CLICK);
	}

	CCScene* gameScene = GameScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, gameScene));
}

void GameScene::gameLogic()
{
	if (isOutOfScreen(hero->m_pBody) && isShowResult == false){
		this->scheduleOnce(schedule_selector(GameScene::resetLevelSchedule), 2.4f);
	}

	//炸弹碰撞检测
	for (int i = 0; i < bombs.count(); i++)
	{
		Bomb* b = (Bomb*)bombs.objectAtIndex(i);

		if (isOutOfScreen(b->m_pBody) && isShowResult == false){
			this->scheduleOnce(schedule_selector(GameScene::resetLevelSchedule), 2.2f);
		}

		if (ccpDistance(b->getPhyPos(), hero->getPhyPos()) < 60)
		{
			CCSprite* smoke = CCSprite::create("label/fog.png");
			smoke->setPosition(b->getPhyPos());
			smoke->setScale(0.0f);
			this->addChild(smoke, 2);
			CCActionInterval* zoomOut = CCScaleTo::create(1.5f, 1.3f);
			CCActionInterval* fadeOut = CCFadeOut::create(1.5f);
			smoke->runAction(CCSpawn::create(zoomOut, fadeOut, NULL));
			world->DestroyBody(b->m_pBody);
			b->removeFromParentAndCleanup(true);
			bombs.removeObjectAtIndex(i);
			if (bombs.count() == 0 && !isPausing){
				isShowResult = true;
				this->scheduleOnce(schedule_selector(GameScene::winSchedule), 2.0f);
			}
		}
		
	}

	//星星碰撞检测
	for (int i = 0; i < stars.count(); i++)
	{
		CCSprite* sprite = (CCSprite*)stars.objectAtIndex(i);

		if (ccpDistance(hero->getPhyPos(), sprite->getPosition()) < 30){
			CCActionInterval* fly = CCMoveTo::create(0.3f, starBords[starNum]->getPosition());

			if (RunTimeData::getInstance()->hasMusic){
				SimpleAudioEngine::sharedEngine()->playEffect(star_catchAudios[starNum]);
			}
			starNum++;
			sprite->runAction(fly);
			stars.removeObjectAtIndex(i);
		}
	}
}

void GameScene::guideLogic()
{
	GuideLayer* guideLayer = GuideLayer::create();
	this->addChild(guideLayer, 3);
	this->unschedule(schedule_selector(GameScene::update));
	//this->unscheduleUpdate();
}

void GameScene::winSchedule(float dt)
{
	isShowResult = true;
	int level = RunTimeData::getInstance()->selectLevel - 1;
	char l = RunTimeData::getInstance()->levelStars[level];
	RunTimeData::getInstance()->currentLevelStarNum = starNum;
	if ((l - '0') < starNum){
		RunTimeData::getInstance()->startNum += starNum;
		RunTimeData::getInstance()->levelStars[level] = starNum + '0';
		RunTimeData::getInstance()->passedLevel++;
		LocalData::writeRuntimeDataToLocal();
	}

	ResultLayer* result = ResultLayer::create();
	this->addChild(result, 3);

	this->unschedule(schedule_selector(GameScene::update));
	//this->unscheduleUpdate();
}

GameScene::~GameScene()
{
	if (world != NULL){
		b2Body* node = world->GetBodyList();
		while (node)
		{
			b2Body* b = node;
			node = node->GetNext();
			world->DestroyBody(b);
		}
		delete world;
	}
}

void GameScene::resetLevelSchedule(float dt)
{
	this->unschedule(schedule_selector(GameScene::update));
	//this->unscheduleUpdate();
	CCScene* gameScene = GameScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.4f, gameScene));
}

bool GameScene::isOutOfScreen(b2Body* body)
{
	float x = body->GetPosition().x * PTM_RATIO;
	float y = body->GetPosition().y * PTM_RATIO;
	if (x < 0 || x > screenWidth || y < 0 || y > screenHeight){
		return true;
	}
	else{
		return false;
	}
}

void GameScene::keyBackClicked(void)
{
	CCLOG("back click");
	if (!isShowResult && !isPausing){
		isPausing = true;
		//pause game
		if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		{
			SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		}
		PauseLayer* layer = PauseLayer::create();
		layer->setTag(100);
		this->addChild(layer, 3);
		this->unschedule(schedule_selector(GameScene::update));
		//this->unscheduleUpdate();
	}
}