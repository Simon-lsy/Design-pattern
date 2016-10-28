#include "LevelScene.h"
#include "HelloWorldScene.h"
USING_NS_CC;

Scene* LevelScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LevelScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool LevelScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	currentPage = 1;
	addScrollView();
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LevelScene::onTouchBegan, this);

	listener->onTouchMoved = CC_CALLBACK_2(LevelScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(LevelScene::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto backMenuItem = MenuItemImage::create(
		"BackUp.png",
		"BackDown.png",
		CC_CALLBACK_1(LevelScene::menuBackCallback, this)
		);
	backMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(600, 600)));

	Menu* menu = Menu::create(backMenuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void LevelScene::addScrollView()
{
	slideShow = Layer::create();

	Sprite *levelScene1 = Sprite::create("levelBackground1.png");
	Sprite *levelScene2 = Sprite::create("levelBackground2.png");
	Sprite *levelScene3 = Sprite::create("levelBackground3.png");

	slideShow->addChild(levelScene1);
	levelScene1->setPosition(Vec2(visibleSize.width/2, visibleSize.height / 2));

	slideShow->addChild(levelScene2);
	levelScene2->setPosition(Vec2(visibleSize.width / 2 * 3, visibleSize.height / 2));

	slideShow->addChild(levelScene3);
	levelScene3->setPosition(Vec2(visibleSize.width / 2 * 5, visibleSize.height / 2));

	slideShow->setPosition(Vec2::ZERO);
	slideShow->setContentSize(CCSize(visibleSize.width * MAP_MOUNT, visibleSize.height));

	scrollView = ScrollView::create();

	scrollView->setContainer(slideShow);
	scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
	scrollView->setTouchEnabled(true);
	scrollView->setPosition(Vec2::ZERO);
	scrollView->setViewSize(visibleSize);
	scrollView->setContentOffset(Size(0, 0), true);
	scrollView->setContentSize(Size(visibleSize.width * MAP_MOUNT, visibleSize.height));

	scrollView->setDelegate(this);

	scrollView->setBounceable(false);

	addChild(scrollView);
}

bool LevelScene::onTouchBegan(Touch*touch, Event *unused_event)
{
	beginTouchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
	return true;
}

void LevelScene::onTouchMoved(Touch*touch, Event*unused_event)
{

}

void LevelScene::onTouchEnded(Touch*touch, Event *unused_event)
{

	Point endPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
	float distance = endPoint.x - beginTouchPoint.x;
	if (fabs(distance) > TOUCH_LENGTH)
	{
		adjustScrollView(distance);
	}
	else
	{
		if (currentPage == 1)
		{
			auto scene = PlayScene1::createScene();
			auto reScene = TransitionFadeUp::create(1.0f, scene);
			Director::getInstance()->pushScene(reScene);
		}
		if (currentPage == 2)
		{
			auto scene = PlayScene2::createScene();
			auto reScene = TransitionFadeUp::create(1.0f, scene);
			Director::getInstance()->pushScene(reScene);
		}
		if (currentPage == 3)
		{
			auto scene = PlayScene3::createScene();
			auto reScene = TransitionFadeUp::create(1.0f, scene);
			Director::getInstance()->pushScene(reScene);
		}
	}
}

void LevelScene::adjustScrollView(float offset)
{

	if (offset<0)
	{
		currentPage++;
	}
	else
	{
		currentPage--;
	}

	if (currentPage <1)
	{
		currentPage = 1;
	}

	if (currentPage > MAP_MOUNT)
	{
		currentPage = MAP_MOUNT;
	}

	Point  adjustPos = Vec2(-visibleSize.width * (currentPage - 1), 0);
	scrollView->setContentOffset(adjustPos, true);
}

void LevelScene::menuBackCallback(Ref* pSender)
{
	auto scene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(scene);
}