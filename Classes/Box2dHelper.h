#ifndef Box2dHelper_h__
#define Box2dHelper_h__
#include "Box2D/Box2D.h"

#define calculate_determinant_2x3(x1,y1,x2,y2,x3,y3) (x1 * y2 + x2 * y3 + x3 * y1-y1 * x2 - y2 * x3 - y3 * x1)
#define calculate_determinant_2x2(x1,y1,x2,y2) (x1 * y2 - y1 * x2)
#define midpoint(a, b) (float)(a + b)/2

class Box2dHelper{
private:
	Box2dHelper();
public:
	static b2Vec2* arrangeVertices(b2Vec2* vertices, int count);
	static bool areVerticesAcceptable(b2Vec2* vertices, int count);
	static b2Body* createBodyX(b2World* world,b2Vec2 position, float rotation, b2Vec2* vertices, int32 count, float density, float friction, float restitution);

};


#endif // Box2dHelper_h__