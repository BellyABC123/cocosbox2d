#include "BasketBall.h"

#include "testResource.h"
#include "VisibleRect.h"
#include "SysMenu.h"
#include "SimpleAudioEngine.h"


using namespace CocosDenshion;
//http://bbs.9ria.com/thread-104394-1-1.html
BasketBall::BasketBall(void):m_pWorld(NULL), m_pBall(NULL), m_pReBound(NULL), m_pGroundBody(NULL)
{
}


BasketBall::~BasketBall(void)
{
	if (m_pReBound) {
		m_pWorld->DestroyBody(m_pReBound);
		m_pReBound = NULL;
	}

	if (m_pGroundBody) {
		m_pWorld->DestroyBody(m_pGroundBody);
		m_pGroundBody = NULL;
	}

	CC_SAFE_DELETE(m_pWorld);
}

bool BasketBall::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}

	b2Vec2 gravity;
	gravity.Set(-10.0f, 0.0f);
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
//	groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
//	m_pGroundBody->CreateFixture(&groundBoxDef);
	groundBox.Set(b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
	m_pGroundBody->CreateFixture(&groundBoxDef);

	m_pBall = Ball::ballWithTexture(CCTextureCache::sharedTextureCache()->addImage("basketball.png"), m_pWorld, ccp(VisibleRect::center().x, VisibleRect::center().y));
	m_pBall->setGroundBody(m_pGroundBody);
	m_pBall->setCanTouch(true);
	addChild(m_pBall, 5);

	m_pWorld->SetContactListener(this);
	setTouchEnabled(true);
	setKeypadEnabled(true);

	m_pLocus = CCArray::createWithCapacity(20);
    m_pLocus->retain();

	m_pEmitter = new CCParticleSystemQuad();
	std::string filename = "ExplodingRing.plist";
	m_pEmitter->initWithFile(filename.c_str());
	m_pEmitter->autorelease();
	m_pEmitter->stopSystem();
	addChild(m_pEmitter, 10);

	return true;
}

void BasketBall::onEnter()
{
	CCLayer::onEnter();
	SimpleAudioEngine::sharedEngine()->preloadEffect( EFFECT_FILE_2);
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);
	this->scheduleUpdate();
}
void BasketBall::onExit()
{
	unscheduleUpdate();
	SimpleAudioEngine::sharedEngine()->unloadEffect(EFFECT_FILE_2);
	CCLayer::onExit();
}

void BasketBall::keyBackClicked()
{
	CCScene * pScene = CCScene::create();
			CCLayer * pSysMenu = SysMenu::create();
			pScene->addChild(pSysMenu);
			CCDirector::sharedDirector()->replaceScene(pScene);
}


void BasketBall::update(float dt)
{
    int velocityIterations = 5;
    int positionIterations = 3;
    m_pWorld->Step(dt, velocityIterations, positionIterations);

}

void BasketBall::BeginContact(b2Contact* contact) {
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	SimpleAudioEngine::sharedEngine()->playEffect(EFFECT_FILE_2);
}
void BasketBall::EndContact(b2Contact* contact) {
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();


}
void BasketBall::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}
void BasketBall::PostSolve(const b2Contact* contact, const b2ContactImpulse* impulse)
{
}

void BasketBall::draw()
{
		b2Vec2 startPos = m_pDistanceJoint1->GetAnchorA();
	    b2Vec2 endPos = m_pDistanceJoint1->GetAnchorB();
	    float startX = startPos.x * PTM_RATIO;
	    float startY = startPos.y * PTM_RATIO;
	    float endX = endPos.x * PTM_RATIO;
	    float endY = endPos.y * PTM_RATIO;
	    glLineWidth( 5.0f );
	    ccDrawColor4B(255,0,0,255);
	    ccDrawLine( ccp(startX, startY), ccp(endX, endY));

	    startPos = m_pDistanceJoint2->GetAnchorA();
	    endPos = m_pDistanceJoint2->GetAnchorB();
	    startX = startPos.x * PTM_RATIO;
	    startY = startPos.y * PTM_RATIO;
	    endX = endPos.x * PTM_RATIO;
	    endY = endPos.y * PTM_RATIO;
	    ccDrawColor4B(255,255,0,255);
	    ccDrawLine(ccp(startX, startY), ccp(endX, endY));
}
