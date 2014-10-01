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

	cocos2d::CCPoint getPhyPos();     //���ˢ�µ���������

	void deletebody();

	//��þ������Ķ������꣨˳ʱ��,�����Ͻǣ�
	void getcoordinate(); 	

public:
	//��cocos2dx�еĶ�������
	cocos2d::CCPoint ver_win[4];

	bool isInter, isCrossed;

	cocos2d::CCPoint inter_Node[2];   //�и�������������������꣬������������

	cocos2d::CCPoint inter_Node0[2];  //�ھ�����ͼ�е�����

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