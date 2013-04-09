/*
 * PlayerPaddle.cpp
 *
 *  Created on: 2013-1-13
 *      Author: cske
 */

#include "PlayerPaddle.h"
#include "testResource.h"
#include "VisibleRect.h"


PlayerPaddle::PlayerPaddle():m_fPower(1.0),  m_pMouseJoint(NULL), m_bIsTouched(false) {
	// TODO Auto-generated constructor stub

}

PlayerPaddle::~PlayerPaddle() {
	// TODO Auto-generated destructor stub
}

PlayerPaddle* PlayerPaddle::paddleWithTexture(CCTexture2D* aTexture, b2World* world,const CCPoint& position,const CCRect& rect)
{
	PlayerPaddle* pPaddle = new PlayerPaddle();
	if(pPaddle && pPaddle->initWithTexture(aTexture, rect))
	{
	   pPaddle->autorelease();
	}
	else
	{
	  	CC_SAFE_DELETE(pPaddle);
	}
	pPaddle->createBox2DBody(world, position);
    return pPaddle;
}

void PlayerPaddle::createBox2DBody(b2World *pWorld, const CCPoint& position)
{
	m_pWorld = pWorld;
	b2BodyDef paddleBodyDef;
	paddleBodyDef.position.Set(position.x/PTM_RATIO, position.y/PTM_RATIO);
	paddleBodyDef.type = b2_dynamicBody;
	m_pBody = m_pWorld->CreateBody(&paddleBodyDef);
	m_pBody->SetFixedRotation(true);
	m_pBody->SetLinearDamping(10.0);
	// Create paddle shape
	b2PolygonShape paddleShape;
	paddleShape.SetAsBox(getContentSize().width/PTM_RATIO/2, getContentSize().height/PTM_RATIO/2);

	// Create shape definition and add to body
	b2FixtureDef paddleShapeDef;
	paddleShapeDef.shape =&paddleShape;
	paddleShapeDef.density =1.0f;
	paddleShapeDef.friction =0.5f;
	paddleShapeDef.restitution =0.0f;
//	paddleShapeDef.isSensor = true;
	m_pFixture = m_pBody->CreateFixture(&paddleShapeDef);

//	b2Vec2 v = b2Vec2(10, 0);
//	m_pBody->SetLinearVelocity(v);
//	m_pBody->ApplyLinearImpulse(force, m_pBody->GetWorldCenter());
}

void PlayerPaddle::creatPrismaticJoint()
{
	b2PrismaticJointDef jointDef;
	b2Vec2 worldAxis(1.0f, 0.0f);
	jointDef.collideConnected =true;
	jointDef.Initialize(m_pBody, m_pGroundBody, m_pBody->GetWorldCenter(), worldAxis);
	jointDef.enableLimit = false;
	jointDef.enableMotor = false;
	m_pPrismaticJoint = (b2PrismaticJoint*)m_pWorld->CreateJoint(&jointDef);
}

bool PlayerPaddle::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint location = touch->getLocation();
	CCRect rect = boundingBox();
		if(!rect.containsPoint(location))
			return false;
			b2Vec2 b2location = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
			if(m_pFixture->TestPoint(b2location))
			{
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
				m_pBody->SetTransform(b2location, 0);
				m_pMouseJoint = (b2MouseJoint *)m_pWorld->CreateJoint(&jointDef);
				m_pBody->SetAwake(true);
				m_bIsTouched = true;
			}


	return true;
}
void PlayerPaddle::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
		CCPoint location = touch->getLocation();

//		if(location.x <0)
//			location.x = 0;
//		if(location.x > VisibleRect::right().x)
//			location.x = VisibleRect::right().x;

//		CCRect rect = boundingBox();
//		if(!rect.containsPoint(location))
//			return;
		if (m_pMouseJoint) {
//			CCPoint location = touch->getLocation();
//			if(!rect().containsPoint(location))
//				return;
			CCLog("PlayerPaddle ccTouchMoved ... ");
			b2Vec2 b2location = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
			m_pMouseJoint->SetTarget(b2location);
		}

}
void PlayerPaddle::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
//	CCPoint location = touch->getLocation();
//	CCRect rect = boundingBox();
//			if(!rect.containsPoint(location))
//				return;
		if (m_pMouseJoint) {
			CCPoint location = touch->getLocation();
			m_pWorld->DestroyJoint(m_pMouseJoint);
			m_pMouseJoint = NULL;
			m_bIsTouched = false;
		}
}

void PlayerPaddle::onEnter()
{
	CCSprite::onEnter();
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    this->scheduleUpdate();
}

void PlayerPaddle::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
    this->unscheduleUpdate();
}

void PlayerPaddle::update(float dt)
{
    if(m_bIsTouched)
    {
    	if(m_fPower < 3.0)
    	{
    		m_fPower += 0.05;
    	}
    }
    else
    {
    	if(m_fPower>1.0)
    	{
    		m_fPower -= 0.1;
    	}
    }
}

