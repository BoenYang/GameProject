#include "SoftElement.h"
#include "Macros.h"
USING_NS_CC;

SoftElement* SoftElement::createWithFilename(b2World* world, const char* filename, CCPoint pos, float rotation)
{
	SoftElement* element = new SoftElement;
	std::string vexterFile = filename;
	std::string pngFile = filename;
	int p = vexterFile.find_last_of('.');
	vexterFile.replace(p, vexterFile.length() - 1, ".xml");
	element->setTag(enableCut);
	if (element->initWithFilename(world, pngFile.c_str(), pos, rotation,vexterFile.c_str())){
		element->autorelease();
		return element;
	}
	CC_SAFE_DELETE(element);
	return NULL;
}

SoftElement* SoftElement::createWithTexture(b2World* world, const char* vertexFile, CCPoint pos, float rotation,CCTexture2D* texture)
{
	SoftElement* element = new SoftElement;
	element->setTag(enableCut);
	if (element->initWithTexture(world, vertexFile, pos, rotation, texture)){
		element->autorelease();
		return element;
	}
	CC_SAFE_DELETE(element);
	return NULL;
}

bool SoftElement::initWithFilename(b2World* world, const char* filename, CCPoint pos, float rotation, const char* vertexFile)
{
	std::string vertexFileFullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(vertexFile);
	CCDictionary* vertexDict = CCDictionary::createWithContentsOfFileThreadSafe(vertexFileFullpath.c_str());
	CCArray* vertexArray = (CCArray*)vertexDict->objectForKey("VertexCoordinate");
	int vertexCount = vertexArray->count();
	b2Vec2* vertexes = new b2Vec2[vertexCount];

	for (int i = 0; i < vertexCount; i++)
	{
		CCString *s = (CCString*)vertexArray->objectAtIndex(i);
		CCPoint vertexPos = CCPointFromString(s->getCString());
		vertexes[i].Set(vertexPos.x / PTM_RATIO, vertexPos.y / PTM_RATIO);
	}
	CCSprite* sprite = CCSprite::create(filename);
	float w = sprite->getContentSize().width;
	float h = sprite->getContentSize().height;

	float radius = sqrt(w*w / 4 + h*h / 4);
	float alph = asin((w / 2) / radius) * 180 / b2_pi;
	float beta = rotation - alph;
	float add_x = sin(beta*M_PI / 180)*radius;
	float add_y = cos(beta*M_PI / 180)*radius;


	b2Body* body = NULL;
	body = createBody(world, b2Vec2((pos.x + add_x) / PTM_RATIO, (pos.y - add_y) / PTM_RATIO), rotation, vertexes,
		vertexCount, 5.0f, 0.2f, 0.2f);
	delete[] vertexes;
	vertexes = NULL;
	body->SetUserData(this);
	if (initWithFileBodyOriginal(filename, body, true)){
		return true;
	}
	return false;

}

bool SoftElement::initWithTexture(b2World* world, const char* vertexFile, CCPoint pos, float rotation, CCTexture2D* texture)
{
	std::string vertexFileFullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(vertexFile);
	CCDictionary* vertexDict = CCDictionary::createWithContentsOfFileThreadSafe(vertexFileFullpath.c_str());
	CCArray* vertexArray = (CCArray*)vertexDict->objectForKey("VertexCoordinate");
	int vertexCount = vertexArray->count();
	b2Vec2* vertexes = new b2Vec2[vertexCount];

	for (int i = 0; i < vertexCount; i++)
	{
		CCString *s = (CCString*)vertexArray->objectAtIndex(i);
		CCPoint vertexPos = CCPointFromString(s->getCString());
		vertexes[i].Set(vertexPos.x / PTM_RATIO, vertexPos.y / PTM_RATIO);
	}
	return true;
}

b2Body* SoftElement::createBody(b2World* world, b2Vec2 position, float rotation, b2Vec2* vertices, int32 count, float density, float friction, float restitution)
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
	shape.Set(vertices, count);
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);

	return body;
}
