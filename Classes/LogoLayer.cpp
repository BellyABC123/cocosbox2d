/*
 * LogoLayer.cpp
 *
 *  Created on: 2013-3-2
 *      Author: cske
 */

#include "LogoLayer.h"
#include "VisibleRect.h"
#include "testResource.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
LogoLayer::LogoLayer():m_pWorld(NULL), m_pFireBody(NULL), m_pIceBody(NULL), m_pFireEmitter(NULL), m_pIceEmitter(NULL) {
	// TODO Auto-generated constructor stub

}

LogoLayer::~LogoLayer() {
	// TODO Auto-generated destructor stub
	if (m_pFireBody) {
		m_pWorld->DestroyBody(m_pFireBody);
		m_pFireBody = NULL;
	}

	if (m_pIceBody) {
		m_pWorld->DestroyBody(m_pIceBody);
		m_pIceBody = NULL;
	}

	if (m_pGroundBody) {
			m_pWorld->DestroyBody(m_pGroundBody);
			m_pGroundBody = NULL;
		}



	CC_SAFE_DELETE(m_pWorld);
}

bool LogoLayer::init()
{
	    if ( !CCLayer::init() )
	    {
	        return false;
	    }

	    // ask director the window size
	    CCSize size = CCDirector::sharedDirector()->getWinSize();
	    CCLabelTTF* beginLable = CCLabelTTF::create("Touch to begin", "Arial", 32);
	    beginLable->setOpacity((GLubyte)(120));
//	    CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(beginLable);
//	        if (pRGBAProtocol)
//	        {
//	            pRGBAProtocol->setOpacity((GLubyte)(120));
//	        }
	    CCMenuItemLabel* pPlayItem = CCMenuItemLabel::create(beginLable, this, menu_selector(LogoLayer::menuCallback));
	    pPlayItem->setTag(1);
	    pPlayItem->setPosition(ccp( VisibleRect::center().x , VisibleRect::bottom().y+50 ));

	    beginLable->runAction(CCRepeatForever::create((CCActionInterval*)CCBlink::create(2, 2)));


	       CCLabelTTF* label = CCLabelTTF::create("COCOSBOX2D DEMO", "Arial", 64);
	       label->setAnchorPoint(ccp(1.0, 0.5));
	       addChild(label, 100, 1000);
	       label->setPosition( ccp(0, size.height-350) );

	       CCLabelTTF *sub = CCLabelTTF::create("cske", "Arial", 46);
	       sub->setAnchorPoint(ccp(1.0, 0.5));
	       addChild(sub, 100);
	       sub->setPosition(ccp(0, size.height-500));

	       CCActionInterval * move = CCSequence::create(CCDelayTime::create(1), CCMoveBy::create(4, ccp(500,0)), NULL);
	       label->runAction(move);

	       CCActionInterval * submove = CCSequence::create(CCDelayTime::create(5), CCMoveBy::create(4, ccp(300,0)), NULL);
	       sub->runAction(submove);


	    CCMenu* pMenu = CCMenu::create(pPlayItem,  NULL);
	    pMenu->setPosition( CCPointZero );
	    addChild(pMenu, 1);
//	    CCSprite* pSprite = CCSprite::create("Default.png");
//	    pSprite->setPosition( ccp(size.width/2, size.height/2) );
//	    addChild(pSprite, 0);

	    b2Vec2 gravity;
	    gravity.Set(0.0f, 0.0f);
	    m_pWorld = new b2World(gravity);
	    m_pWorld->SetAllowSleeping(false);
	    m_pWorld->SetContinuousPhysics(true);

	    	b2BodyDef groundBodyDef;
	    	groundBodyDef.position.Set(0,0);
	    	m_pGroundBody = m_pWorld->CreateBody(&groundBodyDef);
	    	b2EdgeShape groundBox;
	    	b2FixtureDef groundBoxDef;
	    	groundBoxDef.shape =&groundBox;
	    	groundBoxDef.friction = 0.0f;
	    	groundBoxDef.restitution = 1.0;

	    	groundBox.Set(b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO));
	    	m_pGroundBody->CreateFixture(&groundBoxDef);
	    	groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO));
	    	m_pGroundBody->CreateFixture(&groundBoxDef);
	    	groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
	    	m_pGroundBody->CreateFixture(&groundBoxDef);
	    	groundBox.Set(b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
	    	m_pGroundBody->CreateFixture(&groundBoxDef);

	    CCPoint firePosition(100, 200);
	    b2BodyDef fireBodyDef;
	    fireBodyDef.position.Set(firePosition.x / PTM_RATIO, firePosition.y / PTM_RATIO);
	    fireBodyDef.type = b2_dynamicBody;
	    m_pFireBody = m_pWorld->CreateBody(&fireBodyDef);
	    b2CircleShape fireShape;
	    fireShape.m_radius = 10 / PTM_RATIO / 2;
	    b2FixtureDef fireShapeDef;
	   	fireShapeDef.shape = &fireShape;
	   	fireShapeDef.density = 1.0f;
	   	fireShapeDef.friction = 0.1f;
	    fireShapeDef.restitution = 1.0f;
	    m_pFireBody->CreateFixture(&fireShapeDef);

	    b2Vec2 force = b2Vec2(50, -50);
	    m_pFireBody->ApplyLinearImpulse(force, m_pFireBody->GetWorldCenter());

	    m_pFireEmitter = CCParticleSun::createWithTotalParticles(500);
	    m_pFireEmitter->setPosition(firePosition);
	    m_pFireEmitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("fire.png"));
	    addChild(m_pFireEmitter, 5);


	    CCPoint icePosition(200, 600);
	    fireBodyDef.position.Set(icePosition.x / PTM_RATIO, icePosition.y / PTM_RATIO);
	    m_pIceBody = m_pWorld->CreateBody(&fireBodyDef);
	    m_pIceBody->CreateFixture(&fireShapeDef);
	    m_pIceBody->ApplyLinearImpulse(force, m_pFireBody->GetWorldCenter());

	    m_pIceEmitter = CCParticleMeteor::createWithTotalParticles(500);
	    m_pIceEmitter->setPosition(icePosition);
	    m_pIceEmitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("fire.png"));
	    addChild(m_pIceEmitter, 5);

	    //加点雪
	    m_pSnowEmitter = CCParticleSnow::create();
	    addChild(m_pSnowEmitter, 5);
	    // gravity
	    m_pSnowEmitter->setGravity(ccp(0,-10));
	    // speed of particles
	    m_pSnowEmitter->setSpeed(150);
	    m_pSnowEmitter->setSpeedVar(10);
	    ccColor4F startColor = m_pSnowEmitter->getStartColor();
	    startColor.r = 0.9f;
	    startColor.g = 0.9f;
	    startColor.b = 0.9f;
	    m_pSnowEmitter->setStartColor(startColor);
	    ccColor4F startColorVar = m_pSnowEmitter->getStartColorVar();
	    startColorVar.b = 0.1f;
	    m_pSnowEmitter->setStartColorVar(startColorVar);
	    m_pSnowEmitter->setEmissionRate(m_pSnowEmitter->getTotalParticles()/m_pSnowEmitter->getLife());
	    m_pSnowEmitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("snow.png") );
	    m_pSnowEmitter->setPosition( ccp(size.width / 2, size.height) );

		SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic( MUSIC_FILE );
		SimpleAudioEngine::sharedEngine()->preloadEffect( EFFECT_FILE );
    
    // set default volume
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);
		SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
	    scheduleUpdate();
	    return true;
}

