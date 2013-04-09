/*
 * Ball.h
 *
 *  Created on: 2013-1-4
 *      Author: cske
 */

#ifndef BALL_H_
#define BALL_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
class Paddle;

USING_NS_CC;
class Ball: public CCSprite,public CCTargetedTouchDelegate {
private:
	b2Body	*m_pBody;
	b2Fixture	*m_pFixture;
	b2MouseJoint	*m_pMouseJoint;
	b2World	*m_pWorld;
	b2Body	*m_pGroundBody;
	bool	m_bCanTouch;
public:
	Ball(void);
	virtual ~Ball(void);

	virtual const CCPoint& getPosition();
	virtual void setPosition(const CCPoint &position);
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

	void createBox2DBody(b2World *pWorld, const CCPoint& position);

	b2Fixture* getFixture(){return m_pFixture;}

	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

	void setCanTouch(bool isCanTouch)
	{
		m_bCanTouch = isCanTouch;
	}

	void setGroundBody(b2Body* pGroundBody)
	{
	   m_pGroundBody = pGroundBody;
	}

    virtual void onEnter();
    virtual void onExit();

public:
	static Ball* ballWithTexture(CCTexture2D* aTexture, b2World* world, const  CCPoint& position);
};

#endif /* BALL_H_ */
