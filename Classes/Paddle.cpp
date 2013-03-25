#include "Paddle.h"
#include "testResource.h"

Paddle::Paddle(void) :
		m_bIgnoreBodyRotation(false), m_pBody(NULL), m_bIsPlayer(false), m_pFixture(
				NULL), m_pMouseJoint(NULL), m_pPrismaticJoint(NULL), m_pRevoluteJoint(
				NULL), m_pGroundBody(NULL) {
}

Paddle::~Paddle(void)
{
	//m_pWorld->DestroyBody(m_pBody);
	/*m_pBody = NULL;
	if(m_pMouseJoint)
	{
		m_pWorld->DestroyJoint(m_pMouseJoint);
		m_pMouseJoint = NULL;
	}
	if(m_pPrismaticJoint)
	{
		m_pWorld->DestroyJoint(m_pPrismaticJoint);
		m_pPrismaticJoint = NULL;
	}
	if(m_pRevoluteJoint)
	{
		m_pWorld->DestroyJoint(m_pRevoluteJoint);
		m_pRevoluteJoint = NULL;
	}*/

}
Paddle* Paddle::paddleWithTexture(CCTexture2D* aTexture, b2World* world,const CCPoint& position,const CCRect& rect)
{
	Paddle* pPaddle = new Paddle();
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

CCAffineTransform Paddle::nodeToParentTransform(void)
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

const CCPoint& Paddle::getPosition()
{
    b2Vec2 pos = m_pBody->GetPosition();

    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;
    m_obPosition = ccp(x,y);
    return m_obPosition;
}

void Paddle::setPosition(const CCPoint &pos)
{
    float angle = m_pBody->GetAngle();
    m_pBody->SetTransform(b2Vec2(pos.x / PTM_RATIO, pos.y / PTM_RATIO), angle);
}

float Paddle::getRotation()
{
    return (m_bIgnoreBodyRotation ? CCSprite::getRotation() :
            CC_RADIANS_TO_DEGREES(m_pBody->GetAngle()));
}

void Paddle::setRotation(float fRotation)
{
    if (m_bIgnoreBodyRotation)
    {
        CCSprite::setRotation(fRotation);
    }
    else
    {
        b2Vec2 p = m_pBody->GetPosition();
        float radians = CC_DEGREES_TO_RADIANS(fRotation);
        m_pBody->SetTransform(p, radians);
    }
}

void Paddle::createBox2DBody(b2World *pWorld, const CCPoint& position)
{
	m_pWorld = pWorld;
	b2BodyDef paddleBodyDef;
	paddleBodyDef.position.Set(position.x/PTM_RATIO, position.y/PTM_RATIO);
	paddleBodyDef.type = b2_dynamicBody;
	m_pBody = m_pWorld->CreateBody(&paddleBodyDef);
	// Create paddle shape
	b2PolygonShape paddleShape;
	paddleShape.SetAsBox(getContentSize().width/PTM_RATIO/2, getContentSize().height/PTM_RATIO/2);

	// Create shape definition and add to body
	b2FixtureDef paddleShapeDef;
	paddleShapeDef.shape =&paddleShape;
	paddleShapeDef.density =5.0f;
	paddleShapeDef.friction =0.0f;
	paddleShapeDef.restitution =1.0f;
	m_pFixture = m_pBody->CreateFixture(&paddleShapeDef);

//	b2Vec2 v = b2Vec2(10, 0);
//	m_pBody->SetLinearVelocity(v);
//	m_pBody->ApplyLinearImpulse(force, m_pBody->GetWorldCenter());
}

void Paddle::setBox2DVelocity(const b2Vec2& velocity)
{
	this->m_pVelocity = velocity.Length();
	m_pBody->SetLinearVelocity(velocity);
//	schedule(schedule_selector(Paddle::keepVelocity), 5);
}

void Paddle::keepVelocity(float dt)
{
	float ratio = m_pVelocity/m_pBody->GetLinearVelocity().Length();
	m_pBody->GetLinearVelocity() *=ratio;
}

bool Paddle::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint location = touch->getLocation();
	CCRect rect = boundingBox();
	if(!rect.containsPoint(location))
		return false;
	if(m_bIsPlayer)
	{
		if(m_pMouseJoint == NULL)
		{
			b2Vec2 b2location = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
			if(m_pFixture->TestPoint(b2location))
			{
				b2MouseJointDef jointDef;
				jointDef.bodyA = m_pGroundBody;
				jointDef.bodyB = m_pBody;
				jointDef.target = b2location;
				jointDef.collideConnected =true;
				jointDef.maxForce = 5.0f* m_pBody->GetMass();
				jointDef.frequencyHz = 10.0;
				jointDef.dampingRatio = 1.0;
				m_pMouseJoint = (b2MouseJoint *)m_pWorld->CreateJoint(&jointDef);
				m_pBody->SetAwake(true);
			}
		}
	}
	return true;
}
void Paddle::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	if(m_bIsPlayer)
	{
		CCPoint location = touch->getLocation();
//		CCRect rect = boundingBox();
//		if(!rect.containsPoint(location))
//			return;
		if (m_pMouseJoint) {
//			CCPoint location = touch->getLocation();
//			if(!rect().containsPoint(location))
//				return;
			b2Vec2 b2location = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
			m_pMouseJoint->SetTarget(b2location);
		}
	}
}
void Paddle::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	if(m_bIsPlayer)
	{
		if (m_pMouseJoint) {
			CCPoint location = touch->getLocation();
//			if(!rect().containsPoint(location))
//						return;
			m_pWorld->DestroyJoint(m_pMouseJoint);
			m_pMouseJoint = NULL;
		}
	}

}
void Paddle::creatPrismaticJoint(bool enableMotor)
{
	b2PrismaticJointDef jointDef;
	b2Vec2 worldAxis(-1.0f, 0.0f);
	jointDef.collideConnected =true;
	jointDef.Initialize(m_pBody, m_pGroundBody, m_pBody->GetWorldCenter(), worldAxis);
	jointDef.enableLimit = false;
//	jointDef.lowerTranslation = -2;
//	jointDef.upperTranslation = 10;

	if(enableMotor)
	{
		jointDef.enableMotor = true;
		jointDef.maxMotorForce = 100;
		jointDef.motorSpeed = 10;
	}
	m_pPrismaticJoint = (b2PrismaticJoint*)m_pWorld->CreateJoint(&jointDef);
}
void Paddle::creatRevoluteJoint()
{
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.Initialize(m_pBody, m_pGroundBody, m_pBody->GetWorldCenter());

	revoluteJointDef.enableLimit = false;
	revoluteJointDef.lowerAngle = 0;
	revoluteJointDef.upperAngle = 0;
	revoluteJointDef.enableMotor = true;
	revoluteJointDef.maxMotorTorque = 5;
	revoluteJointDef.motorSpeed = CC_DEGREES_TO_RADIANS(90);

	m_pRevoluteJoint = (b2RevoluteJoint*)m_pWorld->CreateJoint(&revoluteJointDef);
}

CCRect Paddle::rect()
{
	b2Vec2 pos = m_pBody->GetPosition();
	float x = pos.x * PTM_RATIO;
	float y = pos.y * PTM_RATIO;
	return CCRectMake(x, y, m_obContentSize.width, m_obContentSize.height);
}

void Paddle::onEnter()
{
	if(m_bIsPlayer)
	{
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	}
    CCSprite::onEnter();
}

void Paddle::onExit()
{
	if(m_bIsPlayer)
	{
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->getTouchDispatcher()->removeDelegate(this);
	}
    CCSprite::onExit();
}
