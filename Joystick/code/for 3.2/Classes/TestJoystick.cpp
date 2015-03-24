#include "TestJoystick.h"

USING_NS_CC;

Scene* TestJoystick::createScene()
{
	Scene* scene = Scene::create();

	TestJoystick* layer = TestJoystick::create();
    scene->addChild(layer);
    return scene;
}


bool TestJoystick::init()
{
    if(!Layer::init())
    {
        return false;
    }
  
	m_isFollow = true;
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

    Sprite* pSprite = Sprite::create("HelloWorld.png");
    pSprite->setPosition(Vec2(visibleOrigin.x + visibleSize.width / 2.0f,
							  visibleOrigin.y + visibleSize.height / 2.0f));
    this->addChild(pSprite, 0);

	MenuItemToggle* toggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(TestJoystick::menuItemToggleCallback,this),
																MenuItemFont::create("follow"),
																MenuItemFont::create("normal"),
																NULL);
	toggle->setAnchorPoint(Vec2(0.5f,0.5f));

	this->addChild(Menu::create(toggle,NULL),1);


	m_ball = CCSprite::create("ball.png");
	m_ball->setScale(0.5f);
	m_ball->setPosition(Vec2(200.0f,200.0f));
	this->addChild(m_ball,2);

	//创建摇杆实
	m_joystick = Joystick::create("joystickBg.png",70.0f,"joystickHandle.png",50.0f);
	m_joystick->setPosition(Vec2(70.0f,70.0f));
	m_joystick->setVisible(false);

	//摇杆触碰事件的回调
	m_joystick->setHandleEventListener(JOYSTICK_CALLBACK(TestJoystick::moveSpr,this));

	//触碰在摇杆外部的回调，用于follow模式下的摇杆，如果该回调返回true,则以后将继续接收JET_TOUCH_BEGIN,JET_TOUCH_MOVE和JET_TOUCH_END消息
	m_joystick->setTouchOutsideCallback(CC_CALLBACK_2(TestJoystick::touchOutsideHandle,this));
	
	this->addChild(m_joystick,3);

    return true;
}

//摇杆事件处理
void TestJoystick::moveSpr(Joystick* joystick,float interval,float x , float y,JoystickEventType type)
{
	switch(type)
	{
	case JoystickEventType::JET_TOUCH_BEGIN:
		joystick->setVisible(true);
		break;

	case JoystickEventType::JET_TOUCH_MOVE:
		m_ball->setPosition(m_ball->getPosition().x + x * 0.5f,
							m_ball->getPosition().y + y * 0.5f);
		break;

	case JoystickEventType::JET_TOUCH_END:
		if(m_isFollow)
			joystick->setVisible(false);
		break;
	}
}


bool TestJoystick::touchOutsideHandle(Joystick* joystick,const cocos2d::Vec2& position)
{
	//如果该回调返回true,则以后将继续接收JET_TOUCH_BEGIN,JET_TOUCH_MOVE和JET_TOUCH_END消息

	if(m_isFollow)
	{
		joystick->setPosition(position);
		return true;
	}
	else
	{
		return false;
	}
}

void TestJoystick::menuItemToggleCallback(Ref* item)
{
	if(((MenuItemToggle*)item)->getSelectedIndex() == 0)
	{
		m_joystick->setVisible(false);
		m_isFollow = true;
	}
	else
	{
		m_joystick->setPosition(Vec2(70,70));
		m_joystick->setVisible(true);
		m_isFollow = false;
	}
}