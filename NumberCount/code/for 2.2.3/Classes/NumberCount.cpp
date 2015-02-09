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
	m_selector(0),
	m_target(0)
{
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(NumberCount::updateNumber),this,0.0f,false);
}

NumberCount::~NumberCount()
{
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NumberCount::updateNumber),this);
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
		m_ccLabelAtlas->setString(CCString::createWithFormat("%d",m_numberCurrent)->getCString());

	if(m_uiLabelAtlas)
		m_uiLabelAtlas->setStringValue(CCString::createWithFormat("%d",m_numberCurrent)->getCString());
	
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
			m_ccLabelAtlas->setString(CCString::createWithFormat("%d",m_numberCurrent)->getCString());

		if(m_uiLabelAtlas)
			m_uiLabelAtlas->setStringValue(CCString::createWithFormat("%d",m_numberCurrent)->getCString());

		if(m_numberCurrent==m_numberTarget && m_selector && m_target)
			(m_target->*m_selector)(m_numberTarget);
	}
	else
		m_timeEscape += fInterval;
}

void NumberCount::begin()
{
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(NumberCount::updateNumber),this,0.0f,false);
}

void NumberCount::stop()
{
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NumberCount::updateNumber),this);
}