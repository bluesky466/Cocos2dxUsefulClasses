#include "HelloWorldScene.h"
#include "editor-support\cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"

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
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "visible.png",
                                           "visible.png",
                                           CC_CALLBACK_1(HelloWorld::btVisbleCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width  - closeItem->getContentSize().width/2 ,
								origin.y + visibleSize.height - closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
	/*Frame的测试*/

	Widget* frame = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("UIRes/DemoLogin.ExportJson");
	frame->setScale(0.8f); //缩放比例可以随便设置
	frame->setAnchorPoint(Vec2(0.5f,0.5f));//锚点可以随便设置
	frame->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));//坐标可以随便设置
	this->addChild(frame);
	
	//触碰测试
	Helper::seekWidgetByName(frame,"login_Button")->addTouchEventListener(CC_CALLBACK_2(HelloWorld::btMenuCallBack,this));
	Helper::seekWidgetByName(frame,"close_Button")->addTouchEventListener(CC_CALLBACK_2(HelloWorld::btMenuCallBack,this));
	Helper::seekWidgetByName(frame,"agree_CheckBox")->addTouchEventListener(CC_CALLBACK_2(HelloWorld::btMenuCallBack,this));
	Helper::seekWidgetByName(frame,"name_TextField")->addTouchEventListener(CC_CALLBACK_2(HelloWorld::btMenuCallBack,this));
	Helper::seekWidgetByName(frame,"password_TextField")->addTouchEventListener(CC_CALLBACK_2(HelloWorld::btMenuCallBack,this));
	Helper::seekWidgetByName(frame,"confirm_TextField")->addTouchEventListener(CC_CALLBACK_2(HelloWorld::btMenuCallBack,this));

	//frame初始化,可以试一下不初始化，直接layer->setVisible(false);设置窗口不可见之后,鼠标点击原本的按钮区域，看看是否有对话框出现
	m_frame.setFrame(frame,Helper::seekWidgetByName(frame,"backageImg"));
	m_frame.setFrameVisible(true);



    return true;
}

void HelloWorld::btVisbleCallback(Ref* pSender)
{
	m_frame.setFrameVisible(true);
}

void HelloWorld::btMenuCallBack(Ref* pObject,Widget::TouchEventType eventType)
{
	if(eventType == Widget::TouchEventType::ENDED)
	{
		MessageBox("","");
	}
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
