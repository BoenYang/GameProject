#include "Hero.h"
#include "Macros.h"

USING_NS_CC;
Hero * Hero::createWithFileName(b2World* world, const char* pfileName, cocos2d::CCPoint pos, float radius)
{
	Hero* hero = new Hero;
	hero->initWithFile(pfileName);
	hero->setPosition(pos);
	hero->setTag(unableCut);

	b2CircleShape shape;
	shape.m_radius = radius / PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.3f;
	fixtureDef.friction = 0.4f;
	fixtureDef.restitution = 0.1f;
	fixtureDef.shape = &shape;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);
	bodyDef.angle = 0;

	b2Body *body = world->CreateBody(&bodyDef);
	body->SetUserData(hero);

	body->CreateFixture(&fixtureDef);

	hero->setPhysicsBody(body, true);
	if (hero){
		hero->autorelease();
		return hero;
	}
	CC_SAFE_DELETE(hero);
	return NULL;
}