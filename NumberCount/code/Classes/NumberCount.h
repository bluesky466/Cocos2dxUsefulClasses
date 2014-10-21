#ifndef _NUMBER_COUNT_H_
#define _NUMBER_COUNT_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class NumberCount : public cocos2d::CCObject
{
public:
	NumberCount();

	void setLabelAtlas(cocos2d::CCLabelAtlas* ccLabelAtlas)   
	{
		m_ccLabelAtlas = ccLabelAtlas;
	}

	void setLabelAtlas(cocos2d::ui::LabelAtlas* uiLabelAtlas) 
	{
		m_ccLabelAtlas = (cocos2d::CCLabelAtlas*)uiLabelAtlas->getVirtualRenderer();
	}

	void setNumberDelta(int delta)     {m_numberDelta = abs(delta);}
	void setIncrement(int increment)   {m_increment   = increment;}
	void setTimeDelta(float timeDelta) {m_timeDelta   = timeDelta;}
	void setNumber(int number, bool bGradually);

	void updateNumber(float fInterval);
private:
	cocos2d::CCLabelAtlas* m_ccLabelAtlas;

	int   m_number;
	int   m_increment;
	int   m_numberDelta;
	float m_timeDelta;
	float m_timeEscape;
};

#endif