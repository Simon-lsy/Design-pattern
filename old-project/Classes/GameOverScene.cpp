#include "GameOverScene.h"
#include "LevelScene.h"
#include "PlayScene.h"
USING_NS_CC;

Scene* GameWinScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameWinScene::create();
	scene->addChild(layer);

	return scene;
}

Scene* GameLoseScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameLoseScene::create();
	scene->addChild(layer);

	return scene;
}


bool GameOverScene::init()
{

	auto backMenuItem = MenuItemImage::create(
		"BackUp.png",
		"BackDown.png",
		CC_CALLBACK_1(GameOverScene::menuBackCallback, this)
		);
	backMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(750, 550)));

	auto replayMenuItem = MenuItemImage::create(
		"replayUp.png",
		"replayDown.png",
		CC_CALLBACK_1(GameOverScene::menuReplayCallback, this)
		);
	replayMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(850, 550)));

	Menu* menu = Menu::create(backMenuItem,replayMenuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

bool GameWinScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *background = Sprite::create("GameWinScene.jpg");
	background->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(background);
	GameOverScene::init();
}

bool GameLoseScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *background = Sprite::create("GameLoseScene.jpg");
	background->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(background);
	GameOverScene::init();
}

void GameOverScene::menuReplayCallback(Object* pSender)
{
	auto playScene = PlayScene1::createScene();
	Director::getInstance()->replaceScene(playScene);
}

void GameOverScene::menuBackCallback(Object* pSender)
{
	auto levelScene = LevelScene::createScene();
	Director::getInstance()->replaceScene(levelScene);
}