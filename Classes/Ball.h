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
class Ball: public CCSprite {
private:
	b2Body	*m_pBody;
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

	void applyBox2DForce(const b2Vec2& force);


public:
	static Ball* ballWithTexture(CCTexture2D* aTexture, b2World* world, const  CCPoint& position);
};

#endif /* BALL_H_ */
