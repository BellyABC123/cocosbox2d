/*
 * LogoLayer.h
 *
 *  Created on: 2013-3-2
 *      Author: cske
 */
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SysMenu.h"
#ifndef LOGOLAYER_H_
#define LOGOLAYER_H_

USING_NS_CC;

class LogoLayer: public CCLayer {
public:
	LogoLayer();
	virtual ~LogoLayer();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	virtual void onEnter();

	virtual void update(float dt);

	CREATE_FUNC(LogoLayer);

	void enterSysMenu();

	void menuCallback(CCObject* pSender);

private:
	b2World	*m_pWorld;
	b2Body	*m_pFireBody;
	b2Body	*m_pIceBody;
	b2Body	*m_pGroundBody;

	CCParticleSystem*    m_pFireEmitter;
	CCParticleSystem*    m_pIceEmitter;
	CCParticleSystem*    m_pSnowEmitter;
};

#endif /* LOGOLAYER_H_ */
