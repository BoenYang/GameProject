#ifndef CutCutCut_RaycastCallback_h
#define CutCutCut_RaycastCallback_h

#include "Box2D/Box2D.h"
#include "PolygonSprite.h"
#include "Macros.h"
#include "cocos2d.h"

#define collinear(x1,y1,x2,y2,x3,y3) fabsf((y1-y2) * (x1-x3) - (y1-y3) * (x1-x2))

class RaycastCallback : public b2RayCastCallback {
public:
	long millisecondNow(){
		cocos2d::cc_timeval now;
		cocos2d::CCTime::gettimeofdayCocos2d(&now, NULL);
		return (now.tv_sec * 1000 + now.tv_usec / 1000);
	}

	//画线的回调函数
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction){
		//判断物理物体是否得承载对象
		cocos2d::CCNode* node = (cocos2d::CCNode*)fixture->GetBody()->GetUserData();
		//需要给node添加标签判断是否可以切割
		if(node != NULL && node->getTag() == enableCut){
			PolygonSprite* ps = (PolygonSprite*)fixture->GetBody()->GetUserData();
			if(!ps)
				return 1;
			//判断光线是进入还是退出
			if(!ps->getSliceEntered()){
				ps->setSliceEntered(true);
				ps->setEntryPoint(ps->getBody()->GetLocalPoint(point));
				ps->setSliceEntryTime((double)millisecondNow() + 1000);
			}else if(!ps->getSliceExited()){
				//设置光线退出点坐标
				ps->setExitPoint(ps->getBody()->GetLocalPoint(point));
				b2Vec2 entrySide = ps->getEntryPoint() - ps->getCentroid();
				b2Vec2 exitSide = ps->getExitPoint() - ps->getCentroid();
				//判断是否从异侧穿过物体
				if(entrySide.x * exitSide.x < 0 || entrySide.y * exitSide.y < 0){
					ps->setSliceExited(true);
				}else{
					//得到刚体
					b2Fixture* fixture = ps->getBody()->GetFixtureList();
					//得到多边形
					b2PolygonShape* polygon = (b2PolygonShape*)fixture->GetShape();
					//得到顶点数目
					int count = polygon->GetVertexCount();
					//是否从同侧穿过标志初始化
					bool onSameLine = false;

					for(int i = 0; i < count; i++){
						b2Vec2 pointA = polygon->GetVertex(i);
						b2Vec2 pointB;
						if(i == count - 1){
							pointB = polygon->GetVertex(0);
						}else{
							pointB = polygon->GetVertex(i + 1);
						}

						float collinear1 = collinear(pointA.x,pointA.y, ps->getEntryPoint().x, ps->getEntryPoint().y, pointB.x,pointB.y);

						if (collinear1 <= 0.00001f)
						{
							float collinear2 = collinear(pointA.x,pointA.y,ps->getExitPoint().x,ps->getExitPoint().y,pointB.x,pointB.y);
							if (collinear2 <= 0.00001f)
							{
								onSameLine = true;
							}
							break;
						}
					}
					if(onSameLine){
						ps->setEntryPoint(ps->getExitPoint());
						ps->setSliceEntryTime((double)millisecondNow() + 1000);
						ps->setSliceExited(false);
					}else{
						ps->setSliceExited(true);
					}
				}
			}
		}
		return 1;
	}
};


#endif