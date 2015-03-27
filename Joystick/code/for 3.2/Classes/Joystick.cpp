#include "Joystick.h"

USING_NS_CC;

Joystick* Joystick::create(const char *fnBg,float bgRadius,const char *fnHandle,float handleRadius)
{
    Joystick *joystick = new Joystick();
    if (joystick && joystick->init(fnBg,bgRadius,fnHandle,handleRadius))
    {
        joystick->autorelease();
        return joystick;
    }

    CC_SAFE_DELETE(joystick);
    return NULL;
}

bool Joystick::init(const char *fnBg,float bgRadius,const char *fnHandle,float handleRadius)
{
	//创建底盘精灵
	m_bg = Sprite::create(fnBg);
	if(!m_bg)
		return false;
	this->addChild(m_bg);

	//创建摇杆精灵
	m_handle = Sprite::create(fnHandle);
	if(!m_handle)
		return false;
	this->addChild(m_handle);
	
	//初始化参数
	m_bgRadius     = bgRadius;
	m_handleRadius = handleRadius;
	m_handlePos    = Vec2(0.0f,0.0f);
	m_touchEventCallback = nullptr;
	m_touchOutsideCallback = nullptr;

	//设置底盘的大小
	float bgDiameter = bgRadius * 2;
	Size oriBgSize = m_bg->getContentSize();
	m_bg->setScaleX(bgDiameter / oriBgSize.width);
	m_bg->setScaleY(bgDiameter / oriBgSize.height);

	//设置摇杆的大小
	float handleDiameter = handleRadius * 2;
	Size oriHandleSize = m_handle->getContentSize();
	m_handle->setScaleX(handleDiameter / oriHandleSize.width);
	m_handle->setScaleY(handleDiameter / oriHandleSize.height);

	//设置定时器
	this->schedule(schedule_selector(Joystick::callHandleEvent));

	//设置触碰触碰事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan     = CC_CALLBACK_2(Joystick::onTouchBegan,this);
	listener->onTouchMoved     = CC_CALLBACK_2(Joystick::onTouchMoved,this);
	listener->onTouchEnded     = CC_CALLBACK_2(Joystick::onTouchEnded,this);
	listener->onTouchCancelled = CC_CALLBACK_2(Joystick::onTouchCancelled,this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

	return true;

}

bool Joystick::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Vec2 point = this->convertTouchToNodeSpaceAR(pTouch);
	
	//判断触点是否在摇杆上
	if(point.x*point.x+point.y*point.y < m_handleRadius*m_handleRadius)
	{
		m_bMove = true;

		setHandlePosition(pTouch->getLocation());

		Vec2 pos = m_handle->getPosition();

		if(m_touchEventCallback)
			m_touchEventCallback(this,0.0f,pos.x/m_bgRadius,pos.y/m_bgRadius,JoystickEventType::JET_TOUCH_BEGIN);

		return true;
	}
	else if(m_touchOutsideCallback && m_touchOutsideCallback(this,pTouch->getLocation()))
	{
		m_bMove = true;

		setHandlePosition(pTouch->getLocation());

		point   = this->convertTouchToNodeSpaceAR(pTouch);
		
		Vec2 pos = m_handle->getPosition();

		if(m_touchEventCallback)
			m_touchEventCallback(this,0.0f,pos.x/m_bgRadius,pos.y/m_bgRadius,JoystickEventType::JET_TOUCH_BEGIN);

		return true;
	}

	return false;
}

void Joystick::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	setHandlePosition(pTouch->getLocation());
}

void Joystick::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Vec2 pos = m_handle->getPosition();

	if(m_touchEventCallback)
		m_touchEventCallback(this,0.0f,pos.x/m_bgRadius,pos.y/m_bgRadius,JoystickEventType::JET_TOUCH_END);

	m_handle->setPosition(Vec2(0.0f,0.0f));

	m_bMove = false;
}

void Joystick::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
	Vec2 pos = m_handle->getPosition();

	if(m_touchEventCallback)
		m_touchEventCallback(this,0.0f,pos.x/m_bgRadius,pos.y/m_bgRadius,JoystickEventType::JET_TOUCH_END);
	
	m_handle->setPosition(Vec2(0.0f,0.0f));

	m_bMove = false;
}

void Joystick::callHandleEvent(float interval)
{
	if(m_bMove)
	{
		//调用摇杆事件处理方法
		Vec2 point = m_handle->getPosition();

		if(m_touchEventCallback)
			m_touchEventCallback(this,interval,point.x/m_bgRadius,point.y/m_bgRadius,JoystickEventType::JET_TOUCH_MOVE);
	}
}

void Joystick::setHandlePosition(const cocos2d::Vec2& position)
{
	//将摇杆限制在底盘的范围内
	Vec2 point = this->convertToNodeSpaceAR(position);

	if(point.x*point.x+point.y*point.y > m_bgRadius*m_bgRadius)
	{
		point.normalize();
		point = point * m_bgRadius;
	}

	m_handle->setPosition(point);
}