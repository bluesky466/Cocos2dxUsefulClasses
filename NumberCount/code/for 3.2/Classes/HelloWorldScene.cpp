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
	pNumber->setAnchorPoint(Vec2(0.5f,0.5f));
	pNumber->setPosition(Vec2(visibleSize.width/2.0f,
                             visibleSize.height/2.0f));

	this->addChild(pNumber);

	m_numberCount.setLabelAtlas(pNumber); //如果是从cocostudio导出的话请调用NumberCount::setTextAtlas

	//将数字每一步的增量初始化为3
	m_numberCount.setNumberDelta(3);

	//将数字增加时间间隔设置为0.1秒
	m_numberCount.setTimeDelta(0.1f);

	//将数字初始化为100
	m_numberCount.setNumber(100,false);
    return true;
}

void HelloWorld::increaseCallback(Ref* pSender)
{
	//如果是setIncrement的话快速连续按多下按钮，得到的结果不是想要的
	m_numberCount.addIncrement(20);
}

void HelloWorld::reduceCallback(Ref* pSender)
{
	if(m_numberCount.getNumber()-20>0)
	{
		//如果是setIncrement的话快速连续按多下按钮，得到的结果不是想要的
		m_numberCount.addIncrement(-20);
	}
}

void HelloWorld::gotoCallback(Ref* pSender)
{
	m_numberCount.setNumber(100,true);
}