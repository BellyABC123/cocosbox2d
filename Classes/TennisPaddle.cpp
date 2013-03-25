/*
 * TennisPaddle.cpp
 *
 *  Created on: 2013-1-11
 *      Author: cske
 */

#include "TennisPaddle.h"
#include "VisibleRect.h"

TennisPaddle::TennisPaddle() {
	// TODO Auto-generated constructor stub
	m_velocity = ccp(50.0f, 0.0f);;
}

TennisPaddle::~TennisPaddle() {
	// TODO Auto-generated destructor stub
}

void TennisPaddle::doStep(float delta)
{
	this->setPosition( ccpAdd(getPosition(), ccpMult(m_velocity, delta)) );

	if (getPosition().x > VisibleRect::right().x)
	{
	        setPosition( ccp( VisibleRect::right().x , getPosition().y) );
	        m_velocity.x *= -1;
	}
	else if (getPosition().x < VisibleRect::left().x)
	{
	        setPosition( ccp(VisibleRect::left().x, getPosition().y) );
	        m_velocity.x *= -1;
	}
}

TennisPaddle* TennisPaddle::paddleWithTexture(CCTexture2D* aTexture)
{
	TennisPaddle* pPaddle = new TennisPaddle();
    pPaddle->initWithTexture( aTexture );
    pPaddle->autorelease();

    return pPaddle;
}
