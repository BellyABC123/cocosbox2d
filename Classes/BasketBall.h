#pragma once

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Ball.h"
USING_NS_CC;
class BasketBall: public CCLayer, b2ContactListener
{
public:
	BasketBall(void);
	~BasketBall(void);

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void keyBackClicked();
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(const b2Contact* contact, const b2ContactImpulse* impulse);

	CREATE_FUNC(BasketBall);
	void update(float dt);
	virtual void draw();

private:
	b2World	*m_pWorld;
	Ball	*m_pBall;
	b2Body	*m_pReBound;
	b2Body	*m_pGroundBody;
	CCParticleSystem*    m_pEmitter;
	CCArray	*m_pLocus;    
};

