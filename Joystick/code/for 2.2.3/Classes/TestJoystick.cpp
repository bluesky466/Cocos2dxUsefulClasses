#include "TestJoystick.h"

USING_NS_CC;

CCScene* TestJoystick::createScene()
{
	CCScene* scene = CCScene::create();

	TestJoystick* layer = TestJoystick::create();
    scene->addChild(layer);
    return scene;
}


bool TestJoystick::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
  
	m_isFollow = true;
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    CCSprite* pSprite = CCSprite::create("HelloWorld.png");
    pSprite->setPosition(ccp(visibleOrigin.x + visibleSize.width / 2.0f,
							 visibleOrigin.y + visibleSize.height / 2.0f));
    this->addChild(pSprite, 0);

	CCMenuItemToggle* toggle = CCMenuItemToggle::createWithTarget(this,
														menu_selector(TestJoystick::menuItemToggleCallback),
														CCMenuItemFont::create("follow"),
														CCMenuItemFont::create("normal"),
														NULL);
	toggle->setAnchorPoint(ccp(0.5f,0.5f));

	this->addChild(CCMenu::create(toggle,NULL),1);


	m_ball = CCSprite::create("ball.png");
	m_ball->setScale(0.5f);
	m_ball->setPosition(ccp(200.0f,200.0f));
	this->addChild(m_ball,2);

	//创建摇杆实
	m_joystick = Joystick::create("joystickBg.png",70.0f,"joystickHandle.png",50.0f);
	m_joystick->setPosition(ccp(70.0f,70.0f));
	m_joystick->setVisible(false);

	//摇杆触碰事件的回调
	m_joystick->setHandleEventListener(this,joystickEvent_selector(TestJoystick::moveSpr));

	//触碰在摇杆外部的回调，用于follow模式下的摇杆，如果该回调返回true,则以后将继续接收JET_TOUCH_BEGIN,JET_TOUCH_MOVE和JET_TOUCH_END消息
	m_joystick->setTouchOutsideHandleEventListener(this,touchOutsideHandleEvent_selector(TestJoystick::touchOutsideHandle));
	
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
		m_ball->setPosition(ccp(m_ball->getPosition().x + x * 0.5f,
								m_ball->getPosition().y + y * 0.5f));
		break;

	case JoystickEventType::JET_TOUCH_END:
		if(m_isFollow)
			joystick->setVisible(false);
		break;
	}
}


bool TestJoystick::touchOutsideHandle(Joystick* joystick,const cocos2d::CCPoint& position)
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

void TestJoystick::menuItemToggleCallback(CCObject* item)
{
	if(((CCMenuItemToggle*)item)->getSelectedIndex() == 0)
	{
		m_joystick->setVisible(false);
		m_isFollow = true;
	}
	else
	{
		m_joystick->setPosition(ccp(70,70));
		m_joystick->setVisible(true);
		m_isFollow = false;
	}
}