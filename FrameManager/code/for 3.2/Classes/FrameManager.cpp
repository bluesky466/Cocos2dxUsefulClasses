#include "FrameManager.h"

USING_NS_CC;
using namespace ui;

FrameManager::FrameManager():
	m_bFrameVisible(true),
	m_frameLayer(0),	
	m_frameBg(0),
	m_eventTarget(0),
	m_eventSelector(0)
{
	m_touchListener = EventListenerTouchOneByOne::create();
	m_touchListener->onTouchBegan = CC_CALLBACK_2(FrameManager::onTouchBegan,this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(m_touchListener,-99999);
}

bool FrameManager::setFrame(Widget* frameLayer,Widget* frameBg)
{
	m_frameLayer    = frameLayer;
	m_frameBg       = frameBg;
	m_frameSize     = m_frameBg->getContentSize();
	m_anchorPoint.x = m_frameSize.width  * m_frameBg->getAnchorPoint().x;
	m_anchorPoint.y = m_frameSize.height * m_frameBg->getAnchorPoint().y;
	m_bFrameVisible = true;

	return true;
}

void FrameManager::setFrameVisible(bool bVisible)
{
	if(m_frameLayer && m_frameBg)
	{
		m_bFrameVisible = bVisible;
		m_frameLayer->setVisible(m_bFrameVisible);
		m_frameLayer->setTouchEnabled(m_bFrameVisible);
		setChildrenTouchEnabled(m_frameLayer,m_bFrameVisible);
	}
}

bool FrameManager::isOutsideFrame(Touch *pTouch)
{
	Point point = m_frameBg->convertTouchToNodeSpaceAR(pTouch);
	point = point + m_anchorPoint;

	if(point.x<0.0f || point.y<0.0f ||
	   point.x>m_frameSize.width || point.y>m_frameSize.height)
	{
		return true;
	}
	
	return false;
}

bool FrameManager::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	if(m_frameBg && m_bFrameVisible && isOutsideFrame(pTouch))
	{
		m_bFrameVisible = false;
		m_frameLayer->setVisible(false);
		m_frameLayer->setTouchEnabled(false);
		setChildrenTouchEnabled(m_frameLayer,false);

		if(m_eventSelector && m_eventTarget)
			(m_eventTarget->*m_eventSelector)(m_frameLayer);

		if(m_eventCallback)
			m_eventCallback(m_frameLayer);
	}

	return false;
}

void FrameManager::setChildrenTouchEnabled(Node* pNode, bool bTouchEnabled)
{
	
	Vector<Node*> pChildren =  pNode->getChildren();
	auto i = pChildren.begin();
	auto end = pChildren.cend();
	while (i!=end)
	{
		((Layout*)(*i))->setTouchEnabled(bTouchEnabled);
		setChildrenTouchEnabled((Node*)(*i),bTouchEnabled);
		i++;
	}
}