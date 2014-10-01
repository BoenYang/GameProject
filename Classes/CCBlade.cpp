#include "CCBlade.h"

using namespace cocos2d;

CCBlade::~CCBlade(){
	CC_SAFE_RELEASE_NULL(_texture);

	delete[] vertices;
	delete[] coordinates;

	path->release();
}


CCBlade* CCBlade::bladeWithMaximumPoint(int limit){
	CCBlade* blade = new CCBlade;
	if(blade && blade->initWithMaximumPoint(limit)){
		blade->autorelease();
		return blade;
	}
	CC_SAFE_RELEASE_NULL(blade);
	return NULL;
}

bool CCBlade::initWithMaximumPoint(int limit){
	bool bRet = false;
	do 
	{
		_texture = NULL;

		this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));

		pointLimit = limit;
		this->width = 5;

		vertices = new CCPoint[2 * limit + 5];
		coordinates = new CCPoint[2 * limit + 5];

		CCPointSet(coordinates + 0, 0.0f, 0.5f);
		_reset = false;

		_willPop = true;
		_finish = false;

		path = new CCArray;
		path->init();

		bRet = true;
	} while (0);
	return bRet;
}

void CCBlade::populateVertices(){
	CCString* string = (CCString*)path->objectAtIndex(0);
	vertices[0] = CCPointFromString(string->getCString());
	CCPoint pre = vertices[0];

	unsigned int i = 0;
	string = (CCString*)path->objectAtIndex(1);
	CCPoint it = CCPointFromString(string->getCString());
	float dd = width / path->count();
	while(i < path->count() - 2){
		f1(pre, it, width - i * dd , vertices+2*i+1, vertices+2*i+2);
		CCPointSet(coordinates+2*i+1, 0.5f, 1.0f);
		CCPointSet(coordinates+2*i+2, 0.5f, 0.0f);

		i++;
		pre = it;

		string = (CCString*)path->objectAtIndex(i + 1);
		it = CCPointFromString(string->getCString());
	}

	CCPointSet(coordinates + 1, 0.25f, 1.0f);
	CCPointSet(coordinates + 2, 0.25f, 0.0f);

	vertices[2*path->count()-3] = it;
	CCPointSet(coordinates+2*path->count()-3, 0.75f, 0.5f);

}

void CCBlade::shift(){
	int index = 2 * pointLimit - 1;
	for (int i = index; i > 3; i -= 2) {
		vertices[i] = vertices[i-2];
		vertices[i-1] = vertices[i-3];
	}
}

void CCBlade::push(cocos2d::CCPoint v){
	_willPop = false;
	if(_reset)
		return;

	CCString* string;

#if USE_LAGRANGE
	if(path->count() == 0){
		string = CCString::createWithFormat("{%f, %f}", v.x, v.y);
		path->insertObject(string, 0);
		return;
	}

	string = (CCString*)path->objectAtIndex(0);
	CCPoint first = CCPointFromString(string->getCString());
	if(ccpDistance(v, first) < DISTANCE_TO_INTERPOLATE){
		string = CCString::createWithFormat("{%f, %f}", v.x, v.y);
		path->insertObject(string, 0);
		if(path->count() > pointLimit){
			path->removeLastObject();
		}
	}else{
		int num = ccpDistance(v, first) / DISTANCE_TO_INTERPOLATE;
		CCPoint iv = ccpMult(ccpSub(v, first), 1.0f / (float)(num + 1));
		for(int i = 1; i <= num + 1; i++){
			CCPoint temp = ccpAdd(first, ccpMult(iv, i));
			string = CCString::createWithFormat("{%f, %f}",temp.x, temp.y);
			path->insertObject(string, 0);
		}
		while(path->count() > pointLimit){
			path->removeLastObject();
		}
	}
#else
	path.push_front(v);
	if (path.size() > pointLimit) {
		path.pop_back();
	}
#endif

	this->populateVertices();
}

void CCBlade::pop(int n){
	while(path->count() > 0 && n > 0){
		path->removeLastObject();
		n --;
	}

	if(path->count() > 2){
		this->populateVertices();
	}
}

void CCBlade::clear(){
	path->removeAllObjects();
	_reset = false;
	if(_finish){
		this->removeFromParentAndCleanup(true);
	}
}

void CCBlade::reset(){
	_reset = true;
}

void CCBlade::dim(bool dim){
	_reset = dim;
}

void CCBlade::draw(){
	if((_reset && path->count() > 0) || (this->getAutoDim() && _willPop)){
		this->pop(1);
		if(path->count() < 3){
			this->clear();
		}
	}

	if(path->count() < 3)
		return;


	_willPop = true;

	CC_NODE_DRAW_SETUP();

	ccGLBlendFunc( CC_BLEND_SRC, CC_BLEND_DST );

	ccGLBindTexture2D(_texture->getName());
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), vertices);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(coordinates[0]), coordinates);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*path->count()-2);

}

void CCBlade::finish(){
	_finish = true;
}