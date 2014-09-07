#include "Frame.h"

/*
     Frame的方法实现
*/
bool Frame::init(UILayer* frameLayer,CCNode* frameBg)
{
	m_frameLayer    = frameLayer;
	m_frameBg       = frameBg;
	m_frameSize     = m_frameBg->getContentSize();
	m_anchorPoint.x = m_frameSize.width  * m_frameBg->getAnchorPoint().x;
	m_anchorPoint.y = m_frameSize.height * m_frameBg->getAnchorPoint().y;
	m_controls.clear();
	m_bFrameVisible = true;

	setControls();

	return true;
}

Frame::Frame():
	m_bFrameVisible(true),
	m_frameLayer(0),	
	m_frameBg(0)
{
	m_controls.clear();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,kCCPriorityNonSystemMin, true);
}

Frame::~Frame()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool Frame::isOutsideFrame(CCTouch *pTouch)
{
	CCPoint point = m_frameBg->convertTouchToNodeSpaceAR(pTouch);
	point = point + m_anchorPoint;

	if(point.x<0.0f || point.y<0.0f ||
	   point.x>m_frameSize.width || point.y>m_frameSize.height)
	{
		return true;
	}
	
	return false;
}

bool Frame::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(m_frameBg && m_bFrameVisible && isOutsideFrame(pTouch))
		setFrameVisble(false);

	return false;
}

void Frame::setFrameVisble(bool bVisible)
{
	CCAssert(m_frameLayer,"");

	m_bFrameVisible = bVisible;
	m_frameLayer->setVisible(bVisible);
	std::vector<Widget*>::iterator i = m_controls.begin();
	std::vector<Widget*>::iterator end = m_controls.end();
	
	while(i!=end)
	{
		(*i)->setTouchEnabled(bVisible);
		i++;
	}
}

/*
     RegisterFrame的方法实现
*/
bool RegisterFrame::init(UILayer* frameLayer,CCNode* frameBg)
{
	if(!Frame::init(frameLayer,frameBg))
		return false;

	return true;
}

void RegisterFrame::setControls()
{
	m_controls.push_back(m_frameLayer->getWidgetByName("login_Button"));
	m_controls.push_back(m_frameLayer->getWidgetByName("close_Button"));
	m_controls.push_back(m_frameLayer->getWidgetByName("agree_CheckBox"));
	m_controls.push_back(m_frameLayer->getWidgetByName("name_TextField"));
	m_controls.push_back(m_frameLayer->getWidgetByName("password_TextField"));
	m_controls.push_back(m_frameLayer->getWidgetByName("confirm_TextField"));
}