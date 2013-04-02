/*
 * Ball.cpp
 *
 *  Created on: 2013-1-4
 *      Author: cske
 */

#include "Ball.h"
#include "Paddle.h"
#include "testResource.h"

Ball::Ball(void)
{
}

Ball::~Ball(void)
{
}

CCAffineTransform Ball::nodeToParentTransform(void)
{

    b2Vec2 pos = m_pBody->GetPosition();

        float x = pos.x * PTM_RATIO;
        float y = pos.y * PTM_RATIO;

        if (m_bIgnoreAnchorPointForPosition)
        {
            x += m_obAnchorPointInPoints.x;
            y += m_obAnchorPointInPoints.y;
        }

        // Make matrix
        float radians = m_pBody->GetAngle();
        float c = cosf(radians);
        float s = sinf(radians);

        if (! m_obAnchorPointInPoints.equals(CCPointZero))
        {
            x += c*(-m_obAnchorPointInPoints.x) + -s*(-m_obAnchorPointInPoints.y);
            y += s*(-m_obAnchorPointInPoints.x) + c*(-m_obAnchorPointInPoints.y);
        }

        // Rot, Transition Matrix
        m_sTransform = CCAffineTransformMake(c, s,
                                             -s, c,
                                             x, y);

        return m_sTransform;
}

const CCPoint& Ball::getPosition()
{
    b2Vec2 pos = m_pBody->GetPosition();

    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;
    m_obPosition = ccp(x,y);
    return m_obPosition;
}

void Ball::setPosition(const CCPoint &pos)
{
    float angle = m_pBody->GetAngle();
    m_pBody->SetTransform(b2Vec2(pos.x / PTM_RATIO, pos.y / PTM_RATIO), angle);
}


void Ball::createBox2DBody(b2World *pWorld, const CCPoint& position) {
	b2BodyDef ballBodyDef;

	ballBodyDef.position.Set(position.x / PTM_RATIO, position.y / PTM_RATIO);
	ballBodyDef.type = b2_dynamicBody;
	m_pBody = pWorld->CreateBody(&ballBodyDef);
	// Create paddle shape
	b2CircleShape ballShape;
	ballShape.m_radius = getContentSize().width / PTM_RATIO / 2;

	// Create shape definition and add to body
	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &ballShape;
	ballShapeDef.density = 1.0f;
	ballShapeDef.friction = 0.1f;
	ballShapeDef.restitution = 1.0f;
	m_pBody->CreateFixture(&ballShapeDef);

	m_pBody->SetFixedRotation(true);
	m_pBody->SetBullet(true);
	m_pBody->SetLinearDamping(0.3);

	m_pBody->SetUserData(this);
}


Ball* Ball::ballWithTexture(CCTexture2D* aTexture, b2World* world, const  CCPoint& position )
{
	Ball* pBall = new Ball();
	if(pBall && pBall->initWithTexture(aTexture))
	{
		pBall->autorelease();
	}
	else
	{
	  	CC_SAFE_DELETE(pBall);
	}
	pBall->createBox2DBody(world, position);
	return pBall;
}

//void Ball::applyBox2DForce(const b2Vec2& force)
//{
////	m_pBody->ApplyForce(force, m_pBody->GetWorldCenter());
//	m_pBody->ApplyLinearImpulse(force, m_pBody->GetWorldCenter());
////	m_pBody->SetLinearVelocity(force);
////	m_pBody->ApplyForceToCenter(force);
//}



