#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "NumberCount.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    virtual bool init();  

    static cocos2d::CCScene* scene();
    
    void increaseCallback(cocos2d::CCObject* pSender);
	void reduceCallback(cocos2d::CCObject* pSender);
	void gotoCallback(cocos2d::CCObject* pSender);
    void finishCallback(int targetNumber);
    CREATE_FUNC(HelloWorld);

private:
	NumberCount m_numberCount;
	cocos2d::CCLabelTTF* m_finishTip;
};

#endif 
