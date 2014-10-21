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

/**
 * @brief 一个数字标签的增强功能类
 *
 * 功能：让数字标签按设定的增量一逐步增加
 *       数字会每隔m_timeDelta时间,增加m_numberDelta,直到总共增加m_increment
 *
 * 应用范围：可以用于分数的显示
 * 可以管理 cocos2d 的 cocos2d::LabelAtlas 和
 * 从 cocostudio 导出的 cocos2d::ui::TextAtlas
 */
class NumberCount : public cocos2d::Ref
{
public:
	NumberCount();

	void setLabelAtlas(cocos2d::LabelAtlas* ccLabelAtlas)   
	{
		m_ccLabelAtlas = ccLabelAtlas;
		m_ccLabelAtlas->setString(cocos2d::__String::createWithFormat("%d",m_number)->getCString());
	}

	void setTextAtlas(cocos2d::ui::TextAtlas* uiLabelAtlas) 
	{
		m_uiLabelAtlas = uiLabelAtlas;
		m_uiLabelAtlas->setString(cocos2d::__String::createWithFormat("%d",m_number)->getCString());
	}

	int getNumber(){return m_number;}

	/**
	 * 设置数字的增量m_increment
	 * 数字会每隔m_timeDelta时间,增加m_numberDelta,直到总共增加m_increment
	 */
	void setIncrement(int increment)   {m_increment   = increment;}

	/**
	 * 增加数字的增量m_increment（在原来的m_increment上再加increment）
	 * 数字会每隔m_timeDelta时间,增加m_numberDelta,直到总共增加m_increment
	 */
	void addIncrement(int increment)   {m_increment += increment;}

	/**
	 * 设置数字每一步增加的增量的绝对值m_numberDelta
	 * 数字会每隔m_timeDelta时间,增加m_numberDelta,直到总共增加m_increment
	 */
	void setNumberDelta(int delta)     {m_numberDelta = abs(delta);}
	
	/**
	 * 设置数字每一步增加的时间间隔m_timeDelta
	 * 数字会每隔m_timeDelta时间,增加m_numberDelta,直到总共增加m_increment
	 */
	void setTimeDelta(float timeDelta) {m_timeDelta   = timeDelta;}

	/**
	 * 设置数字
	 *@param number 目标数字
	 *@param bGradually 是不是由当前数字一步一步增加到目标数字
	 */
	bool setNumber(int number, bool bGradually);

	///schedule方法,数字增加的计算就在这里实现
	void updateNumber(float fInterval);

private:
	cocos2d::LabelAtlas*    m_ccLabelAtlas;
	cocos2d::ui::TextAtlas* m_uiLabelAtlas;

	int   m_number;
	int   m_increment;
	int   m_numberDelta;
	float m_timeDelta;
	float m_timeEscape;
};

#endif