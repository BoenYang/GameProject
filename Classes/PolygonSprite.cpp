#include "PolygonSprite.h"

using namespace cocos2d;


PolygonSprite* PolygonSprite::spriteWithFile(const char* fileName, b2Body* body, bool original){
	PolygonSprite* ps = new PolygonSprite;
	if(ps && ps->initWithFileBodyOriginal(fileName, body, original)){
		ps->autorelease();
		return ps;
	}
	CC_SAFE_DELETE(ps);
	return NULL;
}

PolygonSprite* PolygonSprite::spriteWithTexture(cocos2d::CCTexture2D* texture, b2Body* body, bool original){
	PolygonSprite* ps = new PolygonSprite;
	if(ps && ps->initWithTextureBodyOriginal(texture, body, original)){
		ps->autorelease();
		return ps;
	}
	CC_SAFE_DELETE(ps);
	return NULL;
}

PolygonSprite* PolygonSprite::spriteWithWorld(b2World* world){
	PolygonSprite* ps = new PolygonSprite;
	if(ps && ps->initWithWorld(world)){
		ps->autorelease();
		return ps;
	}
	CC_SAFE_DELETE(ps);
	return NULL;
}

bool PolygonSprite::initWithFileBodyOriginal(const char* fileName, b2Body* body, bool original){
	CCAssert(fileName != NULL, "Invalid filename for sprite");
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(fileName);
	return initWithTextureBodyOriginal(texture, body, original);
}

bool PolygonSprite::initWithTextureBodyOriginal(cocos2d::CCTexture2D* texture, b2Body* body, bool original){
	bool bRet = false;
	do 
	{
		b2Fixture* originalFixture = body->GetFixtureList();
		b2PolygonShape* shape = (b2PolygonShape*)originalFixture->GetShape();
		int vertexCount = shape->GetVertexCount();
		CCArray* points = CCArray::createWithCapacity(vertexCount);
		for(int i = 0; i < vertexCount; i++){
			CCPoint p = ccp(shape->GetVertex(i).x * PTM_RATIO, shape->GetVertex(i).y * PTM_RATIO);
			CCString* temp = CCString::createWithFormat("{%f, %f}", p.x, p.y);
			points->addObject(temp);
		}

		CC_BREAK_IF(!PRFilledPolygon::initWithMember(points, texture));

		_body = body;
		_body->SetUserData(this);
		_original = original;
		_centroid = _body->GetLocalCenter();
		float width = texture->getContentSize().width;
		float height = texture->getContentSize().height; 
		this->setAnchorPoint(ccp( _centroid.x * PTM_RATIO / width,
								  _centroid.y * PTM_RATIO / height));
		//this->setAnchorPoint(ccp(0.5f,0.5f));

		_sliceEntered = false;
		_sliceExited = false;
		_entryPoint.SetZero();
		_exitPoint.SetZero();
		_sliceEntryTime = 0;
		bRet = true;
	} while (0);

	return bRet;
}

bool PolygonSprite::initWithTextureBodyOriginal( cocos2d::CCTexture2D* texture, b2Body* body, bool original,CCRect rect )
{
	bool bRet = false;
	do 
	{
		b2Fixture* originalFixture = body->GetFixtureList();
		b2PolygonShape* shape = (b2PolygonShape*)originalFixture->GetShape();
		int vertexCount = shape->GetVertexCount();
		CCArray* points = CCArray::createWithCapacity(vertexCount);
		for(int i = 0; i < vertexCount; i++){
			CCPoint p = ccp(shape->GetVertex(i).x * PTM_RATIO, shape->GetVertex(i).y * PTM_RATIO);
			CCString* temp = CCString::createWithFormat("{%f, %f}", p.x, p.y);
			points->addObject(temp);
		}

		CC_BREAK_IF(!PRFilledPolygon::initWithMember(points, texture));

		_body = body;
		_body->SetUserData(this);
		_original = original;
		_centroid = _body->GetLocalCenter();
		float width = rect.size.width;
		float height = rect.size.height; 
 		this->setAnchorPoint(ccp( _centroid.x * PTM_RATIO / width,
 			_centroid.y * PTM_RATIO / height));

		_sliceEntered = false;
		_sliceExited = false;
		_entryPoint.SetZero();
		_exitPoint.SetZero();
		_sliceEntryTime = 0;
		bRet = true;
	} while (0);

	return bRet;
}

bool PolygonSprite::initWithWorld(b2World* world){
	return false;
}

void PolygonSprite::setPosition(const CCPoint& var){
	PRFilledPolygon::setPosition(var);
	_body->SetTransform(b2Vec2(var.x / PTM_RATIO, var.y / PTM_RATIO), _body->GetAngle());
}

b2Body* PolygonSprite::createBody(b2World* world, 
										  b2Vec2 position,
										  float rotation,
										  b2Vec2* vertices, 
										  int32 count, 
										  float density, 
										  float friction, 
										  float restitution){
	
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = position;
	bodyDef.angle = rotation;
	b2Body *body = world->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
// 	fixtureDef.filter.categoryBits = 0;
// 	fixtureDef.filter.maskBits = 0;
/*	fixtureDef.isSensor = true;*/

	b2PolygonShape shape;
	shape.Set(vertices,count);
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);

	return body;
}

void PolygonSprite::activateCollisions(){
	b2Fixture *fixture = _body->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();
	filter.categoryBits = 0x0001;
	filter.maskBits = 0x0001;
	fixture->SetFilterData(filter);
}

void PolygonSprite::deactivateCollisions(){
	b2Fixture *fixture = _body->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();
	filter.categoryBits = 0;
	filter.maskBits = 0;
	fixture->SetFilterData(filter);
}

CCAffineTransform PolygonSprite::nodeToParentTransform(){
	b2Vec2 pos = _body->GetPosition();

	CCPoint anchorPointInpoints = this->getAnchorPointInPoints();
	float x = pos.x * PTM_RATIO;
	float y = pos.y * PTM_RATIO;


	if( isIgnoreAnchorPointForPosition() ){
		x += anchorPointInpoints.x;
		y += anchorPointInpoints.y;
	}

	float radians = _body->GetAngle();
	float c = cosf(radians);
	float s = sinf(radians);

	if(!anchorPointInpoints.equals(CCPointZero) ){
		x += c*-anchorPointInpoints.x + -s*-anchorPointInpoints.y;
		y += s*-anchorPointInpoints.x + c*-anchorPointInpoints.y;
	}

	this->setAdditionalTransform(CCAffineTransformMake(c, s, -s, c, x, y));
	return CCAffineTransformMake(c, s, -s, c, x, y);
}

void PolygonSprite::setAnchorPoint( const CCPoint& anchorPoint )
{
	//prevent sub class use it
}

