#ifndef _PADDLE_H_
#define _PADDLE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

USING_NS_CC;


class StaticBody : public CCSprite
{
protected:
	b2Body	*m_pBody;
	b2World	*m_pWorld;
	b2Fixture* m_pFixture;
public:
	StaticBody(void);
    virtual ~StaticBody(void);

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

   void createStatic2DBody(b2World *pWorld, const CCPoint& position, );


public:
    static StaticBody* staticWithTexture(CCTexture2D* aTexture, b2World* world, const  CCPoint& position, const CCRect& rect);

};

#endif
