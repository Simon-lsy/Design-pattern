#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include "cocos2d.h"
#include "GameDefine.h"
USING_NS_CC;

class PlayScene;

class PlaySceneCover : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(PlaySceneCover);

	cocos2d::Label *labelTime;
	cocos2d::Label *score;
	cocos2d::Label *state;
	void myclock(float dt);
	void myscore(float dt);
	void mystate(float dt);
	CC_SYNTHESIZE(PlayScene*, _gameLayer, GameLayer);

	void win();
	void lose();
};

class PlayScene : public cocos2d::Layer
{
protected:
	cocos2d::TMXTiledMap *_tileMap;
	cocos2d::TMXLayer *_background;
	cocos2d::Sprite *_player;
	cocos2d::Sprite *helper;
	cocos2d::TMXLayer *_meta;
	cocos2d::TMXLayer *_foreground_1;
	cocos2d::TMXLayer *_foreground_2;
	cocos2d::TMXLayer *_foreground_3;
	cocos2d::TMXLayer *_foreground_4;
	cocos2d::TMXLayer *_foreground_5;
	cocos2d::TMXLayer *_foreground_6;

	float actualDuration;

	int using_time=5;
	int heroHp = 3;

	bool group_2=true;
	bool group_3=true;
	bool done_2=false;
	bool done_3=false;
	bool guard_done=false;

	bool get_key = false;
	bool move_guard = false;
	bool speedup = false;
	bool get_helper = false;
	bool get_bullet = false;
	bool show_words = false;

	cocos2d::Point special_pos;

	cocos2d::Vector<cocos2d::Sprite *> stable_enemies;
	cocos2d::Vector<cocos2d::Sprite *> unchanged_enemies;
	cocos2d::Vector<cocos2d::Sprite *> _enemies;
	cocos2d::Vector<cocos2d::Sprite *> _enemies_2;
	cocos2d::Vector<cocos2d::Sprite *> _projectiles;

	static PlaySceneCover *_cover;
public:
	virtual bool init();
	CREATE_FUNC(PlayScene);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void ViewPoint(cocos2d::Point position);
	void PlayerPosition(cocos2d::Point position);
	

	cocos2d::Point tileToPosition(cocos2d::Point position);

	void Enemy_Stable(cocos2d::Point pos);
	void Enemy_Move(cocos2d::Point pos, int i);
	
	void addEnemy(cocos2d::Point pos);
	void addEnemy_2(cocos2d::Point pos);
	
	void projectileMoveFinished(Object *pSender);
	void enemyMoveFinished(cocos2d::Object *pSender);
	void moveEnemy(cocos2d::Sprite *enemy);
	
	void testCollisions(float dt);
	void test_super(float dt);
	void test_guard(float dt);
	void unchanged_test(float dt);
	
	void lose();
	
	void bullet(float dt);
	void myusingtime(float dt);
	void myhelper(float dt);
	void helper_get(float dt);
	
	void menuPauseCallback(Object* pSender);
};

class PlayScene1 : public PlayScene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(PlayScene1);
};

class PlayScene2 : public PlayScene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(PlayScene2);
};

class PlayScene3 : public PlayScene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(PlayScene3);
};

#endif // __HELLOWORLD_SCENE_H__

