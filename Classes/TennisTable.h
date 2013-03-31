/*
 * TennisTable.h
 *
 *  Created on: 2013-2-28
 *      Author: cske
 */

#ifndef TENNISTABLE_H_
#define TENNISTABLE_H_
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Paddle.h"
#include "Ball.h"
#include "PlayerPaddle.h"
USING_NS_CC;
class TennisTable: public CCLayer, b2ContactListener {
	enum GameState
	{
	    inited	= 0,
	    running	= 1,
		paused	= 2,
		topwin	= 3,
		bottomwin	=4,
		over	= 5
	};

public:
	TennisTable();
	virtual ~TennisTable();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchEnded(CCSet *pTouch, CCEvent *pEvent);

	virtual void keyBackClicked();//Android 返回键
	virtual void keyMenuClicked();//Android 菜单键

	virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(const b2Contact* contact, const b2ContactImpulse* impulse);

	CREATE_FUNC(TennisTable);
	void update(float dt);
private:
	b2World	*m_pWorld;
	CCArray*	m_paddles;
	PlayerPaddle	*m_pTopPlayer;
	PlayerPaddle	*m_pBottomPlayer;
	Ball	*m_pBall;
	b2Body	*m_pGroundBody;
	b2Fixture	*m_pBottomFixture;
	b2Fixture	*m_pTopFixture;
	CCLabelTTF	*m_plabel;
	GameState	m_eGameState;
	unsigned int	m_iTopPoint;
	unsigned int	m_iBotPoint;
	CCLabelTTF	*m_pTopPointLabel;
	CCLabelTTF	*m_pBotPointLabel;
};


#endif /* TENNISTABLE_H_ */
