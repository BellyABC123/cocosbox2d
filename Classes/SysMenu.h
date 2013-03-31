/*
 * SysMenu.h
 *
 *  Created on: 2013-2-28
 *      Author: cske
 */
#include "cocos2d.h"
#ifndef SYSMENU_H_
#define SYSMENU_H_
USING_NS_CC;


class SysMenu : public CCLayer{
public:
	SysMenu();
	virtual ~SysMenu();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	    // a selector callback
	void menuCallback(CCObject* pSender);

	virtual void onEnter();

	virtual void onExit();


	    // implement the "static node()" method manually
	CREATE_FUNC(SysMenu);
};


#endif /* SYSMENU_H_ */
