/*
 * Bounce.cpp
 *
 *  Created on: 2013-4-8
 *      Author: cske
 */

#include "Bounce.h"
#include "testResource.h"
#include "VisibleRect.h"
#include "SysMenu.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

Bounce::Bounce():m_pWorld(NULL), m_pBall(NULL)
{
	// TODO Auto-generated constructor stub

}

Bounce::~Bounce() {
	// TODO Auto-generated destructor stub
	if (m_pGroundBody) {
		m_pWorld->DestroyBody(m_pGroundBody);
		m_pGroundBody = NULL;
	}

	CC_SAFE_DELETE(m_pWorld);
}

bool Bounce::init()
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
	groundBoxDef.restitution = 0.5;

	groundBox.Set(b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO));
	m_pGroundBody->CreateFixture(&groundBoxDef);
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO));
	m_pGroundBody->CreateFixture(&groundBoxDef);
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
	m_pGroundBody->CreateFixture(&groundBoxDef);
	groundBox.Set(b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
	m_pGroundBody->CreateFixture(&groundBoxDef);

	m_pBall = Ball::ballWithTexture(CCTextureCache::sharedTextureCache()->addImage("ball.png"), m_pWorld, ccp(VisibleRect::center().x, VisibleRect::center().y));
	m_pBall->setGroundBody(m_pGroundBody);
	m_pBall->setCanTouch(true);
	addChild(m_pBall);

	Ball *pBall = Ball::ballWithTexture(CCTextureCache::sharedTextureCache()->addImage("ball.png"), m_pWorld, ccp(VisibleRect::top().x, VisibleRect::top().y-50));
	pBall->setGroundBody(m_pGroundBody);
	pBall->setCanTouch(true);
	addChild(pBall);


	b2DistanceJointDef jd;
	b2Vec2 p1, p2, d;

	jd.frequencyHz = 2.0f;
	jd.dampingRatio = 0.0f;
	jd.bodyA = m_pGroundBody;
	jd.bodyB = m_pBall->getB2Body();
	jd.localAnchorA.Set(VisibleRect::top().x/PTM_RATIO, (VisibleRect::top().y-100)/PTM_RATIO);
	jd.localAnchorB.Set(0, 0);
	p1 = jd.bodyA->GetWorldPoint(jd.localAnchorA);
	p2 = jd.bodyB->GetWorldPoint(jd.localAnchorB);
	d = p2 - p1;
	jd.length = d.Length();
	m_pDistanceJoint1 = (b2DistanceJoint*)m_pWorld->CreateJoint(&jd);

	jd.frequencyHz = 2.0f;
	jd.dampingRatio = 1.0f;
	jd.localAnchorA.Set(VisibleRect::bottom().x/PTM_RATIO, (VisibleRect::bottom().y+100)/PTM_RATIO);
	jd.localAnchorB.Set(0, 0);
	p1 = jd.bodyA->GetWorldPoint(jd.localAnchorA);
	p2 = jd.bodyB->GetWorldPoint(jd.localAnchorB);
	d = p2 - p1;
	jd.length = d.Length();
	m_pDistanceJoint2 = (b2DistanceJoint*)m_pWorld->CreateJoint(&jd);

	m_pWorld->SetContactListener(this);
	setTouchEnabled(true);
	setKeypadEnabled(true);
	setAccelerometerEnabled(true);

	CCLabelTTF* beginLable = CCLabelTTF::create("begin", "Arial", 32);
	beginLable->setOpacity((GLubyte)100);
	CCMenuItemLabel* pPlayItem = CCMenuItemLabel::create(beginLable, this, menu_selector(Bounce::menuCallback));
	pPlayItem->setTag(1);
	pPlayItem->setPosition(ccp( VisibleRect::right().x-100 , VisibleRect::bottom().y+50 ));

	CCMenu* pMenu = CCMenu::create(pPlayItem,  NULL);
	pMenu->setPosition(CCPointZero);
	addChild(pMenu, 1);

	m_eGameState = inited;

	m_pEmitter = new CCParticleSystemQuad();
	std::string filename = "ExplodingRing.plist";
	m_pEmitter->initWithFile(filename.c_str());
	m_pEmitter->autorelease();
	m_pEmitter->stopSystem();
	addChild(m_pEmitter, 10);

	m_pDrawNode = CCDrawNode::create();
	m_pDrawNode->clear();
	addChild(m_pDrawNode, 0);

	return true;
}

void Bounce::onEnter()
{
	CCLayer::onEnter();
	SimpleAudioEngine::sharedEngine()->preloadEffect( EFFECT_FILE_2);
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);
	this->scheduleUpdate();
}
void Bounce::onExit()
{
	unscheduleUpdate();
	SimpleAudioEngine::sharedEngine()->unloadEffect(EFFECT_FILE_2);
	CCLayer::onExit();
}

void Bounce::keyBackClicked()
{
	CCScene * pScene = CCScene::create();
			CCLayer * pSysMenu = SysMenu::create();
			pScene->addChild(pSysMenu);
			CCDirector::sharedDirector()->replaceScene(pScene);
}

void Bounce::keyMenuClicked()
{

}

void Bounce::update(float dt)
{
    int velocityIterations = 5;
    int positionIterations = 3;
    m_pWorld->Step(dt, velocityIterations, positionIterations);


    m_pDrawNode->clear();

    b2Vec2 startPos = m_pDistanceJoint1->GetAnchorA();
    b2Vec2 endPos = m_pDistanceJoint1->GetAnchorB();
    float startX = startPos.x * PTM_RATIO;
    float startY = startPos.y * PTM_RATIO;
    float endX = endPos.x * PTM_RATIO;
    float endY = endPos.y * PTM_RATIO;
    m_pDrawNode->drawSegment(ccp(startX, startY), ccp(endX, endY), 5, ccc4f(0, 1, 0, 1));

    startPos = m_pDistanceJoint2->GetAnchorA();
    endPos = m_pDistanceJoint2->GetAnchorB();
    startX = startPos.x * PTM_RATIO;
    startY = startPos.y * PTM_RATIO;
    endX = endPos.x * PTM_RATIO;
    endY = endPos.y * PTM_RATIO;
    m_pDrawNode->drawSegment(ccp(startX, startY), ccp(endX, endY), 5, ccc4f(0, 1, 0, 1));


}

void Bounce::BeginContact(b2Contact* contact) {
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_FILE_2);




}
void Bounce::EndContact(b2Contact* contact) {
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();


}
void Bounce::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}
void Bounce::PostSolve(const b2Contact* contact, const b2ContactImpulse* impulse)
{
}

void Bounce::menuCallback(CCObject* pSender)
{

}

void Bounce::didAccelerate(CCAcceleration* pAccelerationValue)
{
//	CCLog("Accelerate ")
}
