/****************************************************************************
 for cocos2d-x 3.2

 author : LuoYu
 
 e-mail : 466474482@qq.com

 github : https://github.com/bluesky466/cocos2dxUsefulClasses
 ****************************************************************************/

#ifndef _NUMBER_COUNT_H_
#define _NUMBER_COUNT_H_

#include "cocos2d.h"
#include "editor-support\cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"

///到达目标数字的回调
typedef std::function<void(int target)> ChangeFinishCallback;

/**
 * @brief 一个数字标签的增强功能类
 *
 * 功能：让数字标签按设定的增量一逐步增加
 *       数字会每隔m_timeDelta时间,增加m_numberDelta,直到到达目标数字m_numberTarget
 *
 * 应用范围：可以用于分数的显示或者倒计时
 */
class NumberCount : public cocos2d::Ref
{
public:
	NumberCount();
	~NumberCount();

	void setLabelAtlas(cocos2d::LabelAtlas* ccLabelAtlas)   
	{
		m_ccLabelAtlas = ccLabelAtlas;
		m_ccLabelAtlas->setString(cocos2d::__String::createWithFormat("%d",m_numberCurrent)->getCString());
	}

	void setTextAtlas(cocos2d::ui::TextAtlas* uiLabelAtlas) 
	{
		m_uiLabelAtlas = uiLabelAtlas;
		m_uiLabelAtlas->setString(cocos2d::__String::createWithFormat("%d",m_numberCurrent)->getCString());
	}

	void setChangeFinishCallback(ChangeFinishCallback callback)
	{
		m_finishCallback = callback;
	}

	///默认就是在计数的，如果觉得有必要，可以先关了它，到需要的时候在开始
	void begin()
	{
		cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(NumberCount::updateNumber),this,0.0f,false);
	}

	///默认就是在计数的，如果觉得有必要，可以先关了它，到需要的时候在开始
	void stop()
	{
		cocos2d::Director::getInstance()->getScheduler()->unschedule(schedule_selector(NumberCount::updateNumber),this);
	}
	
	///获得数字变化完成时最终显示的数字
	int getNumberTarget() {return m_numberTarget;}

	///获得当前显示的数字
	int getNumberCurrent(){return m_numberCurrent;}

	/**
	 * 使数字增加一定值
	 */
	void add(int increment)   {m_numberTarget += increment;}

	/**
	 * 设置数字每一步增加的增量的绝对值
	 * 数字会每隔m_timeDelta时间,增加m_numberDelta,直到到达m_numberTarget
	 */
	void setNumberDelta(int delta)     {m_numberDelta = abs(delta);}
	
	/**
	 * 设置数字每一步增加的时间间隔m_timeDelta
	 * 数字会每隔m_timeDelta时间,增加或减少m_numberDelta,直到到达m_numberTarget
	 */
	void setTimeDelta(float timeDelta) {m_timeDelta = timeDelta;}

	/**
	 * 设置数字
	 *@param number 目标数字
	 *@param bGradually 是不是由当前数字一步一步增加到目标数字
	 */
	bool setNumber(int number, bool bGradually);

	///schedule方法,数字增加的计算就在这里实现
	void updateNumber(float fInterval);

private:
	int   m_numberTarget;  ///<数字变化的目标值
	int   m_numberCurrent; ///<数字当前的值
	int   m_numberDelta;   ///<每一步的增量
	float m_timeDelta;     ///<多久改变一次数字
	float m_timeEscape;
	
	cocos2d::LabelAtlas*    m_ccLabelAtlas;
	cocos2d::ui::TextAtlas* m_uiLabelAtlas;

	ChangeFinishCallback m_finishCallback;
};

#endif