#ifndef PhysicsSprite_h__
#define PhysicsSprite_h__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Macros.h"


class PhysicsSprite : public cocos2d::CCSprite
{
public:
	PhysicsSprite();

	~PhysicsSprite();

	void setPhysicsBody(b2Body * body, bool _isspin);

	virtual bool isDirty(void);

	virtual cocos2d::CCAffineTransform nodeToParentTransform(void);

	cocos2d::CCPoint getPhyPos();     //获得刷新的物理坐标

	void deletebody();

	//获得精灵矩阵的顶点坐标（顺时针,从左上角）
	void getcoordinate(); 	

public:
	//在cocos2dx中的顶点坐标
	cocos2d::CCPoint ver_win[4];

	bool isInter, isCrossed;

	cocos2d::CCPoint inter_Node[2];   //切割与物体的两个交点坐标，从左至右排序

	cocos2d::CCPoint inter_Node0[2];  //在矩形贴图中的坐标

	b2Body* m_pBody;    // strong ref

	float maxX, maxY, minX, minY;

	bool isspin;

private:
	cocos2d::CCRect rect;

	float _x, _y;

	float _c, _s;

	cocos2d::CCPoint p;
};
#endif // PhysicsSprite_h__