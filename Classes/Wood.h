#ifndef Wood_h__
#define Wood_h__

#include "PolygonSprite.h"
#include "cocos2d.h"

class Wood : public PolygonSprite{
	bool initWithFile(b2World* world,cocos2d::CCPoint pos, float rotation, const char* pFileName);
	bool initWithTexture(b2World* world, cocos2d::CCTexture2D* texture, cocos2d::CCPoint pos, float rotation, cocos2d::CCRect rect);
public:
	static Wood *createWithFileName(b2World* world, cocos2d::CCPoint pos, float rotation, const char* pfileName);
	static Wood *createWithTexture(b2World* world, cocos2d::CCTexture2D* texture, cocos2d::CCPoint pos, float rotation, const char * textureName);
	virtual b2Body* createBody(b2World* world, b2Vec2 position, float rotation, b2Vec2* vertices, int32 count, float density, float friction, float restitution);
};


#endif // Wood_h__