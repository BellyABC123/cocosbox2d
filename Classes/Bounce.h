/*
 * Bounce.h
 *
 *  Created on: 2013-4-8
 *      Author: cske
 */

#ifndef BOUNCE_H_
#define BOUNCE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Ball.h"
USING_NS_CC;

class Bounce: public CCLayer, b2ContactListener {

	enum GameState
	{
		inited	= 0,
		ready	= 1,
		running	= 2,
		paused	= 3,
		over	= 4
	};
public:
	Bounce();
	virtual ~Bounce();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	virtual void keyBackClicked(); //Android 返回键
	virtual void keyMenuClicked(); //Android 菜单键

	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(const b2Contact* contact,
			const b2ContactImpulse* impulse);

	virtual void didAccelerate(CCAcceleration* pAccelerationValue);

	CREATE_FUNC(Bounce);
	void update(float dt);

	void menuCallback(CCObject* pSender);

	virtual void draw();

private:
	b2World	*m_pWorld;
	Ball	*m_pBall;
	Ball	*m_pBall2;
	b2Body	*m_pGroundBody;
	b2DistanceJoint	*m_pDistanceJoint1;
	b2DistanceJoint	*m_pDistanceJoint2;
	GameState	m_eGameState;
	CCParticleSystem*    m_pEmitter;
};

#endif /* BOUNCE_H_ */
