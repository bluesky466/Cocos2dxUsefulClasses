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
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::setNumberCallback));
    
	pItem->setPosition(ccp(origin.x + visibleSize.width - pItem->getContentSize().width/2 ,
                                origin.y + pItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

	CCLabelAtlas* pNumber = CCLabelAtlas::create("","number.png",55,84,'0');
	pNumber->setAnchorPoint(ccp(0.5f,0.5f));
	pNumber->setPosition(ccp(origin.x + visibleSize.width/2.0f,
                             origin.y + visibleSize.height/2.0f));
	this->addChild(pNumber);

	m_numberCount.setLabelAtlas(pNumber);
	m_numberCount.setNumber(100,false);
    return true;
}


void HelloWorld::setNumberCallback(CCObject* pSender)
{
	m_numberCount.setIncrement(100);
}
