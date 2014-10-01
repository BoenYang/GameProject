#ifndef Ball_h__
#define Ball_h__

#include "cocos2d.h"
#include "PhysicsSprite.h"

class Ball : public PhysicsSprite{
public:
	static Ball *createWithFileName(b2World* world, const char* pfileName,cocos2d::CCPoint pos, float radius);
};

#endif // Ball_h__