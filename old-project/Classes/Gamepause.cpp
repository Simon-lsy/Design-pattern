#include"cocos2d.h"
#include "Gamepause.h" 
#include"HelloWorldScene.h"
#include"levelScene.h"
USING_NS_CC;

//传入一个CCrenderTexture   
//相当于一个正在运行的游戏的截图作为这个暂停对话框的背景   
//这样就看起来像是对话框在游戏界面之上，一般游戏当中都是这样子写的。  

Scene* Gamepause::scene(RenderTexture* sqr)
{
	Scene *scene = Scene::create();
	Gamepause *layer = Gamepause::create();
	scene->addChild(layer,1);
	//增加部分：使用Game界面中截图的sqr纹理图片创建Sprite  
	//并将Sprite添加到GamePause场景层中  
	//得到窗口的大小  
	Size visibleSize = Director::getInstance()->getWinSize();
	Sprite *back_spr = Sprite::createWithTexture(sqr->getSprite()->getTexture());
	back_spr->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2)); //放置位置,这个相对于中心位置。  
	back_spr->setFlipY(true);            //翻转，因为UI坐标和OpenGL坐标不同  
	back_spr->setColor(Color3B::GRAY); //图片颜色变灰色  
	scene->addChild(back_spr);
	return scene;
}

bool Gamepause::init()
{

	if (!Layer::init())
	{
		return false;
	}
	//得到窗口的大小  
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//原点坐标  
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *background = Sprite::create("help.png");
	background->setScale(0.6f);
	background->setPosition(visibleSize / 2);
	this->addChild(background);

	//继续游戏按钮  
	MenuItemImage *ContinueItem = MenuItemImage::create(
		"nextUp.png",
		"nextDown.png",
		this,
		menu_selector(Gamepause::menuContinueCallback));

	ContinueItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 ));

	MenuItem *backItem = MenuItemImage::create(
		"backUp.png",
		"backDown.png",
		this,
		menu_selector(Gamepause::menuBcakCallback)
	);
	backItem->setPosition(visibleSize.width / 2 - 100, visibleSize.height / 2);

	MenuItem *replayItem = MenuItemImage::create(
		"replayUp.png",
		"replayDown.png",
		this,
		menu_selector(Gamepause::menuReplayCallback)
		);
	replayItem->setPosition(visibleSize.width / 2 + 100, visibleSize.height / 2);

	Menu* Menu = Menu::create(ContinueItem,backItem,replayItem,NULL);
	Menu->setPosition(Vec2::ZERO);
	this->addChild(Menu, 2);
	return true;
}
void Gamepause::menuContinueCallback(Object* pSender)
{
	Director::getInstance()->popScene();
}
void Gamepause::menuReplayCallback(Object* pSender)
{
	auto playScene = PlayScene1::createScene();
	Director::getInstance()->replaceScene(playScene);
}
void Gamepause::menuBcakCallback(Object* pSender)
{
	auto levelScene = LevelScene::createScene();
	Director::getInstance()->replaceScene(levelScene);
}
