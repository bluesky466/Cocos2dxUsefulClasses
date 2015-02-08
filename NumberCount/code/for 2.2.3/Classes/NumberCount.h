/****************************************************************************
 for cocos2d-x 2.2.3

 author : LuoYu
 
 e-mail : 466474482@qq.com

 github : https://github.com/bluesky466/cocos2dxUsefulClasses
 ****************************************************************************/

#ifndef _NUMBER_COUNT_H_
#define _NUMBER_COUNT_H_

#include "cocos2d.h"
#include "cocos-ext.h"

///到达目标数字的回调
typedef void (cocos2d::CCObject::*SEL_ChangeFinish)(int numberTarget);
#define changeFinish_selector(_SELECTOR) (SEL_ChangeFinish)(&_SELECTOR)

/**
 * @brief 一个数字标签的增强功能类
 *
 * 功能：让数字标签按设定的增量一逐步增加
 *       数字会每隔m_timeDelta时间,增加m_numberDelta,直到到达目标数字m_numberTarget
 *
 * 应用范围：可以用于分数的显示或者倒计时
 */
class NumberCount : public cocos2d::CCObject
{
public:
	NumberCount();

	void setLabelAtlas(cocos2d::CCLabelAtlas* ccLabelAtlas)   
	{
		m_ccLabelAtlas = ccLabelAtlas;
		m_ccLabelAtlas->setString(cocos2d::CCString::createWithFormat("%d",m_numberCurrent)->getCString());
	}

	void setLabelAtlas(cocos2d::ui::LabelAtlas* uiLabelAtlas) 
	{
		m_uiLabelAtlas = uiLabelAtlas;
		m_uiLabelAtlas->setStringValue(cocos2d::CCString::createWithFormat("%d",m_numberCurrent)->getCString());
	}

	void setChangeFinishCallback(SEL_ChangeFinish selector,CCObject* target)
	{
		m_selector = selector;
		m_target = target;
	}

	///默认就是在计数的，如果觉得有必要，可以先关了它，到需要的时候在开始
	void begin();

	///默认就是在计数的，如果觉得有必要，可以先关了它，到需要的时候在开始
	void stop();
	
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
	
	cocos2d::CCLabelAtlas*    m_ccLabelAtlas;
	cocos2d::ui::LabelAtlas*  m_uiLabelAtlas;

	SEL_ChangeFinish m_selector;
	CCObject* m_target;
};

#endif