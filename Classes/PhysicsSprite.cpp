#include "PhysicsSprite.h"


USING_NS_CC;

PhysicsSprite::PhysicsSprite()
    : m_pBody(NULL), isInter(false), isCrossed(false)
{
}

void PhysicsSprite::getcoordinate()
{
    rect = getTextureRect();
    CCPoint ac = getAnchorPointInPoints();
    float X1 = rect.getMaxX() / 2;
    float Y1 = rect.getMaxY() / 2;
    float X2 = -rect.getMaxX() / 2;
    float Y2 = -rect.getMaxY() / 2;
    float x[4], y[4];
    x[0] = X2 * _c - Y1 * _s + p.x;
    y[0] = Y1 * _c + X2 * _s + p.y;
    x[1] = X1 * _c - Y1 * _s + p.x;
    y[1] = Y1 * _c + X1 * _s + p.y;
    x[2] = X1 * _c - Y2 * _s + p.x;
    y[2] = Y2 * _c + X1 * _s + p.y;
    x[3] = X2 * _c - Y2 * _s + p.x;
    y[3] = Y2 * _c + X2 * _s + p.y;
    ver_win[0] = ccp(x[0], y[0]);
    ver_win[1] = ccp(x[1], y[1]);
    ver_win[2] = ccp(x[2], y[2]);
    ver_win[3] = ccp(x[3], y[3]);
    maxX = maxY = 0; minX = x[0]; minY = y[0];
    for (int i = 0; i < 4; i++)
    {
        if (maxX < x[i])
            maxX = x[i];
        if (maxY < y[i])
            maxY = y[i];
        if (minX > x[i])
            minX = x[i];
        if (minY > y[i])
            minY = y[i];
    }
}

PhysicsSprite::~PhysicsSprite()
{
	deletebody();
}

void PhysicsSprite::setPhysicsBody(b2Body * body, bool _isspin)
{
    isspin = _isspin;
    m_pBody = body;
}

void PhysicsSprite::deletebody()
{
	//world->DestroyBody(m_pBody);
    m_pBody = NULL;
}

// this method will only get called if the sprite is batched.
// return YES if the physics values (angles, position ) changed
// If you return NO, then nodeToParentTransform won't be called.
bool PhysicsSprite::isDirty(void)
{
    return true;
}

CCPoint PhysicsSprite::getPhyPos()
{
	return ccp(m_pBody->GetPosition().x*PTM_RATIO, m_pBody->GetPosition().y*PTM_RATIO);
}

// returns the transform matrix according the Chipmunk Body values
CCAffineTransform PhysicsSprite::nodeToParentTransform(void)
{
    CCPoint anchorPointInpoints = this->getAnchorPointInPoints();
    b2Vec2 pos  = m_pBody->GetPosition();
    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;
    _x = x;
    _y = y;
    p = ccp(_x, _y);
    
    if ( isIgnoreAnchorPointForPosition() )
    {
        x += anchorPointInpoints.x;
        y += anchorPointInpoints.y;
    }
    
    // Make matrix
    float radians;
    if (isspin)
        radians = m_pBody->GetAngle();
    else
        radians = 0;
    float c = cosf(radians);
    float s = sinf(radians);
    
    if( ! anchorPointInpoints.equals(CCPointZero) )
    {
        x += c * -anchorPointInpoints.x + -s * -anchorPointInpoints.y;
        y += s * -anchorPointInpoints.x + c * -anchorPointInpoints.y;
    }
    
    _c = c;
    _s = s;
    return CCAffineTransformMake(c, s, -s, c, x, y);
}

