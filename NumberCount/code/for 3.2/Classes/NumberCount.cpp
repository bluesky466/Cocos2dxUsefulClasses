#include "NumberCount.h"

USING_NS_CC;
using namespace ui;

NumberCount::NumberCount():
	m_numberTarget(0),
	m_numberCurrent(0),
	m_numberDelta(1),
	m_timeDelta(0.01f),
	m_timeEscape(0.0f),
	m_ccLabelAtlas(0),
	m_uiLabelAtlas(0),
	m_finishCallback(nullptr)
{
	Director::getInstance()->getScheduler()->schedule(schedule_selector(NumberCount::updateNumber),this,0.0f,false);
}

bool NumberCount::setNumber(int number, bool bGradually)
{
	if(!bGradually)
	{
		m_numberTarget  = number;
		m_numberCurrent = number;
	}
	else
	{
		m_numberTarget = number;
		m_timeEscape   = 0.0f;
	}

	if(m_ccLabelAtlas)
		m_ccLabelAtlas->setString(__String::createWithFormat("%d",m_numberCurrent)->getCString());

	if(m_uiLabelAtlas)
		m_uiLabelAtlas->setString(__String::createWithFormat("%d",m_numberCurrent)->getCString());
	
	return true;
}

void NumberCount::updateNumber(float fInterval)
{
	if(m_numberCurrent == m_numberTarget)
		return;

	if(m_timeEscape > m_timeDelta)
	{
		if(m_numberTarget > m_numberCurrent)
		{
			m_numberCurrent += m_numberDelta;

			if(m_numberCurrent>m_numberTarget)
				m_numberCurrent = m_numberTarget;
		}
		else
		{
			m_numberCurrent -= m_numberDelta;

			if(m_numberCurrent < m_numberTarget)
				m_numberCurrent = m_numberTarget;
		}

		m_timeEscape = 0.0f;

		if(m_ccLabelAtlas)
			m_ccLabelAtlas->setString(__String::createWithFormat("%d",m_numberCurrent)->getCString());

		if(m_uiLabelAtlas)
			m_uiLabelAtlas->setString(__String::createWithFormat("%d",m_numberCurrent)->getCString());

		if(m_numberCurrent==m_numberTarget && m_finishCallback)
			m_finishCallback(m_numberTarget);
	}
	else
		m_timeEscape += fInterval;
}