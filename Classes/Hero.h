#ifndef Hero_h__
#define Hero_h__

#include "cocos2d.h"
#include "PhysicsSprite.h"

class Hero : public PhysicsSprite{
public:
	static Hero *createWithFileName(b2World* world, const char* pfileName, cocos2d::CCPoint pos, float radius);
};

#endif // Hero_h__