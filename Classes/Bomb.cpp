#include "Bomb.h"
#include "Macros.h"
USING_NS_CC;

Bomb * Bomb::createWithFileName(b2World* world, const char* pfileName,CCPoint pos, float radius)
{
	Bomb* bomb = new Bomb();
	bomb->initWithFile(pfileName);
	bomb->setPosition(pos);
	bomb->setTag(unableCut);

	b2CircleShape shape;
	shape.m_radius = radius/PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.1f;
	fixtureDef.friction = 0.2f;
	fixtureDef.restitution = 0.4f;
	fixtureDef.shape = &shape;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x/PTM_RATIO,pos.y/PTM_RATIO);
	bodyDef.angle = 0;

	b2Body *body = world->CreateBody(&bodyDef);
	body->SetUserData(bomb);

	body->CreateFixture(&fixtureDef);

	bomb->setPhysicsBody(body,true);
	if(bomb){
		bomb->autorelease();
		return bomb;
	}
	CC_SAFE_DELETE(bomb);
	return NULL;
}

