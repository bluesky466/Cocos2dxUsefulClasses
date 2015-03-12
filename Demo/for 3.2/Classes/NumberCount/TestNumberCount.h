#ifndef _TEST_NUMBER_COUNT_H_
#define _TEST_NUMBER_COUNT_H_

#include "cocos2d.h"
#include "TestBase.h"
#include "NumberCount.h"

class TestNumberCount : public TestBase
{

public:
    virtual bool init();  
    
    void increaseCallback(cocos2d::Ref* pSender);
	void reduceCallback(cocos2d::Ref* pSender);
	void gotoCallback(cocos2d::Ref* pSender);
    void finishCallback(int targetNumber);
	
    CREATE_FUNC(TestNumberCount);
    CREATE_SCENE(TestNumberCount);

private:
	NumberCount     m_numberCount;
	cocos2d::Label* m_finishTip;
};

#endif
