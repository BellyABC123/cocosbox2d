/*
 * TennisTable.cpp
 *
 *  Created on: 2013-2-28
 *      Author: cske
 */

#include "TennisTable.h"
#include "testResource.h"
#include "VisibleRect.h"
#include "SysMenu.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

TennisTable::TennisTable():m_pWorld(NULL),m_paddles(NULL), m_pTopPlayer(NULL), m_pBottomPlayer(NULL), m_pBall(NULL){
	// TODO Auto-generated constructor stub

}

TennisTable::~TennisTable() {
	if(m_pGroundBody)
	{
		m_pWorld->DestroyBody(m_pGroundBody);
		m_pGroundBody = NULL;
	}

	CC_SAFE_DELETE(m_pWorld);
	m_paddles->release();
}

bool TennisTable::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}

	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);
	m_pWorld = new b2World(gravity);
	m_pWorld->SetAllowSleeping(false);
	m_pWorld->SetContinuousPhysics(true);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0,0);
	m_pGroundBody = m_pWorld->CreateBody(&groundBodyDef);
	b2EdgeShape groundBox;
	b2FixtureDef groundBoxDef;
	groundBoxDef.shape =&groundBox;
	groundBoxDef.friction = 0.0f;
	groundBoxDef.restitution = 1.0;

	groundBox.Set(b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO));
	m_pBottomFixture = m_pGroundBody->CreateFixture(&groundBoxDef);
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO));
	m_pGroundBody->CreateFixture(&groundBoxDef);
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
	m_pTopFixture = m_pGroundBody->CreateFixture(&groundBoxDef);
	groundBox.Set(b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
	m_pGroundBody->CreateFixture(&groundBoxDef);

	m_paddles = CCArray::createWithCapacity(3);
	m_paddles->retain();

//	Paddle* pPaddle = Paddle::paddleWithTexture(CCTextureCache::sharedTextureCache()->addImage("Block.png"), m_pWorld, ccp(VisibleRect::center().x-200, VisibleRect::center().y+200), CCRectMake(0, 0, 90, 20));
//	pPaddle->setGroundBody(m_pGroundBody);
//	pPaddle->creatRevoluteJoint();
//	m_paddles->addObject(pPaddle);
//
//	pPaddle = Paddle::paddleWithTexture(CCTextureCache::sharedTextureCache()->addImage("Block.png"), m_pWorld, ccp(VisibleRect::center().x+200, VisibleRect::center().y+200), CCRectMake(0, 0, 90, 20));
//	pPaddle->setGroundBody(m_pGroundBody);
//	pPaddle->creatRevoluteJoint();
//	m_paddles->addObject(pPaddle);
//
//	pPaddle = Paddle::paddleWithTexture(CCTextureCache::sharedTextureCache()->addImage("Block.png"), m_pWorld, ccp(VisibleRect::center().x -200, VisibleRect::center().y-200), CCRectMake(0, 0, 90, 20));
//	pPaddle->setGroundBody(m_pGroundBody);
//	pPaddle->creatRevoluteJoint();
//	m_paddles->addObject(pPaddle);
//
//	pPaddle = Paddle::paddleWithTexture(CCTextureCache::sharedTextureCache()->addImage("Block.png"), m_pWorld, ccp(VisibleRect::center().x +200, VisibleRect::center().y-200), CCRectMake(0, 0, 90, 20));
//	pPaddle->setGroundBody(m_pGroundBody);
//	pPaddle->creatRevoluteJoint();
//	m_paddles->addObject(pPaddle);

	Paddle* pPaddle = Paddle::paddleWithTexture(CCTextureCache::sharedTextureCache()->addImage("Paddle.png"), m_pWorld, ccp(VisibleRect::center().x, VisibleRect::bottom().y+350), CCRectMake(0, 0, 120, 30));
	pPaddle->setGroundBody(m_pGroundBody);
	pPaddle->creatRevoluteJoint();
	m_paddles->addObject(pPaddle);

	pPaddle = Paddle::paddleWithTexture(CCTextureCache::sharedTextureCache()->addImage("Paddle.png"), m_pWorld, ccp(VisibleRect::center().x, VisibleRect::top().y-350), CCRectMake(0, 0, 120, 30));
	pPaddle->setGroundBody(m_pGroundBody);
	pPaddle->creatRevoluteJoint();
	m_paddles->addObject(pPaddle);

	pPaddle = Paddle::paddleWithTexture(CCTextureCache::sharedTextureCache()->addImage("Paddle.png"), m_pWorld, ccp(VisibleRect::center().x, VisibleRect::center().y), CCRectMake(0, 0, 120, 30));
	pPaddle->setGroundBody(m_pGroundBody);
	pPaddle->creatPrismaticJoint(false);
	b2Vec2 v = b2Vec2(10, 0);
	pPaddle->setBox2DVelocity(v);
	m_paddles->addObject(pPaddle);

	CCObject* pObj = NULL;
	CCARRAY_FOREACH(m_paddles, pObj)
	{
		pPaddle = (Paddle*)(pObj);

	        if(!pPaddle)
	            break;

	        addChild(pPaddle);
	}



	m_pTopPlayer = PlayerPaddle::paddleWithTexture(CCTextureCache::sharedTextureCache()->addImage("player.png"), m_pWorld, ccp(VisibleRect::top().x, VisibleRect::top().y-50), CCRectMake(0, 0, 150, 50));
	m_pTopPlayer->setGroundBody(m_pGroundBody);
//	m_pTopPlayer->creatPrismaticJoint(false);
	addChild(m_pTopPlayer);

	m_pBottomPlayer = PlayerPaddle::paddleWithTexture(CCTextureCache::sharedTextureCache()->addImage("player.png"), m_pWorld, ccp(VisibleRect::bottom().x, VisibleRect::bottom().y+50), CCRectMake(0, 0, 150, 50));
	m_pBottomPlayer->setGroundBody(m_pGroundBody);
//	m_pBottomPlayer->creatPrismaticJoint(false);
	addChild(m_pBottomPlayer);

	m_pBall = Ball::ballWithTexture(CCTextureCache::sharedTextureCache()->addImage("ball.png"), m_pWorld, ccp(VisibleRect::center().x, VisibleRect::bottom().y+100));
	b2Vec2 force = b2Vec2(0, 20);
	m_pBall->applyBox2DForce(force);
	addChild(m_pBall);

	m_pWorld->SetContactListener(this);

	setTouchEnabled(true);
	setAccelerometerEnabled(true);

	m_plabel = CCLabelTTF::create("touch to begin", "Arial", 32);
	m_plabel->setOpacity((GLubyte)100);
//	CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(m_plabel);
//    if (pRGBAProtocol)
//    {
//        pRGBAProtocol->setOpacity((GLubyte)(100));
//    }
	addChild(m_plabel, -1);
	m_plabel->setPosition(ccp(VisibleRect::center().x, VisibleRect::center().y-50));

	m_eGameState = inited;
	m_iTopPoint = 0;
	m_iBotPoint = 0;

	char string[15] = {0};
	sprintf(string, "得分：%d", m_iTopPoint);
	m_pTopPointLabel = CCLabelTTF::create(string, "Arial", 16);;
	m_pTopPointLabel->setPosition(ccp(VisibleRect::top().x, VisibleRect::top().y-80));
	addChild(m_pTopPointLabel, -1);

	sprintf(string, "得分：%d", m_iBotPoint);
	m_pBotPointLabel = CCLabelTTF::create(string, "Arial", 16);
	m_pBotPointLabel->setPosition(ccp(VisibleRect::bottom().x, VisibleRect::bottom().y+80));
	addChild(m_pBotPointLabel, -1);


	return true;
}

