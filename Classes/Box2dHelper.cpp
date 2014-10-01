#include "Box2dHelper.h"

int comparator(const void *a, const void *b){
	const b2Vec2 *va = (const b2Vec2 *)a;
	const b2Vec2 *vb = (const b2Vec2 *)b;

	if (va->x > vb->x) {
		return 1;
	}
	else if (va->x < vb->x) {
		return -1;
	}
	return 0;
};

Box2dHelper::Box2dHelper()
{

}

b2Vec2* Box2dHelper::arrangeVertices(b2Vec2* vertices, int count)
{
	float determinant;
	int iCounterClockWise = 1;
	int iClockWise = count - 1;
	int i;

	b2Vec2 referencePointA, referencePointB;

	b2Vec2 *sortedVertices = new b2Vec2[count];

	//对顶点坐标排序
	qsort(vertices, count, sizeof(b2Vec2), comparator);
	//将第一个点加入到排序后的顶点数组中
	sortedVertices[0] = vertices[0];
	//设置第一个点位参考点A
	referencePointA = vertices[0];
	//设置第一个点位参考点B
	referencePointB = vertices[count - 1];

	//将顶点进行逆时针排序
	for (i = 1; i < count - 1; i++)
	{
		determinant = calculate_determinant_2x3(referencePointA.x,
			referencePointA.y,
			referencePointB.x,
			referencePointB.y,
			vertices[i].x,
			vertices[i].y);
		if (determinant < 0){
			sortedVertices[iCounterClockWise++] = vertices[i];
		}
		else {
			sortedVertices[iClockWise--] = vertices[i];
		}
	}

	sortedVertices[iCounterClockWise] = vertices[count - 1];
	return sortedVertices;
}

bool Box2dHelper::areVerticesAcceptable(b2Vec2* vertices, int count)
{
	if (count < 3)
		return false;

	if (count > b2_maxPolygonVertices){
		return false;
	}

	int32 i;
	for (i = 0; i < count; i++){
		int32 i1 = i;
		int32 i2 = i + 1 < count ? i + 1 : 0;
		b2Vec2 edge = vertices[i2] - vertices[i1];
		if (edge.LengthSquared() <= b2_epsilon * b2_epsilon){
			return false;
		}
	}

	float32 area = 0.0f;
	b2Vec2 pRef(0.0f, 0.0f);
	for (i = 0; i < count; i++){
		b2Vec2 p1 = pRef;
		b2Vec2 p2 = vertices[i];
		b2Vec2 p3 = i + 1 < count ? vertices[i + 1] : vertices[0];

		b2Vec2 e1 = p2 - p1;
		b2Vec2 e2 = p3 - p1;

		float32 D = b2Cross(e1, e2);

		float32 triangleArea = 0.5f * D;
		area += triangleArea;
	}

	if (area <= 0.0001f)
	{
		return false;
	}

	float determinant;
	float referenceDeterminant;
	b2Vec2 v1 = vertices[0] - vertices[count - 1];
	b2Vec2 v2 = vertices[1] - vertices[0];
	referenceDeterminant = calculate_determinant_2x2(v1.x, v1.y, v2.x, v2.y);

	for (i = 1; i < count - 1; i++)
	{
		v1 = v2;
		v2 = vertices[i + 1] - vertices[i];
		determinant = calculate_determinant_2x2(v1.x, v1.y, v2.x, v2.y);

		if (referenceDeterminant * determinant < 0.0f)
		{
			return false;
		}
	}
	v1 = v2;
	v2 = vertices[0] - vertices[count - 1];
	determinant = calculate_determinant_2x2(v1.x, v1.y, v2.x, v2.y);
	if (referenceDeterminant * determinant < 0.0f)
	{
		return false;
	}
	return true;
}

b2Body* Box2dHelper::createBodyX(b2World* world, b2Vec2 position, float rotation, b2Vec2* vertices, int32 count, float density, float friction, float restitution)
{
	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.position = position;
	bodydef.angle = rotation;
	b2Body* body = world->CreateBody(&bodydef);

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

