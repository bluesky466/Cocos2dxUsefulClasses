#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "NumberCount.h"
USING_NS_CC;
class HelloWorld : public cocos2d::Layer
{

public:
    virtual bool init();  

    static cocos2d::Scene* createScene();
    
    void increaseCallback(Ref* pSender);
	void reduceCallback(Ref* pSender);
	void gotoCallback(Ref* pSender);
    
    CREATE_FUNC(HelloWorld);

private:
	NumberCount m_numberCount;
};

#endif // __HELLOWORLD_SCENE_H__