void TennisTable::onEnter()
{
	CCLayer::onEnter();
	SimpleAudioEngine::sharedEngine()->preloadEffect( EFFECT_FILE_1);
	SimpleAudioEngine::sharedEngine()->preloadEffect( EFFECT_FILE_2);
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);
	m_pScheduler->scheduleUpdateForTarget(this, 0, true);
}
void TennisTable::onExit()
{
	unscheduleUpdate();
	SimpleAudioEngine::sharedEngine()->unloadEffect(EFFECT_FILE_1);
	SimpleAudioEngine::sharedEngine()->unloadEffect(EFFECT_FILE_2);
	CCLayer::onExit();
}

void TennisTable::keyBackClicked()
{
	CCScene * pScene = CCScene::create();
			CCLayer * pSysMenu = SysMenu::create();
			pScene->addChild(pSysMenu);
			CCDirector::sharedDirector()->replaceScene(pScene);
}

void TennisTable::keyMenuClicked()
{

}

void TennisTable::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCLog("ccTouchBegan ...");
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
	CCPoint touchLocation = touch->getLocation();

	if (touchLocation.y < VisibleRect::top().y - 350
			&& touchLocation.y > VisibleRect::bottom().y + 350) {
//		CCScene * pScene = CCScene::create();
//		CCLayer * pSysMenu = SysMenu::create();
//		pScene->addChild(pSysMenu);
//		CCDirector::sharedDirector()->replaceScene(pScene);
		if (m_pScheduler->isTargetPaused(this)) {
					m_pScheduler->resumeTarget(this);
					m_eGameState = running;
					m_plabel->setString("touch to pause");
				} else {
					m_eGameState = paused;
					m_pScheduler->pauseTarget(this);
					m_plabel->setString("touch to begin");
				}
	}
}
void TennisTable::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCLog("ccTouchesMoved ...");
}

