//
//  GB2ShapeCache-x.cpp
//  
//  Loads physics sprites created with http://www.PhysicsEditor.de
//  To be used with cocos2d-x
//
//  Generic Shape Cache for box2d
//
//  Created by Thomas Broquist
//
//      http://www.PhysicsEditor.de
//      http://texturepacker.com
//      http://www.code-and-web.de
//  
//  All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include "GB2ShapeCache-x.h"
#include "Box2D/Box2D.h"
#include "cocos2d.h"


using namespace cocos2d;

/**
 * Internal class to hold the fixtures
 */
class FixtureDef {
public:
    FixtureDef()
    : next(NULL) {}
    
    ~FixtureDef() {
        delete next;
        delete fixture.shape;
    }
    
    FixtureDef *next;
    b2FixtureDef fixture;
    int callbackData;
};

class BodyDef {
public:
	BodyDef()
	: fixtures(NULL) {}
	
	~BodyDef() {
		if (fixtures)
			delete fixtures;
	}
	
	FixtureDef *fixtures;
	CCPoint anchorPoint;
};

static GB2ShapeCache *_sharedGB2ShapeCache = NULL;

GB2ShapeCache* GB2ShapeCache::sharedGB2ShapeCache(void) {
	if (!_sharedGB2ShapeCache) {
		_sharedGB2ShapeCache = new GB2ShapeCache();
        _sharedGB2ShapeCache->init();
	}
	
	return _sharedGB2ShapeCache;
}

bool GB2ShapeCache::init() {
	return true;
}

void GB2ShapeCache::reset() {
	std::map<std::string, BodyDef *>::iterator iter;
	for (iter = shapeObjects.begin() ; iter != shapeObjects.end() ; ++iter) {
		delete iter->second;
	}
	shapeObjects.clear();
}

void GB2ShapeCache::addFixturesToBody(b2Body *body, const std::string &shape) {
	std::map<std::string, BodyDef *>::iterator pos = shapeObjects.find(shape);
	assert(pos != shapeObjects.end());
	
	BodyDef *so = (*pos).second;

	FixtureDef *fix = so->fixtures;
    while (fix) {
        body->CreateFixture(&fix->fixture);
        fix = fix->next;
    }
}

cocos2d::CCPoint GB2ShapeCache::anchorPointForShape(const std::string &shape) {
	std::map<std::string, BodyDef *>::iterator pos = shapeObjects.find(shape);
	assert(pos != shapeObjects.end());
	
	BodyDef *bd = (*pos).second;
	return bd->anchorPoint;
}


void GB2ShapeCache::addShapesWithFile(const char* pFile) {
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pFile);
	CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());
	CCDictionary *metadataDict = (CCDictionary *)dict->objectForKey("metadata");
    int format = metadataDict->valueForKey("format")->intValue();
	ptmRatio = metadataDict->valueForKey("ptm_ratio")->floatValue();

	CCDictionary *bodyDict = (CCDictionary *)dict->objectForKey("bodies");

    b2Vec2 vertices[b2_maxPolygonVertices];
	
	CCDictElement *bodyData = NULL;
	CCDICT_FOREACH(bodyDict, bodyData)
	{
		std::string bodyName = bodyData->getStrKey();
		BodyDef *bodyDef = new BodyDef();
		CCDictionary* frame = (CCDictionary*)bodyData->getObject();
		bodyDef->anchorPoint = CCPointFromString(frame->valueForKey("anchorpoint")->getCString());
		CCArray *fixtureList = (CCArray*)(frame->objectForKey("fixtures"));
		FixtureDef **nextFixtureDef = &(bodyDef->fixtures);

		for (int i = 0; i < fixtureList->count(); i++)
		{
			b2FixtureDef basicData;
			CCDictionary* fixtureData = (CCDictionary*)fixtureList->objectAtIndex(i);
			basicData.filter.categoryBits = fixtureData->valueForKey("filter_categoryBits")->intValue();
			basicData.filter.maskBits = fixtureData->valueForKey("filter_maskBits")->intValue();
			basicData.filter.groupIndex = fixtureData->valueForKey("filter_groupIndex")->intValue();
			basicData.friction = fixtureData->valueForKey("friction")->floatValue();
			basicData.density = fixtureData->valueForKey("density")->floatValue();
			basicData.restitution = fixtureData->valueForKey("restitution")->floatValue();
			basicData.isSensor = fixtureData->valueForKey("isSensor")->boolValue();

			std::string fixtureType = fixtureData->valueForKey("fixture_type")->getCString();
			if (fixtureType == "POLYGON") {
				CCArray *polygonsArray = (CCArray *)(fixtureData->objectForKey("polygons"));

				for (int j = 0; j < polygonsArray->count(); j++) 
				{
					FixtureDef *fix = new FixtureDef();
					fix->fixture = basicData; // copy basic data
					//fix->callbackData = callbackData;

					b2PolygonShape *polyshape = new b2PolygonShape();
					int vindex = 0;

					CCArray *polygonArray = (CCArray*)polygonsArray->objectAtIndex(j);

					for (int k = 0; k < polygonArray->count(); k++) 
					{
						CCString* str = (CCString*)polygonArray->objectAtIndex(k);
						CCPoint offset = CCPointFromString(str->getCString());
						vertices[vindex].x = (offset.x / ptmRatio);
						vertices[vindex].y = (offset.y / ptmRatio);
						vindex++;
					}

					polyshape->Set(vertices, vindex);
					fix->fixture.shape = polyshape;

					// create a list
					*nextFixtureDef = fix;
					nextFixtureDef = &(fix->next);
				}

			}
			else if (fixtureType == "CIRCLE") {
				FixtureDef *fix = new FixtureDef();
				fix->fixture = basicData; // copy basic data
				//fix->callbackData = callbackData;

				CCDictionary *circleData = (CCDictionary *)fixtureData->objectForKey("circle");

				b2CircleShape *circleShape = new b2CircleShape();

				circleShape->m_radius = circleData->valueForKey("radius")->floatValue() / ptmRatio;
				CCPoint p = CCPointFromString(circleData->valueForKey("position")->getCString());
				circleShape->m_p = b2Vec2(p.x / ptmRatio, p.y / ptmRatio);
				fix->fixture.shape = circleShape;
				// create a list
				*nextFixtureDef = fix;
				nextFixtureDef = &(fix->next);
			}
			else {
				CCAssert(0, "Unknown fixtureType");
			}

			shapeObjects[bodyName] = bodyDef;
		}
	}
}