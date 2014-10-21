#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
 
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	CCMenuItemFont* pIncrease = CCMenuItemFont::create("increase 20",this,menu_selector(HelloWorld::increaseCallback));
	pIncrease->setAnchorPoint(ccp(0.0f,1.0f));
	pIncrease->setPosition(0.0f,visibleSize.height - 0.0f);


	CCMenuItemFont* pReduce   = CCMenuItemFont::create("reduce 20",this,menu_selector(HelloWorld::reduceCallback));
	pReduce->setAnchorPoint(ccp(0.0f,1.0f));
	pReduce->setPosition(0.0f,visibleSize.height - 40.0f);

	CCMenuItemFont* pGoto     = CCMenuItemFont::create("goto 100",this,menu_selector(HelloWorld::gotoCallback));
	pGoto->setAnchorPoint(ccp(0.0f,1.0f));
	pGoto->setPosition(0.0f,visibleSize.height - 80.0f);

	CCMenu* pMenu = CCMenu::create(pIncrease,pReduce,pGoto, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);



	//////////////////////////////////////////////////////////////


	CCLabelAtlas* pNumber = CCLabelAtlas::create("","number.png",55,84,'0');
	pNumber->setAnchorPoint(ccp(0.5f,0.5f));
	pNumber->setPosition(ccp(visibleSize.width/2.0f,
                             visibleSize.height/2.0f));

	this->addChild(pNumber);

	m_numberCount.setLabelAtlas(pNumber);

	//将数字每一步的增量初始化为3
	m_numberCount.setNumberDelta(3);

	//将数字增加时间间隔设置为0.1秒
	m_numberCount.setTimeDelta(0.1f);

	//将数字初始化为100
	m_numberCount.setNumber(100,false);

    return true;
}


void HelloWorld::increaseCallback(CCObject* pSender)
{
	//如果是setIncrement的话快速连续按多下按钮，得到的结果不是想要的
	m_numberCount.addIncrement(20);
}

void HelloWorld::reduceCallback(CCObject* pSender)
{
	if(m_numberCount.getNumber()-20>0)
	{
		//如果是setIncrement的话快速连续按多下按钮，得到的结果不是想要的
		m_numberCount.addIncrement(-20);
	}
}

void HelloWorld::gotoCallback(CCObject* pSender)
{
	m_numberCount.setNumber(100,true);
}