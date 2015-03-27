/****************************************************************************
 for cocos2d-x 2.2.3

 author : LuoYu
 
 e-mail : 466474482@qq.com

 github : https://github.com/bluesky466/cocos2dxUsefulClasses
 ****************************************************************************/

#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include "cocos2d.h"

enum JoystickEventType{
	JET_TOUCH_BEGIN,
	JET_TOUCH_MOVE,
	JET_TOUCH_END
};

class Joystick;

//interval是时间间隔,传入的x、y的范围都是0-1.0F,JoystickEventType是类型（开始,移动,结束）
typedef void (cocos2d::CCObject::*SEL_JoystickEvent)(Joystick*,float interval,float x, float y,JoystickEventType type);
#define joystickEvent_selector(_SELECTOR) (SEL_JoystickEvent)(&_SELECTOR)

///触碰在摇杆外部的时候的回调方法，如果该回调返回true,则以后将继续接收JET_TOUCH_BEGIN,JET_TOUCH_MOVE和JET_TOUCH_END消息
typedef bool (cocos2d::CCObject::*SEL_TouchOutsideHandleEvent)(Joystick*,const cocos2d::CCPoint&);
#define touchOutsideHandleEvent_selector(_SELECTOR) (SEL_TouchOutsideHandleEvent)(&_SELECTOR)

/**
 * @brief 一个虚拟摇杆类
 *
 * 操纵杆,背景和按钮都是是精灵类,它们的锚点一直都是ccp(0.5,0.5)
 * 可以用setHandleEventListener方法设置摇杆摇动事件的处理
 */
class Joystick : public cocos2d::CCNode,public cocos2d::CCTargetedTouchDelegate
{
public:
	Joystick():m_bMove(false){}
	virtual ~Joystick(){}

	/**
	 * 创建一个摇杆实例的静态方法
	 *@param fnBg 背景图片的文件名,用来创建摇杆的底盘精灵
	 *@param bgRadius 摇杆的底盘的半径
	 *@param fnHandle 摇杆图片的文件名,用来创建摇杆精灵
	 *@param handleRadius 摇杆的半径
	 */
	static Joystick* create(const char *fnBg,     float bgRadius,
					        const char *fnHandle, float handleRadius);

	bool init(const char *fnBg,     float bgRadius,
			  const char *fnHandle, float handleRadius);

	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	void onEnter();
	void onExit();

	///设置摇杆移动时要调用的方法，这个方法的声明为void f（float interval, float x, float y）,interval是时间间隔，传入的x、y的范围都是0-1.0F
	void setHandleEventListener(cocos2d::CCObject *target, SEL_JoystickEvent selector);

	///触碰在摇杆外部的时候的回调方法，如果该回调返回true,则以后将继续接收JET_TOUCH_BEGIN,JET_TOUCH_MOVE和JET_TOUCH_END消息
	void setTouchOutsideHandleEventListener(cocos2d::CCObject *target, SEL_TouchOutsideHandleEvent selector);

	///这个方法每一帧都被调用,如果设置了摇杆事件的处理的话他会调用哪个处理方法
	void callHandleEvent(float interval);
protected:
	cocos2d::CCSprite* m_bg;        ///<底盘的精灵
	cocos2d::CCSprite* m_handle;    ///<摇杆的精灵

	float m_bgRadius;               ///<底盘的半径
	float m_handleRadius;           ///<摇杆的半径
	bool  m_bMove;		            ///<摇杆是否正在移动

	cocos2d::CCPoint m_handlePos;   ///<摇杆在底盘坐标系的坐标
	
	cocos2d::CCObject*  m_touchEventListener;
    SEL_JoystickEvent   m_touchEventSelector;

	cocos2d::CCObject*  m_outsideEventListener;
    SEL_TouchOutsideHandleEvent  m_outsideEventSelector;

	void setHandlePosition(const cocos2d::CCPoint& position);
};

inline void Joystick::setHandleEventListener(cocos2d::CCObject *target, SEL_JoystickEvent selector)
{
	m_touchEventListener = target;
	m_touchEventSelector = selector;
}

inline void Joystick::setTouchOutsideHandleEventListener(cocos2d::CCObject *target, SEL_TouchOutsideHandleEvent selector)
{
	m_outsideEventListener = target;
	m_outsideEventSelector = selector;
}

#endif