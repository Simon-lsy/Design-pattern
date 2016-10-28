#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
class HelpScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuNextCallback(cocos2d::Ref* pSender);
	void menuBackCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(HelpScene);
};

