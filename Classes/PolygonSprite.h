#ifndef __POLYGON_SPRITE_H__
#define __POLYGON_SPRITE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "PRFilledPolygon.h"
#include "Macros.h"

class PolygonSprite : public PRFilledPolygon{
public:
	bool initWithTextureBodyOriginal(cocos2d::CCTexture2D* texture, b2Body* body, bool original);
	bool initWithTextureBodyOriginal(cocos2d::CCTexture2D* texture, b2Body* body, bool original,cocos2d::CCRect rect);
	bool initWithFileBodyOriginal(const char* fileName, b2Body* body, bool original);
	bool initWithWorld(b2World* world);

	static PolygonSprite* spriteWithFile(const char* fileName, b2Body* body, bool original);
	static PolygonSprite* spriteWithTexture(cocos2d::CCTexture2D* texture, b2Body* body, bool original);
	static PolygonSprite* spriteWithWorld(b2World* world);

	virtual b2Body* createBody(b2World* world, b2Vec2 position, float rotation,
		b2Vec2* vertices, int32 count, float density, float friction, float restitution);

	void activateCollisions();
	void deactivateCollisions();

	void setPosition(const cocos2d::CCPoint& var);

	CC_SYNTHESIZE(b2Body*, _body, Body);
	CC_SYNTHESIZE(bool, _original, Original);
	CC_SYNTHESIZE(b2Vec2, _centroid, Centroid);

	CC_SYNTHESIZE(bool, _sliceEntered, SliceEntered);
	CC_SYNTHESIZE(bool, _sliceExited, SliceExited);
	CC_SYNTHESIZE_PASS_BY_REF(b2Vec2, _entryPoint, EntryPoint);
	CC_SYNTHESIZE_PASS_BY_REF(b2Vec2, _exitPoint, ExitPoint);
	CC_SYNTHESIZE(double, _sliceEntryTime, SliceEntryTime);


	CC_SYNTHESIZE(cocos2d::CCParticleSystemQuad*, _splurt, Splurt);

private:
	cocos2d::CCAffineTransform nodeToParentTransform();

	virtual void setAnchorPoint( const cocos2d::CCPoint& anchorPoint );


};

#endif