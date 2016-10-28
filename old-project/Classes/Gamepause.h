#include "cocos2d.h"  
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
class Gamepause : public cocos2d::CCLayer
{
public:
	virtual bool init();
	static cocos2d::Scene* scene(RenderTexture* sqr);

	CREATE_FUNC(Gamepause);
	//¼ÌÐøÓÎÏ·  
	void menuContinueCallback(Object* pSender); 
	void menuReplayCallback(Object* pSender);
	void menuBcakCallback(Object* pSender);
private:

};