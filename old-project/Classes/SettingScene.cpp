#include "SettingScene.h"
USING_NS_CC;

using namespace CocosDenshion;
bool isPause;
Scene* SettingScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SettingScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool SettingScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *Background = Sprite::create("settingScene.jpg");
	Background->setScale(1.35f);
	Background->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(Background);

	Sprite *MusicScene = Sprite::create("Music.png");
	Sprite *SoundScene = Sprite::create("Sound.png");
	MusicScene->setPosition(340, 500);
	SoundScene->setPosition(340, 400);
	this->addChild(MusicScene);
	this->addChild(SoundScene);

	_turnOn = MenuItemImage::create(
		"On.png",
		"On.png");
	_turnOff = MenuItemImage::create(
		"Off.png",
		"Off.png");
	_turnOn1 = MenuItemImage::create(
		"On.png",
		"On.png");
	_turnOff1 = MenuItemImage::create(
		"Off.png",
		"Off.png");
	MenuItemToggle *toggleItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SettingScene::menuMusicCallback, this), _turnOn, _turnOff, NULL);
	MenuItemToggle *toggleItem1 = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SettingScene::menuEffectsCallBack, this), _turnOn1, _turnOff1, NULL);

	toggleItem->setPosition(Vec2(500,500));
	toggleItem1->setPosition(Vec2(500,400));
	
	auto backMenuItem = MenuItemImage::create(
		"BackUp.png",
		"BackDown.png",
		CC_CALLBACK_1(SettingScene::menuBackCallback, this));
	backMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(600, 600)));
	Menu* menu = Menu::create(backMenuItem, toggleItem,toggleItem1,NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu,1);
	isPause = false;
	return true;
}

void SettingScene::menuMusicCallback(cocos2d::Ref* pSender)
{
	if (isPause == false)
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		isPause = true;
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		isPause = false;
	}
}

void SettingScene::menuEffectsCallBack(cocos2d::Ref* pSender)
{
	if (isPause == false)
	{
		SimpleAudioEngine::sharedEngine()->stopAllEffects();
		isPause = true;
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
		isPause = false;
	}
}
void SettingScene::menuBackCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->popScene();
}