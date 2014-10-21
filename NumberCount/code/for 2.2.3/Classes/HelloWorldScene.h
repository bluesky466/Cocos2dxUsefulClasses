#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "NumberCount.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    virtual bool init();  

    static cocos2d::CCScene* scene();
    
    void increaseCallback(CCObject* pSender);
	void reduceCallback(CCObject* pSender);
	void gotoCallback(CCObject* pSender);
    
    CREATE_FUNC(HelloWorld);

private:
	NumberCount m_numberCount;
};

#endif 
