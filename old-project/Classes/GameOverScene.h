#ifndef __HELLO__GameOverScene__
#define __HELLO__GameOverScene__

#include "cocos2d.h"  
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
class GameOverScene : public cocos2d::Layer
{
public:
	virtual bool init();

	void menuReplayCallback(Object* pSender);
	void menuBackCallback(Object* pSender);
private:

};

class GameWinScene : public GameOverScene
{
public:

	virtual bool init();
	static cocos2d::Scene* createScene();
	CREATE_FUNC(GameWinScene);
};

class GameLoseScene : public GameOverScene
{
public:

	virtual bool init();
	static cocos2d::Scene* createScene();
	CREATE_FUNC(GameLoseScene);
};
#endif /* defined(__TileGame__GameOverScene__) */
