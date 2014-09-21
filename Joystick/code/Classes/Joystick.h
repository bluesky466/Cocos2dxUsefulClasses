#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "cocos2d.h"
USING_NS_CC;

enum JoystickEventType{JET_TOUCH_BEGIN,JET_TOUCH_MOVE,JET_TOUCH_END};

//interval是时间间隔,传入的x、y的范围都是0-1.0F,JoystickEventType是类型（开始,移动,结束）
typedef void (CCObject::*SEL_JoystickEvent)(float interval,float x, float y,JoystickEventType type);
#define joystickEvent_selector(_SELECTOR) (SEL_JoystickEvent)(&_SELECTOR)

/**
 *  操纵杆,背景和按钮都是是精灵类,它们的锚点一直都是ccp(0.5,0.5)
 *  可以用setHandleEventListener方法设置摇杆摇动事件的处理
 */
class Joystick : public CCNode,public CCTargetedTouchDelegate
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

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	void onEnter();
	void onExit();

	///设置摇杆移动时要调用的方法，这个方法的声明为void f（float interval, float x, float y）,interval是时间间隔，传入的x、y的范围都是0-1.0F
	void setHandleEventListener(CCObject *target, SEL_JoystickEvent selector);

	///这个方法每一帧都被调用,如果设置了摇杆事件的处理的话他会调用哪个处理方法
	void callHandleEvent(float interval);
protected:
	CCSprite* m_bg;        ///<底盘的精灵
	CCSprite* m_handle;    ///<摇杆的精灵

	float m_bgRadius;      ///<底盘的半径
	float m_handleRadius;  ///<摇杆的半径
	bool  m_bMove;		   ///<摇杆是否正在移动

	CCPoint m_handlePos;   ///<摇杆在底盘坐标系的坐标

	CCObject*         m_touchEventListener;
    SEL_JoystickEvent m_touchEventSelector;

	bool init(const char *fnBg,     float bgRadius,
			  const char *fnHandle, float handleRadius);
	
};

inline void Joystick::setHandleEventListener(CCObject *target, SEL_JoystickEvent selector)
{
	m_touchEventListener = target;
	m_touchEventSelector = selector;
}

#endif