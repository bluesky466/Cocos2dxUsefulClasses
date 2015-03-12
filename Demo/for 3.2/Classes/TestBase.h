#ifndef _TEST_BASE_H_
#define _TEST_BASE_H_

#include "cocos2d.h"

#define CREATE_SCENE(__TYPE__) \
static cocos2d::Scene* createScene() \
{ \
    auto scene = cocos2d::Scene::create(); \
    auto layer = __TYPE__::create(); \
    scene->addChild(layer); \
    return scene; \
}

class TestBase : public cocos2d::Layer
{
public:
    virtual bool init();  
    
	virtual void btBackCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(TestBase);

protected:
	cocos2d::Vec2 m_visibleCenter;
	cocos2d::Vec2 m_visibleOrigin;
	cocos2d::Size m_visibleSize;

	virtual void setDesignResolutionSize()
	{	
		cocos2d::Director::getInstance()->getOpenGLView()->setDesignResolutionSize(480.0f,320.0f,ResolutionPolicy::SHOW_ALL);
	}
};


#endif