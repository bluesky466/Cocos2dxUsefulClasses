#include "HelloWorldScene.h"

USING_NS_CC;
using namespace ui;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	Size visibleSize = Director::getInstance()->getVisibleSize();

	MenuItemFont* pIncrease = MenuItemFont::create("increase 20",CC_CALLBACK_1(HelloWorld::increaseCallback,this));
	pIncrease->setAnchorPoint(Vec2(0.0f,1.0f));
	pIncrease->setPosition(0.0f,visibleSize.height - 0.0f);


	MenuItemFont* pReduce   = MenuItemFont::create("reduce 20",CC_CALLBACK_1(HelloWorld::reduceCallback,this));
	pReduce->setAnchorPoint(Vec2(0.0f,1.0f));
	pReduce->setPosition(0.0f,visibleSize.height - 40.0f);

	MenuItemFont* pGoto     = MenuItemFont::create("goto 100",CC_CALLBACK_1(HelloWorld::gotoCallback,this));
	pGoto->setAnchorPoint(Vec2(0.0f,1.0f));
	pGoto->setPosition(0.0f,visibleSize.height - 80.0f);

	Menu* pMenu = Menu::create(pIncrease,pReduce,pGoto, NULL);
	pMenu->setPosition(Vec2(0.0f,0.0f));
    this->addChild(pMenu, 1);


	LabelAtlas* pNumber = LabelAtlas::create("","number.png",55,84,'0');
	pNumber->setAnchorPoint(Vec2(0.5f,0.0f));
	pNumber->setPosition(Vec2(visibleSize.width*0.5f,
                             visibleSize.height*0.5f));

	this->addChild(pNumber);

	m_finishTip = Label::create();
	m_finishTip->setAnchorPoint(Vec2(0.5f,1.0f));
	m_finishTip->setPosition(Vec2(visibleSize.width*0.5f,
                                  visibleSize.height*0.5f));
	this->addChild(m_finishTip);

	//如果是从cocostudio导出的话请调用NumberCount::setTextAtlas
	m_numberCount.setLabelAtlas(pNumber); 

	//将数字每一步的增量初始化为3
	m_numberCount.setNumberDelta(3);

	//将数字变化时间间隔设置为0.1秒
	m_numberCount.setTimeDelta(0.1f);

	//将数字初始化为100
	m_numberCount.setNumber(100,false);

	//设置数字停止变化时候的回调
	m_numberCount.setChangeFinishCallback(CC_CALLBACK_1(HelloWorld::finishCallback,this));

	//可以这样将它用于10秒倒计时
	//m_numberCount.setNumberDelta(1);
	//m_numberCount.setTimeDelta(1.0f);
	//m_numberCount.setNumber(9,false);
	//m_numberCount.setNumber(0,true);
	//m_numberCount.setChangeFinishCallback(CC_CALLBACK_1(HelloWorld::finishCallback,this));

    return true;
}

void HelloWorld::increaseCallback(Ref* pSender)
{
	m_numberCount.add(20);
	m_finishTip->setString("");
}

void HelloWorld::reduceCallback(Ref* pSender)
{
	if(m_numberCount.getNumberTarget()-20>0)
	{
		m_numberCount.add(-20);
		m_finishTip->setString("");
	}
}

void HelloWorld::gotoCallback(Ref* pSender)
{
	m_numberCount.setNumber(100,true);
	m_finishTip->setString("");
}

void HelloWorld::finishCallback(int targetNumber)
{
	m_finishTip->setString("finish !");
}