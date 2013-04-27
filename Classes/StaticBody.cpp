#include "StaticBody.h"
#include "testResource.h"

StaticBody::StaticBody(void) : m_pBody(NULL),  m_pFixture(
				NULL), m_pWorld(NULL) {
}

StaticBody::~StaticBody(void)
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
StaticBody* StaticBody::StaticBodyWithTexture(CCTexture2D* aTexture, b2World* world,const CCPoint& position,const CCRect& rect)
{
	StaticBody* pStaticBody = new StaticBody();
	if(pStaticBody && pStaticBody->initWithTexture(aTexture, rect))
	{
	   pStaticBody->autorelease();
	}
	else
	{
	  	CC_SAFE_DELETE(pStaticBody);
	}
	pStaticBody->createBox2DBody(world, position);
    return pStaticBody;
}

CCAffineTransform StaticBody::nodeToParentTransform(void)
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

const CCPoint& StaticBody::getPosition()
{
    b2Vec2 pos = m_pBody->GetPosition();

    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;
    m_obPosition = ccp(x,y);
    return m_obPosition;
}

void StaticBody::setPosition(const CCPoint &pos)
{
    float angle = m_pBody->GetAngle();
    m_pBody->SetTransform(b2Vec2(pos.x / PTM_RATIO, pos.y / PTM_RATIO), angle);
}

float StaticBody::getRotation()
{
    return (m_bIgnoreBodyRotation ? CCSprite::getRotation() :
            CC_RADIANS_TO_DEGREES(m_pBody->GetAngle()));
}

void StaticBody::setRotation(float fRotation)
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

void StaticBody::createBox2DBody(b2World *pWorld, const CCPoint& position, float angle)
{
	m_pWorld = pWorld;
	b2BodyDef StaticBodyBodyDef;
	StaticBodyBodyDef.position.Set(position.x/PTM_RATIO, position.y/PTM_RATIO);
	StaticBodyBodyDef.type = b2_staticBody;
	m_pBody = m_pWorld->CreateBody(&StaticBodyBodyDef);
	// Create StaticBody shape
	b2PolygonShape StaticBodyShape;
	StaticBodyShape.SetAsBox(getContentSize().width/PTM_RATIO/2, getContentSize().height/PTM_RATIO/2);

	// Create shape definition and add to body
	b2FixtureDef StaticBodyShapeDef;
	StaticBodyShapeDef.shape =&StaticBodyShape;
	StaticBodyShapeDef.density =5.0f;
	StaticBodyShapeDef.friction =0.0f;
	StaticBodyShapeDef.restitution =1.0f;
	m_pFixture = m_pBody->CreateFixture(&StaticBodyShapeDef);

//	b2Vec2 v = b2Vec2(10, 0);
//	m_pBody->SetLinearVelocity(v);
//	m_pBody->ApplyLinearImpulse(force, m_pBody->GetWorldCenter());
}




