#include "TestJoystick.h"

USING_NS_CC;

bool TestJoystick::init()
{
    if(!TestBase::init())
    {
        return false;
    }
    
    m_label = Label::createWithTTF("", "Joystick/fonts/arial.ttf", 24);
	m_label->setAnchorPoint(Vec2(0.5f,1.0f));
    m_label->setPosition(Vec2(m_visibleOrigin.x + m_visibleSize.width / 2.0f,
                              m_visibleOrigin.y + m_visibleSize.height));
    this->addChild(m_label, 1);

	
    Sprite* pSprite = Sprite::create("Joystick/HelloWorld.png");
    pSprite->setPosition(Vec2(m_visibleOrigin.x + m_visibleSize.width / 2.0f,
							  m_visibleOrigin.y + m_visibleSize.height / 2.0f));
    this->addChild(pSprite, 0);


	m_ball = CCSprite::create("Joystick/ball.png");
	m_ball->setScale(0.5f);
	m_ball->setPosition(Vec2(m_visibleOrigin.x + m_visibleSize.width / 2.0f,
							 m_visibleOrigin.y + m_visibleSize.height / 2.0f));
	this->addChild(m_ball,2);

	//创建摇杆实
	Joystick* j = Joystick::create("Joystick/joystickBg.png",70.0f,"Joystick/joystickHandle.png",50.0f);
	j->setPosition(Vec2(70.0f,70.0f));

	//这两种设置回调的方法都可以
	//j->setHandleEventListener(this,joystickEvent_selector(TestJoystick::moveSpr));
	j->setHandleEventListener(JOYSTICK_CALLBACK(TestJoystick::moveSpr,this));
	
	this->addChild(j,3);

    return true;
}

//摇杆事件处理
void TestJoystick::moveSpr(float interval,float x , float y,JoystickEventType type)
{
	
	Point pos = m_ball->getPosition();
	
	pos.x+=x*0.5f;
	
	pos.y+=y*0.5f;
	
	m_ball->setPosition(pos);

	std::string str;

	switch(type)
	{
	case JoystickEventType::JET_TOUCH_BEGIN:
		str = "Begin";
		break;
	case JoystickEventType::JET_TOUCH_MOVE:
		str = "Move";
		break;
	case JoystickEventType::JET_TOUCH_END:
		str = "End";
		break;
	}

	m_label->setString(str.c_str());
}
