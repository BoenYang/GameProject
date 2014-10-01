#ifndef __CCBLADE_H__
#define __CCBLADE_H__

#include "cocos2d.h"

#define USE_LAGRANGE    1
#define USE_STL_LIST    0
#define DISTANCE_TO_INTERPOLATE 10

inline void CCPointSet(cocos2d::CCPoint *v, float x, float y){
	v->x = x;
	v->y = y;
}

inline float fangle(cocos2d::CCPoint vect){
	if (vect.x == 0.0 && vect.y == 0.0) {
		return 0;
	}

	if (vect.x == 0.0) {
		return vect.y > 0 ? M_PI/2 : -M_PI/2;
	}

	if (vect.y == 0.0 && vect.x < 0) {
		return -M_PI;
	}

	float angle = atan(vect.y / vect.x);

	return vect.x < 0 ? angle + M_PI : angle;
}

inline void f1(cocos2d::CCPoint p1, cocos2d::CCPoint p2, float d, cocos2d::CCPoint *o1, cocos2d::CCPoint *o2){
	float l = ccpDistance(p1, p2);
	float angle = fangle(ccpSub(p2, p1));
	*o1 = ccpRotateByAngle(ccp(p1.x + l,p1.y + d), p1, angle);
	*o2 = ccpRotateByAngle(ccp(p1.x + l,p1.y - d), p1, angle);
}

inline float lagrange1(cocos2d::CCPoint p1, cocos2d::CCPoint p2, float x){
	return (x-p1.x)/(p2.x - p1.x)*p2.y + (x-p2.x)/(p1.x - p2.x)*p1.y ;
}

class CCBlade : public cocos2d::CCNode {
public:
	~CCBlade();
	void draw(void);

	static CCBlade* bladeWithMaximumPoint(int limit);
	bool initWithMaximumPoint(int limit);
	void push(cocos2d::CCPoint v);
	void pop(int n);
	void clear();
	void reset();
	void dim(bool dim);
	void finish();

	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, path, Path);
	CC_SYNTHESIZE_READONLY(unsigned int, pointLimit, PointLimit);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCTexture2D*, _texture, Texture);
	CC_SYNTHESIZE(float, width, Width);
	CC_SYNTHESIZE(bool, autoDim, AutoDim);

protected:
	//cocos2d::CCArray* path;
	int count;
	cocos2d::CCPoint *vertices;
	cocos2d::CCPoint *coordinates;
	bool _reset;
	bool _finish;
	bool _willPop;

private:

	void populateVertices();
	void shift();

};

#endif