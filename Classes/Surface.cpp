#include "Surface.h"
#include "Macros.h"
#include "GB2ShapeCache-x.h"

USING_NS_CC;

Surface* Surface::createWithFilename(b2World* world, const char* filename,CCPoint pos, float rotation)
{
	Surface* surface = new Surface;
	std::string keyName;
	std::string strFile = filename;
	int start = strFile.find_last_of('/');
	if (start == -1){
		start = 0;
	}
	int end = strFile.find_last_of('.');
	keyName = strFile.substr(start+1, end - start - 1);
	if (surface->initWithFile(filename)){
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.angle = rotation*b2_pi / 180;
		bodyDef.position.Set( pos.x  / PTM_RATIO, pos.y/ PTM_RATIO);
		bodyDef.userData = surface;
		b2Body *body = world->CreateBody(&bodyDef);
		body->SetUserData(NULL);
		GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
		sc->addFixturesToBody(body, keyName);
		surface->setAnchorPoint(sc->anchorPointForShape(keyName));
		surface->setPosition(pos);
		surface->setRotation(-rotation);
		surface->setTag(unableCut);
		surface->autorelease();
		return surface;
	}
	CC_SAFE_DELETE(surface);
	return NULL;
}

