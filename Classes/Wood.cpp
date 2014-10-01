#include "Wood.h"
#include "Box2dHelper.h"
#include "TextureHelper.h"
#include "Macros.h"
#include <math.h>
USING_NS_CC;

bool Wood::initWithFile(b2World* world, CCPoint pos, float rotation, const char* pFileName)
{
	CCSprite* sprite = CCSprite::create(pFileName);
	float w = sprite->getContentSize().width;
	float h = sprite->getContentSize().height;
	b2Vec2 vertices[] = {
		b2Vec2( 0.0f/PTM_RATIO, h /PTM_RATIO),
		b2Vec2( 0.0f/PTM_RATIO,0.0f /PTM_RATIO),
		b2Vec2( w /PTM_RATIO, 0.0f/PTM_RATIO),
		b2Vec2( w /PTM_RATIO, h/PTM_RATIO),
	};
	
	float radius = sqrt(w*w / 4 + h*h / 4);
	float alph = asin((w / 2) / radius) * 180/ b2_pi;
	float beta = rotation - alph;
	float add_x = sin(beta*M_PI/180)*radius;
	float add_y = cos(beta*M_PI/180)*radius;

	b2Body* body = NULL;
	body = createBody(world, b2Vec2((pos.x + add_x) / PTM_RATIO, (pos.y - add_y) / PTM_RATIO), rotation, vertices,
		4, 2.0f, 0.7f, 0.2f);

	body->SetUserData(this);
	if(initWithFileBodyOriginal(pFileName,body,true)){
		return true;
	}
	return false;
}

bool Wood::initWithTexture(b2World* world, CCTexture2D *texture, CCPoint pos, float rotation, CCRect rect)
{
	float x = rect.origin.x;
	float y = rect.origin.y;
	float w = rect.size.width;
	float h = rect.size.height;

	b2Vec2 vertices[] = {
		b2Vec2( x/PTM_RATIO, y/PTM_RATIO),
		b2Vec2( x/PTM_RATIO, (y - h)/PTM_RATIO),
		b2Vec2( (x + w)/PTM_RATIO, (y - h)/PTM_RATIO),
		b2Vec2( (x + w)/PTM_RATIO, y/PTM_RATIO),
	};	
	b2Vec2* v = Box2dHelper::arrangeVertices(vertices, 4);
	float tempx = x + w / 2;
	float tempy = y - h / 2;
	float radius = sqrt(tempx*tempx + tempy*tempy);
	float alph = atan(tempx / tempy) * 180.0f / b2_pi;
	float beta = rotation - alph;
	float add_x = sin(beta*b2_pi / 180.0f)*radius;
	float add_y = cos(beta*b2_pi / 180.0f)*radius;
	b2Body* body = createBody(world, b2Vec2((pos.x + add_x) / PTM_RATIO, (pos.y - add_y) / PTM_RATIO), rotation, v,
			4, 2.0f, 0.7f, 0.2f);
	body->SetUserData(this);
	if(initWithTextureBodyOriginal(texture,body,true,rect)){
		return true;
	}
	return false;
}

Wood * Wood::createWithFileName(b2World* world, CCPoint pos, float rotation, const char* pfileName)

{
	Wood *wood = new Wood;
	wood->setTag(enableCut);
	if (wood && wood->initWithFile(world, pos, rotation, pfileName)){
		wood->autorelease();
		return wood;
	}
	CC_SAFE_DELETE(wood);
	return NULL;
}

Wood * Wood::createWithTexture(b2World* world, CCTexture2D* texture, CCPoint pos, float rotation, const char * textureName)
{
	Wood *wood = new Wood;
	wood->setTag(enableCut);
	CCRect rect = TextureHelper::shareTextureHelper()->getRectByName(textureName);
	if (wood && wood->initWithTexture(world, texture, pos, rotation, rect)){
		wood->autorelease();
		return wood;
	}
	CC_SAFE_DELETE(wood);
	return NULL;
}

b2Body* Wood::createBody(b2World* world, b2Vec2 position, float rotation, b2Vec2* vertices, int32 count, float density, float friction, float restitution)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = position;
	bodyDef.angle = rotation * b2_pi / 180;
	b2Body *body = world->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;

	b2PolygonShape shape;
	shape.Set(vertices,count);
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);

	return body;
}


