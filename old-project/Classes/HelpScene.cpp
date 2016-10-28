#include "HelpScene.h"
USING_NS_CC;

Scene* HelpScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelpScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool HelpScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *Help = Sprite::create("help1.png");
	Help->setScale(0.95f);
	Help->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(Help);

	auto backMenuItem = MenuItemImage::create(
		"BackUp.png",
		"BackDown.png",
		CC_CALLBACK_1(HelpScene::menuBackCallback, this)
		);
	backMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(630, 600)));

	Menu* menu = Menu::create(backMenuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	auto nextMenuItem = MenuItemImage::create(
		"NextUp.png",
		"NextDown.png",
		CC_CALLBACK_1(HelpScene::menuNextCallback, this)
		);
	nextMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(870, 330)));

	Menu* menu1 = Menu::create(nextMenuItem, NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1);

	return true;
}

void HelpScene::menuBackCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}

void HelpScene::menuNextCallback(cocos2d::Ref* pSender)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite *Help = Sprite::create("help2.png");
	Help->setScale(0.95f);
	Help->setPosition(Vec2(visibleSize/2));
	this->addChild(Help);
}