#ifndef CutAbleElement_h__
#define CutAbleElement_h__

#include "cocos2d.h"
#include "PolygonSprite.h"


class SoftElement : public PolygonSprite{
public:
	static SoftElement* createWithFilename(b2World* world, const char* fileName, cocos2d::CCPoint pos, float rotation);
	static SoftElement* createWithTexture(b2World* world, const char* fileName, cocos2d::CCPoint pos, float rotation,cocos2d::CCTexture2D* texture);
private:
	bool initWithFilename(b2World* world, const char* fileName, cocos2d::CCPoint pos, float rotation, const char* vexterFile);

	bool initWithTexture(b2World* world, const char* fileName, cocos2d::CCPoint pos, float rotation, cocos2d::CCTexture2D* texture);

	virtual b2Body* createBody(b2World* world, b2Vec2 position, float rotation, b2Vec2* vertices, int32 count, float density, float friction, float restitution);

};
#endif // CutAbleElement_h__