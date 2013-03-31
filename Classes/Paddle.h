#ifndef _PADDLE_H_
#define _PADDLE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

USING_NS_CC;


class Paddle : public CCSprite
{
protected:
	bool	m_bIgnoreBodyRotation;
	b2Body	*m_pBody;
	b2MouseJoint	*m_pMouseJoint;
	float	m_pVelocity;
	b2World	*m_pWorld;
	b2Fixture* m_pFixture;
	b2PrismaticJoint* m_pPrismaticJoint;
	b2RevoluteJoint* m_pRevoluteJoint;
	b2Body	*m_pGroundBody;
public:
    Paddle(void);
    virtual ~Paddle(void);

    virtual const CCPoint& getPosition();
    virtual void setPosition(const CCPoint &position);
    virtual float getRotation();
    virtual void setRotation(float fRotation);
    virtual CCAffineTransform nodeToParentTransform();

    b2Body* getB2Body() const
    {
    	return m_pBody;
    }
    void setB2Body(b2Body *pBody)
    {
    	m_pBody = pBody;
    }

    virtual bool isDirty(){return true;}
    bool isIgnoreBodyRotation() const
    {
    	return m_bIgnoreBodyRotation;
    }
    void setIgnoreBodyRotation(bool bIgnoreBodyRotation)
    {
    	m_bIgnoreBodyRotation = bIgnoreBodyRotation;
    }

    void setGroundBody(b2Body* pGroundBody)
    {
    	m_pGroundBody = pGroundBody;
    }

    virtual void createBox2DBody(b2World *pWorld, const CCPoint& position);

    void setBox2DVelocity(const b2Vec2& velocity);

    void creatPrismaticJoint(bool enableMotor);
    void creatRevoluteJoint();

    b2Fixture* getFixture() const
    {
    	return m_pFixture;
    }

private:
    void keepVelocity(float dt);
    CCRect rect();

public:
    static Paddle* paddleWithTexture(CCTexture2D* aTexture, b2World* world, const  CCPoint& position, const CCRect& rect);

};

#endif
