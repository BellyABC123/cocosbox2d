/*
 * PlayerPaddle.h
 *
 *  Created on: 2013-1-13
 *      Author: cske
 */

#ifndef PLAYERPADDLE_H_
#define PLAYERPADDLE_H_
#include "Paddle.h"

class PlayerPaddle: public Paddle, public CCTargetedTouchDelegate
{
private:
	float	m_fPower;
	bool	m_bIsTouched;
	b2MouseJoint	*m_pMouseJoint;
public:
	PlayerPaddle();
	virtual ~PlayerPaddle();
	virtual void createBox2DBody(b2World *pWorld, const CCPoint& position);
	virtual void creatPrismaticJoint();

	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

    virtual void onEnter();
    virtual void onExit();

    void update(float dt);

    float getPower() const
    {
    	return m_fPower;
    }
public:
    static PlayerPaddle* paddleWithTexture(CCTexture2D* aTexture, b2World* world, const  CCPoint& position, const CCRect& rect);

};

#endif /* PLAYERPADDLE_H_ */
