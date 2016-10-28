#include "HelloWorldScene.h"
USING_NS_CC;
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *Background = Sprite::create("Background.png");
	Background->setScale(1.3f);
	Background->setPosition(Vec2(origin.x + visibleSize.width / 2
		, origin.y + visibleSize.height / 2));
	this->addChild(Background);

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	MenuItemImage *playMenuItem = MenuItemImage::create(
		"PlayItem.png",
		"PlayItem.png",
		CC_CALLBACK_1(HelloWorld::menuItemPlayCallback, this)
		);
	playMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(500,250)));
	playMenuItem->setScale(1.2f);

	MenuItemImage *SettingMenuItem = MenuItemImage::create(
		"SettingItem.png",
		"SettingItem.png",
		CC_CALLBACK_1(HelloWorld::menuItemSettingCallback, this)
		);
	SettingMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(500, 380)));
	SettingMenuItem->setScale(1.2f);

	MenuItemImage *HelpMenuItem = MenuItemImage::create(
		"HelpItem.png",
		"HelpItem.png",
		CC_CALLBACK_1(HelloWorld::menuItemHelpCallback, this)
		);
	HelpMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(500,510)));
	HelpMenuItem->setScale(1.2f);

	Menu *menu = Menu::create(playMenuItem, SettingMenuItem, HelpMenuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Background.mp3", true);
    return true;
}

void HelloWorld::menuItemPlayCallback(cocos2d::Ref* pSender)
{
	auto sc = LevelScene::createScene();
	auto reScene = TransitionFadeUp::create(1.0f, sc);
	Director::getInstance()->pushScene(reScene);
}

void HelloWorld::menuItemSettingCallback(cocos2d::Ref* pSender)
{
	auto sc = SettingScene::createScene();
	auto reScene = TransitionFadeUp::create(1.0f, sc);
	Director::getInstance()->pushScene(reScene);
}

void HelloWorld::menuItemHelpCallback(cocos2d::Ref* pSender)
{
	auto sc = HelpScene::createScene();
	auto reScene = TransitionFadeUp::create(1.0f, sc);
	Director::getInstance()->pushScene(reScene);
}