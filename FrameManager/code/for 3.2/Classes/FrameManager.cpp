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
}

bool FrameManager::setFrame(Widget* frameLayer,Widget* frameBg)
{
	m_frameLayer    = frameLayer;
	m_frameBg       = frameBg;
	m_frameSize     = m_frameBg->getContentSize();
	m_anchorPoint.x = m_frameSize.width  * m_frameBg->getAnchorPoint().x;
	m_anchorPoint.y = m_frameSize.height * m_frameBg->getAnchorPoint().y;
	m_bFrameVisible = true;

	m_touchListener = EventListenerTouchOneByOne::create();
	m_touchListener->onTouchBegan = CC_CALLBACK_2(FrameManager::onTouchBegan,this);
	m_touchListener->onTouchEnded = CC_CALLBACK_2(FrameManager::onTouchEnded,this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_touchListener,m_frameLayer);

	return true;
}

void FrameManager::setFrameVisible(bool bVisible)
{
	CCAssert(m_frameLayer!=0 && m_frameBg!=0,"m_frameBg or m_frameBg is null");

	if(m_frameLayer && m_frameBg)
	{
		m_bFrameVisible = bVisible;
		m_frameLayer->setVisible(m_bFrameVisible);
	}
}

bool FrameManager::isOutsideFrame(Touch *pTouch)
{
	CCAssert(m_frameBg,"m_frameBg is null");

	if(!m_frameBg)
		return false;

	Point point = m_frameBg->convertTouchToNodeSpaceAR(pTouch);
	point = point + m_anchorPoint;

	if(point.x<0.0f || point.y<0.0f ||
	   point.x>m_frameSize.width || point.y>m_frameSize.height)
	{
		return true;
	}
	
	return false;
}

void FrameManager::onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
	if(m_bFrameVisible && isOutsideFrame(pTouch))
	{
		if(m_eventSelector && m_eventTarget)
			(m_eventTarget->*m_eventSelector)(m_frameLayer);

		if(m_eventCallback)
			m_eventCallback(m_frameLayer);
	}
}

bool FrameManager::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	if(m_bFrameVisible)
		return true;

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
