#ifndef Surface_h__
#define Surface_h__
#include "cocos2d.h"
#include "PhysicsSprite.h"


class Surface : public cocos2d::CCSprite {
public:
	static Surface* createWithFilename(b2World* world, const char* filename,cocos2d::CCPoint pos, float rotation);

};

#endif // Surface_h__