void LogoLayer::onEnter()
{
	CCLayer::onEnter();
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_FILE, true);
	runAction( CCSequence::create(
	                                    CCDelayTime::create(20.0f),
	                                    CCCallFunc::create(this, callfunc_selector(LogoLayer::enterSysMenu)),
	                                    NULL)
	             );
}


void LogoLayer::enterSysMenu()
{
	CCScene * pScene = CCScene::create();
	CCLayer * pSysMenu = SysMenu::create();
	pScene->addChild(pSysMenu);
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void LogoLayer::menuCallback(CCObject* pSender)
{
	CCScene * pScene = CCScene::create();
	CCLayer * pSysMenu = SysMenu::create();
	pScene->addChild(pSysMenu);
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void LogoLayer::update(float dt)
{
	int velocityIterations = 8;
	int positionIterations = 3;
	m_pWorld->Step(dt, velocityIterations, positionIterations);
    if (m_pFireEmitter)
    {
    	b2Vec2 pos = m_pFireBody->GetPosition();

    	float x = pos.x * PTM_RATIO;
    	float y = pos.y * PTM_RATIO;
    	m_pFireEmitter->setPosition( ccp(x, y) );
    }
    if (m_pIceEmitter)
        {
        	b2Vec2 pos = m_pIceBody->GetPosition();

        	float x = pos.x * PTM_RATIO;
        	float y = pos.y * PTM_RATIO;
        	m_pIceEmitter->setPosition( ccp(x, y) );
        }
}
