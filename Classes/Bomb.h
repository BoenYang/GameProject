#ifndef Bomb_h__
#define Bomb_h__

#include "cocos2d.h"
#include "PhysicsSprite.h"

class Bomb : public PhysicsSprite
{
public:
	static Bomb *createWithFileName(b2World* world, const char* pfileName,cocos2d::CCPoint pos, float radius);
};

#endif // Bomb_h__
