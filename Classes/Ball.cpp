#include "Ball.h"
#include "Macros.h"

USING_NS_CC;

Ball * Ball::createWithFileName(b2World* world,const char* pfileName,cocos2d::CCPoint pos, float radius)
{
	Ball* ball = new Ball;
	ball->initWithFile(pfileName);
	ball->setPosition(pos);
	ball->setTag(unableCut);

	b2CircleShape shape;
	shape.m_radius = radius/PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.density = 9.0f;
	fixtureDef.friction = 0.4f;
	fixtureDef.restitution = 0.5f;
	fixtureDef.shape = &shape;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x/PTM_RATIO,pos.y/PTM_RATIO);
	bodyDef.angle = 0;

	b2Body *body = world->CreateBody(&bodyDef);
	body->SetUserData(ball);

	body->CreateFixture(&fixtureDef);

	ball->setPhysicsBody(body,true);
	if(ball){
		ball->autorelease();
		return ball;
	}
	CC_SAFE_DELETE(ball);
	return NULL;
}

