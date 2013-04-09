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
	m_iTopPoint = 0;
		m_iBotPoint = 0;
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
	groundBoxDef.restitution = 0.1;

	groundBox.Set(b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO));
	m_pBottomFixture = m_pGroundBody->CreateFixture(&groundBoxDef);
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO));
	m_pLeftFixture = m_pGroundBody->CreateFixture(&groundBoxDef);
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
	m_pTopFixture = m_pGroundBody->CreateFixture(&groundBoxDef);
	groundBox.Set(b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
	m_pRightFixture = m_pGroundBody->CreateFixture(&groundBoxDef);

	m_paddles = CCArray::createWithCapacity(4);
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

	Paddle* pPaddle = Paddle::paddleWithTexture(CCTextureCache::sharedTextureCache()->addImage("Paddle.png"), m_pWorld, ccp(VisibleRect::center().x, VisibleRect::bottom().y+300), CCRectMake(0, 0, 120, 30));
	pPaddle->setGroundBody(m_pGroundBody);
	pPaddle->creatRevoluteJoint();
	m_paddles->addObject(pPaddle);

	pPaddle = Paddle::paddleWithTexture(CCTextureCache::sharedTextureCache()->addImage("Paddle.png"), m_pWorld, ccp(VisibleRect::center().x, VisibleRect::top().y-300), CCRectMake(0, 0, 120, 30));
	pPaddle->setGroundBody(m_pGroundBody);
	pPaddle->creatRevoluteJoint();
	m_paddles->addObject(pPaddle);

	pPaddle = Paddle::paddleWithTexture(CCTextureCache::sharedTextureCache()->addImage("Paddle.png"), m_pWorld, ccp(VisibleRect::left().x+60, VisibleRect::center().y+150), CCRectMake(0, 0, 120, 30));
	pPaddle->setGroundBody(m_pGroundBody);
	b2Vec2 worldAxis(-1.0f, 0.0f);
	pPaddle->creatPrismaticJoint(worldAxis);
	m_paddles->addObject(pPaddle);

	pPaddle = Paddle::paddleWithTexture(CCTextureCache::sharedTextureCache()->addImage("Paddle.png"), m_pWorld, ccp(VisibleRect::right().x-60, VisibleRect::center().y-150), CCRectMake(0, 0, 120, 30));
	pPaddle->setGroundBody(m_pGroundBody);
	worldAxis.x = 1.0;
	pPaddle->creatPrismaticJoint(worldAxis);
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
	m_pTopPlayer->creatPrismaticJoint();
	addChild(m_pTopPlayer);

	m_pBottomPlayer = PlayerPaddle::paddleWithTexture(CCTextureCache::sharedTextureCache()->addImage("player.png"), m_pWorld, ccp(VisibleRect::bottom().x, VisibleRect::bottom().y+50), CCRectMake(0, 0, 150, 50));
	m_pBottomPlayer->setGroundBody(m_pGroundBody);
	m_pBottomPlayer->creatPrismaticJoint();
	addChild(m_pBottomPlayer);

	m_pBall = Ball::ballWithTexture(CCTextureCache::sharedTextureCache()->addImage("ball.png"), m_pWorld, ccp(VisibleRect::center().x, VisibleRect::center().y));
	m_pBall->setCanTouch(false);
	addChild(m_pBall);

	m_pWorld->SetContactListener(this);

	setTouchEnabled(true);
	setKeypadEnabled(true);

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

	char string[15] = {0};
	sprintf(string, "得分：%d", m_iTopPoint);
	m_pTopPointLabel = CCLabelTTF::create(string, "Arial", 16);;
	m_pTopPointLabel->setPosition(ccp(VisibleRect::top().x, VisibleRect::top().y-100));
	addChild(m_pTopPointLabel, -1);

	sprintf(string, "得分：%d", m_iBotPoint);
	m_pBotPointLabel = CCLabelTTF::create(string, "Arial", 16);
	m_pBotPointLabel->setPosition(ccp(VisibleRect::bottom().x, VisibleRect::bottom().y+100));
	addChild(m_pBotPointLabel, -1);

	sprintf(string, "power：%.2f", m_pTopPlayer->getPower());
	m_pTopPowerLabel = CCLabelTTF::create(string, "Arial", 20);;
	m_pTopPowerLabel->setPosition(ccp(VisibleRect::right().x-50, VisibleRect::top().y-50));
	addChild(m_pTopPowerLabel, -1);

	sprintf(string, "power：%.2f", m_pBottomPlayer->getPower());
	m_pBotPowerLabel = CCLabelTTF::create(string, "Arial", 16);
	m_pBotPowerLabel->setPosition(ccp(VisibleRect::left().x+50, VisibleRect::bottom().y+50));
	addChild(m_pBotPowerLabel, -1);

	m_pEmitter = new CCParticleSystemQuad();
	std::string filename = "ExplodingRing.plist";
	m_pEmitter->initWithFile(filename.c_str());
	m_pEmitter->autorelease();
	m_pEmitter->stopSystem();
	addChild(m_pEmitter, 10);

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
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
	CCPoint touchLocation = touch->getLocation();
	if (touchLocation.y < VisibleRect::top().y - 400
			&& touchLocation.y > VisibleRect::bottom().y + 400) {
//		CCScene * pScene = CCScene::create();
//		CCLayer * pSysMenu = SysMenu::create();
//		pScene->addChild(pSysMenu);
//		CCDirector::sharedDirector()->replaceScene(pScene);
		if (m_pScheduler->isTargetPaused(this)) {
					m_pScheduler->resumeTarget(this);
					if(m_eGameState == inited)
					{
						b2Vec2 v = b2Vec2(0, 20);
						m_pBall->getB2Body()->SetLinearVelocity(v);
					}
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
    int velocityIterations = 5;
    int positionIterations = 3;
    m_pWorld->Step(dt, velocityIterations, positionIterations);
    if(m_eGameState == bottomwin || m_eGameState == topwin)
    {
    	m_eGameState == bottomwin?m_iBotPoint++:m_iTopPoint++;
    	CCScene * pScene = CCScene::create();
    	TennisTable * pLayer = new TennisTable();

    	if (pLayer)
    	{
    		pLayer->setBottomPoint(m_iBotPoint);
    		pLayer->setTopPoint(m_iTopPoint);
    		pLayer->init();
    		pLayer->autorelease();
    	}
    	else
    	{
    	        delete pLayer;
    	        pLayer = NULL;
    	        return;
    	}
    	pScene->addChild(pLayer);
    	CCDirector::sharedDirector()->replaceScene(pScene);
    }

    char string[15] = {0};
    sprintf(string, "power：%.2f", m_pTopPlayer->getPower());
    m_pTopPowerLabel->setString(string);

    sprintf(string, "power：%.2f", m_pBottomPlayer->getPower());
    m_pBotPowerLabel->setString(string);
}

void TennisTable::BeginContact(b2Contact* contact) {
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	if (fixtureA == m_pBottomFixture || fixtureB == m_pBottomFixture) {
		m_eGameState = topwin;
		SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_FILE_1);
	}

	if (fixtureA == m_pTopFixture || fixtureB == m_pTopFixture) {
		m_eGameState = bottomwin;
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

	if(fixtureA == m_pBall->getFixture()||fixtureB == m_pBall->getFixture())
	{
		//normal manifold contains all info...
			int numPoints = contact->GetManifold()->pointCount;
			//...world manifold is helpful for getting locations
			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);
			for (int i = 0; i < numPoints; i++)
			{
				m_pEmitter->setPosition( ccp(worldManifold.points[i].x*PTM_RATIO, worldManifold.points[i].y*PTM_RATIO) );
				m_pEmitter->resetSystem();
			}
	}


}
void TennisTable::EndContact(b2Contact* contact) {
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();


	if (fixtureA == m_pTopPlayer->getFixture()
			|| fixtureB == m_pTopPlayer->getFixture()) {
		m_pBall->getB2Body()->SetLinearVelocity(m_pTopPlayer->getPower()*m_pBall->getB2Body()->GetLinearVelocity());
	}

	if (fixtureA == m_pBottomPlayer->getFixture()
			|| fixtureB == m_pBottomPlayer->getFixture()) {
		m_pBall->getB2Body()->SetLinearVelocity(m_pBottomPlayer->getPower()*m_pBall->getB2Body()->GetLinearVelocity());
	}

}
void TennisTable::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}
void TennisTable::PostSolve(const b2Contact* contact, const b2ContactImpulse* impulse)
{
}
