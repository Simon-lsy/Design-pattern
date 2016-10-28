#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__
#include "cocos2d.h"
#include "PlayScene.h"
#include "extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
#define MAP_MOUNT 3
#define TOUCH_LENGTH 20
class LevelScene : public Layer, public ScrollViewDelegate
{
public:

	static Scene* createScene();
	virtual bool init();
	void menuBackCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(LevelScene);

private:

	Layer* slideShow;

	int currentPage;
	Point beginTouchPoint;
	ScrollView *scrollView;

	void singleTouchDown(Object* pSender, Control::EventType event);

	void addScrollView();

	void adjustScrollView(float distance);
	void onTouchEnded(Touch*touch, Event *unused_event);
	bool onTouchBegan(Touch * touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	Size visibleSize = Director::getInstance()->getVisibleSize();
};

#endif