void TennisTable::ccTouchEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCLog("ccTouchEnded ...");
}

void TennisTable::update(float dt)
{
    int velocityIterations = 8;
    int positionIterations = 3;
    m_pWorld->Step(dt, velocityIterations, positionIterations);
    if(m_eGameState == bottomwin)
    {
    	m_plabel->setString("touch to continue game");
    	m_iBotPoint++;
    	char string[15] = {0};
    	sprintf(string, "得分：%d", m_iBotPoint);
    	m_pBotPointLabel->setString(string);
    	m_pScheduler->pauseTarget(this);
    }

    if (m_eGameState == topwin) {
		m_plabel->setString("touch to continue game");
		m_iTopPoint++;
		char string[15] = { 0 };
		sprintf(string, "得分：%d", m_iTopPoint);
		m_pTopPointLabel->setString(string);
		m_pScheduler->pauseTarget(this);
	}
}

void TennisTable::BeginContact(b2Contact* contact) {
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if (fixtureA == m_pBottomFixture) {
		void* userData = fixtureB->GetBody()->GetUserData();
		if (userData) {
			m_eGameState = topwin;
		}
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_FILE_1);
	}

	if (fixtureB == m_pBottomFixture) {
		void* userData = fixtureA->GetBody()->GetUserData();
		if (userData) {
			m_eGameState = topwin;
		}
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_FILE_1);
	}

	if (fixtureA == m_pTopFixture) {
			void* userData = fixtureB->GetBody()->GetUserData();
			if (userData) {
				m_eGameState = bottomwin;
			}
			SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_FILE_1);
		}

		if (fixtureB == m_pTopFixture) {
			void* userData = fixtureA->GetBody()->GetUserData();
			if (userData) {
				m_eGameState = bottomwin;
			}
			SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_FILE_1);
		}

		if(fixtureA == m_pTopPlayer->getFixture()||fixtureB == m_pTopPlayer->getFixture())
			{
			SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_FILE_2);
			}

			if(fixtureA == m_pBottomPlayer->getFixture()||fixtureB == m_pBottomPlayer->getFixture())
			{
				SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_FILE_2);
			}
}
void TennisTable::EndContact(b2Contact* contact) {
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();


	if (fixtureA == m_pTopFixture||fixtureB == m_pTopFixture) {
		CCLog("End Contact top");
		b2Vec2 force = b2Vec2(0, -10);
		m_pBall->getB2Body()->SetLinearVelocity(force);
	}

	if (fixtureA == m_pBottomFixture||fixtureB == m_pBottomFixture) {
		CCLog("End Contact bottom");
		b2Vec2 force = b2Vec2(0, 10);
		m_pBall->getB2Body()->SetLinearVelocity(force);
	}

	if (fixtureA == m_pTopPlayer->getFixture()
			|| fixtureB == m_pTopPlayer->getFixture()) {
		m_pBall->getB2Body()->SetLinearVelocity(1.2*m_pBall->getB2Body()->GetLinearVelocity());
	}

	if (fixtureA == m_pBottomPlayer->getFixture()
			|| fixtureB == m_pBottomPlayer->getFixture()) {
		m_pBall->getB2Body()->SetLinearVelocity(1.2*m_pBall->getB2Body()->GetLinearVelocity());
	}

}
void TennisTable::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}
void TennisTable::PostSolve(const b2Contact* contact, const b2ContactImpulse* impulse)
{
}
