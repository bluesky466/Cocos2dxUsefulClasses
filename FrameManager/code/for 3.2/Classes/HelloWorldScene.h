#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "FrameManager.h"

USING_NS_CC;
using namespace ui;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
	void btVisbleCallback(Ref*);
	void btMenuCallBack(Ref*,Widget::TouchEventType);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	FrameManager m_frame;
};

#endif // __HELLOWORLD_SCENE_H__
