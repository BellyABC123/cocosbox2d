/*
 * TennisPaddle.h
 *
 *  Created on: 2013-1-11
 *      Author: cske
 */

#ifndef TENNISPADDLE_H_
#define TENNISPADDLE_H_

#include "Paddle.h"

class TennisPaddle: public Paddle {
private:
	CCPoint m_velocity;
public:
	TennisPaddle();
	virtual ~TennisPaddle();

	virtual void doStep(float delta);

	static TennisPaddle* paddleWithTexture(CCTexture2D* aTexture);
};

#endif /* TENNISPADDLE_H_ */
