#ifndef _TEST_JOYSTICK_H_
#define _TEST_JOYSTICK_H_

#include "cocos2d.h"
#include "TestBase.h"
#include "Joystick.h"

class TestJoystick : public TestBase
{
public:
    bool init();  

	//摇杆事件处理
	void moveSpr(Joystick* joystick,float interval,float x , float y,JoystickEventType type);

	//触碰在摇杆外部的事件处理，在follow模式下的摇杆有用,如果该回调返回true,则以后将继续接收JET_TOUCH_BEGIN,JET_TOUCH_MOVE和JET_TOUCH_END消息
	bool touchOutsideHandle(Joystick* joystick,const cocos2d::Vec2&);

	
	void menuItemToggleCallback(Ref*);

    CREATE_FUNC(TestJoystick);
    CREATE_SCENE(TestJoystick);

private:
	cocos2d::Sprite* m_ball;
	Joystick* m_joystick;
	bool      m_isFollow;
};

#endif
