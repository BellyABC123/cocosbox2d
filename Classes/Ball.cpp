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
	m_pWorld = pWorld;
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
	ballShapeDef.density = 0.5f;
	ballShapeDef.friction = 0.0f;
	ballShapeDef.restitution = 1.0f;
	m_pFixture = m_pBody->CreateFixture(&ballShapeDef);
	m_pBody->SetFixedRotation(true);
//	m_pBody->SetBullet(true);

//	m_pBody->SetUserData(this);
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

bool Ball::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCLog("ccTouchBegan Ball");
	CCPoint location = touch->getLocation();
	CCRect rect = boundingBox();
		if(!rect.containsPoint(location))
			return false;
		CCLog("ccTouchBegan Ball 1");
			b2Vec2 b2location = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
			if(m_pFixture->TestPoint(b2location))
			{
				CCLog("ccTouchBegan Ball 3");
				b2MouseJointDef jointDef;
				jointDef.bodyA = m_pGroundBody;
				jointDef.bodyB = m_pBody;
				jointDef.target = b2location;
				jointDef.collideConnected =true;
				jointDef.maxForce = 5000.0f * m_pBody->GetMass();;
				jointDef.frequencyHz = 10.0;
				jointDef.dampingRatio = 0.0;
				if(m_pMouseJoint)
				{
					m_pWorld->DestroyJoint(m_pMouseJoint);
					m_pMouseJoint = NULL;
				}
				CCLog("ccTouchBegan Ball 4");
				m_pBody->SetTransform(b2location, 0);
				m_pMouseJoint = (b2MouseJoint *)m_pWorld->CreateJoint(&jointDef);
				m_pBody->SetAwake(true);
			}

			CCLog("ccTouchBegan Ball end");
	return true;
}
void Ball::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
		CCPoint location = touch->getLocation();
		if (m_pMouseJoint) {
			b2Vec2 b2location = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
			m_pMouseJoint->SetTarget(b2location);
		}

}

void Ball::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
		if (m_pMouseJoint) {
			CCPoint location = touch->getLocation();
			m_pWorld->DestroyJoint(m_pMouseJoint);
			m_pMouseJoint = NULL;
		}
}


void Ball::onEnter() {
	CCSprite::onEnter();
	if (m_bCanTouch)
	{
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	}
}

void Ball::onExit()
{
	if(m_bCanTouch)
	{
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->removeDelegate(this);
	}
    CCSprite::onExit();
}

