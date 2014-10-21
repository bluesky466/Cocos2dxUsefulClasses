#include "HelloWorldScene.h"

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
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "visible.png",
                                        "visible.png",
                                        this,
                                        menu_selector(HelloWorld::btVisbleCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width  - pCloseItem->getContentSize().width/2 ,
								origin.y + visibleSize.height - pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);


    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
 


	/*Frame的测试*/
	
	Widget* frame = GUIReader::shareReader()->widgetFromJsonFile("UIRes/DemoLogin.ExportJson");
	
	UILayer* layer =UILayer::create();
	layer->setScale(0.8f); //缩放比例可以随便设置
	layer->addWidget(frame);
	this->addChild(layer);

	//触碰测试
	layer->getWidgetByName("login_Button")->addTouchEventListener(this,toucheventselector(HelloWorld::btMenuCallBack));
	layer->getWidgetByName("close_Button")->addTouchEventListener(this,toucheventselector(HelloWorld::btMenuCallBack));
	layer->getWidgetByName("agree_CheckBox")->addTouchEventListener(this,toucheventselector(HelloWorld::btMenuCallBack));
	layer->getWidgetByName("name_TextField")->addTouchEventListener(this,toucheventselector(HelloWorld::btMenuCallBack));
	layer->getWidgetByName("password_TextField")->addTouchEventListener(this,toucheventselector(HelloWorld::btMenuCallBack));
	layer->getWidgetByName("confirm_TextField")->addTouchEventListener(this,toucheventselector(HelloWorld::btMenuCallBack));

	//frame初始化,可以试一下不初始化，直接layer->setVisible(false);设置窗口不可见之后,鼠标点击原本的按钮区域，看看是否有对话框出现
	m_frame.setFrame(frame,(UIImageView*)layer->getWidgetByName("backageImg"));
	m_frame.setFrameVisible(true);


    return true;
}


void HelloWorld::btVisbleCallback(CCObject* pSender)
{
	m_frame.setFrameVisible(true);
}

void HelloWorld::btMenuCallBack(CCObject* pObject,TouchEventType eventType)
{
	if(eventType == TOUCH_EVENT_ENDED)
	{
		CCMessageBox("","");
	}
}