/****************************************************************************
 for cocos2d-x 3.2

 author : LuoYu
 
 e-mail : 466474482@qq.com

 github : https://github.com/bluesky466/cocos2dxUsefulClasses
 ****************************************************************************/

#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include "cocos2d.h"

enum class JoystickEventType{
	JET_TOUCH_BEGIN,
	JET_TOUCH_MOVE,
	JET_TOUCH_END
};

class Joystick;

//interval是时间间隔,传入的x、y的范围都是0-1.0F,JoystickEventType是类型（开始,移动,结束）
typedef std::function<void(Joystick*,float interval,float x, float y,JoystickEventType type)> JoystickEventCallback;
#define JOYSTICK_CALLBACK(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, ##__VA_ARGS__)

///触碰在摇杆外部的时候的回调方法，如果该回调返回true,则以后将继续接收JET_TOUCH_BEGIN,JET_TOUCH_MOVE和JET_TOUCH_END消息
typedef std::function<bool(Joystick*,const cocos2d::Vec2&)> TouchOutsideHandleEvent;

/**
 * @brief 一个虚拟摇杆类
 *
 * 操纵杆,背景和按钮都是是精灵类,它们的锚点一直都是ccp(0.5,0.5)
 * 可以用setHandleEventListener方法设置摇杆摇动事件的处理
 */
class Joystick : public cocos2d::Node
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

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

	///设置摇杆移动时要调用的方法，这个方法的声明为void f（float interval, float x, float y）,interval是时间间隔，传入的x、y的范围都是0-1.0F
	void setHandleEventListener(const JoystickEventCallback& callback);

	///设置触碰在摇杆外部的时候的回调方法，如果该回调返回true,则以后将继续接收JET_TOUCH_BEGIN,JET_TOUCH_MOVE和JET_TOUCH_END消息
	void setTouchOutsideCallback(const TouchOutsideHandleEvent& callback);

	///这个方法每一帧都被调用,如果设置了摇杆事件的处理的话他会调用哪个处理方法
	void callHandleEvent(float interval);
protected:
	cocos2d::Sprite* m_bg;         ///<底盘的精灵
	cocos2d::Sprite* m_handle;     ///<摇杆的精灵

	float m_bgRadius;              ///<底盘的半径
	float m_handleRadius;          ///<摇杆的半径
	bool  m_bMove;		           ///<摇杆是否正在移动

	cocos2d::Point m_handlePos;    ///<摇杆在底盘坐标系的坐标
	
	JoystickEventCallback m_touchEventCallback;
	TouchOutsideHandleEvent m_touchOutsideCallback;
	
	void setHandlePosition(const cocos2d::Vec2 position);
};

inline void Joystick::setHandleEventListener(const JoystickEventCallback& callback)
{
	m_touchEventCallback = callback;
}

inline void Joystick::setTouchOutsideCallback(const TouchOutsideHandleEvent& callback)
{
	m_touchOutsideCallback = callback;
}

#endif