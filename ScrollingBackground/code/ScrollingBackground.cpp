#include "ScrollingBackground.h"

ScrollingBackground* ScrollingBackground::create(ScrollType type,ScrollDirection direct,float velocity)
{ 
    ScrollingBackground *pRet = new ScrollingBackground(); 
    if (pRet && pRet->init(type,direct,velocity)) 
    { 
        pRet->autorelease(); 
        return pRet; 
    } 
    else 
    { 
        delete pRet; 
        pRet = NULL; 
        return NULL; 
    } 
}

ScrollingBackground::ScrollingBackground():
	m_eventListener(0),
	m_eventSelector(0),
	m_isScrolling(false)
{

}

ScrollingBackground::~ScrollingBackground()
{
	if(m_type == ST_CYCLE)
	{
		NodeList* fNode = m_blockList;
		NodeList* lNode = fNode->_next;

		while(lNode!=m_blockList)
		{
			CC_SAFE_DELETE(fNode);

			fNode = lNode;
			lNode = lNode->_next;
		}

		CC_SAFE_DELETE(fNode);
	}
	else
	{
		NodeList* pNode = m_blockList;

		while(pNode)
		{
			NodeList* p = pNode;
			pNode = pNode->_next;

			CC_SAFE_DELETE(p);
		}
	}
}

bool ScrollingBackground::init(ScrollType type,ScrollDirection direct,float velocity)
{
	if(!CCNode::init())
		return false;

	m_direct   = direct;
	m_velocity = velocity;
	m_type	   = type;

	m_visibleSize       = CCDirector::sharedDirector()->getVisibleSize();
	m_blockList         = new NodeList;
	m_blockList->_pNode = 0;

	if(type==ST_CYCLE)
		m_blockList->_next  = m_blockList;
	else
		m_blockList->_next  = 0;

	m_lastListNode = m_blockList;
	m_thisListNode = m_blockList;

	return true;
}

bool ScrollingBackground::beginScroll()
{
	CCAssert(m_blockList->_pNode,"");

	//设置第一个画面
	CCNode* fNode = m_blockList->_pNode;
	
	CCPoint fAnchorPoint = fNode->getAnchorPoint();
	CCPoint fPos(fAnchorPoint.x*m_visibleSize.width,
				 fAnchorPoint.y*m_visibleSize.height);

	fNode->setVisible(true);
	fNode->setPosition(fPos);

	m_thisListNode = m_blockList;

	m_isScrolling = true;

	this->schedule(schedule_selector(ScrollingBackground::moveAction));

	return true;
}

bool ScrollingBackground::isOutOfScreen(const CCPoint& pos,const CCPoint& achorPoint)
{
	switch(m_direct)
	{
	case SD_RIGHT_TO_LEFT:
		return  (pos.x < (-1.0f + achorPoint.x) * m_visibleSize.width);

	case SD_LEFT_TO_RIGHT:
		return  (pos.x > (1.0f + achorPoint.x) * m_visibleSize.width);


	case SD_TOP_TO_BOTTOM:
		return  (pos.y < (-1.0f + achorPoint.y) * m_visibleSize.height);


	case SD_BOTTOM_TO_TOP:
		return  (pos.y > (1.0f + achorPoint.y) * m_visibleSize.height);

	default:
		return true;
	}
}

void ScrollingBackground::moveAction(float d)
{
	if(!m_isScrolling || !m_thisListNode)
		return;

	NodeList* pF = m_thisListNode;
	NodeList* pL = m_thisListNode->_next;

	CCPoint posF  = pF->_pNode->getPosition();
	CCPoint aPosF = pF->_pNode->getAnchorPoint();

	if(isOutOfScreen(posF,aPosF))
		nodeOutEvent();
	else
	{
		switch(m_direct)
		{
		case SD_RIGHT_TO_LEFT:
			posF.x-=m_velocity*d;
			break;

		case SD_LEFT_TO_RIGHT:
			posF.x+=m_velocity*d;
			break;

		case SD_TOP_TO_BOTTOM:
			posF.y-=m_velocity*d;
			break;

		case SD_BOTTOM_TO_TOP:
			posF.y+=m_velocity*d;
			break;
		}
		
		pF->_pNode->setPosition(posF);
	}

	if(m_thisListNode)
		setLastNodePosition();
}

bool ScrollingBackground::addBackgroundBlock(CCNode* bgBlock)
{
	if(!m_blockList)
		return false;

	CCSize size = bgBlock->getContentSize();

	bgBlock->setScaleX(m_visibleSize.width  / size.width);
	bgBlock->setScaleY(m_visibleSize.height / size.height);

	bgBlock->setVisible(false);

	this->addChild(bgBlock);

	addListNode(bgBlock);

	return true;
}

void ScrollingBackground::addListNode(CCNode* bgBlock)
{
	if(m_blockList->_pNode !=0)
	{
		NodeList* newNode = new NodeList;
		newNode->_pNode   = bgBlock;

		if(m_type ==ST_CYCLE)
			newNode->_next    = m_blockList;
		else
			newNode->_next = 0;
		
		m_lastListNode->_next = newNode;
		m_lastListNode		  = newNode;
	}
	else
	{
		m_blockList->_pNode = bgBlock;
	}
}

void ScrollingBackground::nodeOutEvent()
{
	m_thisListNode = m_thisListNode->_next;

	if(m_type == ST_ONE_WAY)
	{
		this->removeChild(m_blockList->_pNode);
		CC_SAFE_DELETE(m_blockList);

		m_blockList = m_thisListNode;

		if(m_eventListener && m_eventSelector)
			(m_eventListener->*m_eventSelector)();
	}
}

void ScrollingBackground::setLastNodePosition()
{
	CCPoint posF = m_thisListNode->_pNode->getPosition();
	CCPoint aPosF = m_thisListNode->_pNode->getAnchorPoint();

	if(m_thisListNode->_next==0 || m_thisListNode->_next==m_thisListNode)
		return;

	CCNode* pNodeL = m_thisListNode->_next->_pNode;
	CCPoint aPosL  = pNodeL->getAnchorPoint();
	CCPoint pos;

	switch(m_direct)
	{
	case SD_RIGHT_TO_LEFT:
		pos.x = posF.x + (1.0f - aPosF.x) * m_visibleSize.width + aPosL.x * m_visibleSize.width;
		pos.y = aPosL.y * m_visibleSize.height;
		break;

	case SD_LEFT_TO_RIGHT:
		pos.x = posF.x - aPosF.x * m_visibleSize.width - (1.0f - aPosL.x) * m_visibleSize.width;
		pos.y = aPosL.y * m_visibleSize.height;
		break;

	case SD_TOP_TO_BOTTOM:
		pos.x = aPosL.x * m_visibleSize.width;
		pos.y = posF.y + (1.0f - aPosF.y) * m_visibleSize.height + aPosL.y * m_visibleSize.height;
		break;

	case SD_BOTTOM_TO_TOP:
		pos.x = aPosL.x * m_visibleSize.width;
		pos.y = posF.y - aPosF.y * m_visibleSize.height - (1.0f - aPosL.y) * m_visibleSize.height;
		break;
	}

	pNodeL->setVisible(true);
	pNodeL->setPosition(pos);
}

