#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
class SettingScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menuBackCallback(cocos2d::Ref* pSender);
	cocos2d::MenuItemImage *_turnOn, *_turnOff, *_turnOn1, *_turnOff1;
	void menuMusicCallback(cocos2d::Ref* pSender);
	void menuEffectsCallBack(cocos2d::Ref* pSender);
	CREATE_FUNC(SettingScene);
};