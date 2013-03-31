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
	float power;
	float m_fInitPositionY;
public:
	PlayerPaddle();
	virtual ~PlayerPaddle();
	virtual void createBox2DBody(b2World *pWorld, const CCPoint& position);

	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

    virtual void onEnter();
    virtual void onExit();

    void setInitPositionY(float y)
    {
    	this->m_fInitPositionY = y;
    }
public:
    static PlayerPaddle* paddleWithTexture(CCTexture2D* aTexture, b2World* world, const  CCPoint& position, const CCRect& rect);

};

#endif /* PLAYERPADDLE_H_ */
