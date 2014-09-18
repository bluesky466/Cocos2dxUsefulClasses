#include "ScrollingBackground.h"

ScrollingBackground* ScrollingBackground::create(ScrollDirection direction)
{ 
    ScrollingBackground *pRet = new ScrollingBackground; 
    if (pRet && pRet->init(direction)) 
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
	m_isScrolling(false),
	m_bScrollToNext(false),
	m_bScrollBy(false)
{
	m_bgBlockList.clear();
}

ScrollingBackground::~ScrollingBackground()
{
	m_bgBlockList.clear();
}

bool ScrollingBackground::init(ScrollDirection direction)
{
	if(!CCNode::init())
		return false;

	m_direction   = direction;
	m_visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	this->schedule(schedule_selector(ScrollingBackground::moveAction));

	return true;
}

bool ScrollingBackground::beginScroll(float velocity)
{
	if(m_bgBlockList.size()<2)
		return false;

	m_followIter = m_curIter;
	m_followIter++;

	if(m_followIter == m_bgBlockList.end())
		m_followIter = m_bgBlockList.begin();

	m_isScrolling = true;
	m_velocity    = velocity;

	return true;
}

bool ScrollingBackground::scrollToNextBgBlock(float velocity)
{
	if(!beginScroll(velocity))
		return false;

	m_bScrollToNext = true;

	return true;
}

bool ScrollingBackground::setBgBlockScrollBy(float velocity,float distance)
{
	if(!beginScroll(velocity))
		return false;

	m_distance  = abs(distance);
	m_bScrollBy = true;

	return true;
}

void ScrollingBackground::preSetCurPos()
{
	if(m_velocity)
	{
		ListIter preIter = m_curIter;

		if(preIter == m_bgBlockList.begin())
			preIter = m_bgBlockList.end();
			
		preIter--;

		CCPoint aPointCur = (*m_curIter)->getAnchorPoint();
		CCPoint posCur  = (*m_curIter)->getPosition();

		CCPoint aPointPre = (*preIter)->getAnchorPoint();
		CCPoint posPre;

		switch(m_direction)
		{
		case SD_RIGHT_TO_LEFT:
			posPre.x = posCur.x - aPointCur.x * m_visibleSize.width - (1.0f - aPointPre.x) * m_visibleSize.width;
			posPre.y = aPointPre.y * m_visibleSize.height;
			break;

		case SD_LEFT_TO_RIGHT:
			posPre.x = posCur.x + (1.0f - aPointCur.x) * m_visibleSize.width + aPointPre.x * m_visibleSize.width;
			posPre.y = aPointPre.y * m_visibleSize.height;
			break;

		case SD_TOP_TO_BOTTOM:
			posPre.x = aPointPre.x * m_visibleSize.width;
			posPre.y = posCur.y - aPointCur.y * m_visibleSize.height - (1.0f - aPointPre.y) * m_visibleSize.height;
			break;

		case SD_BOTTOM_TO_TOP:
			posPre.x = aPointPre.x * m_visibleSize.width;
			posPre.y = posCur.y + (1.0f - aPointCur.y) * m_visibleSize.height + aPointPre.y * m_visibleSize.height;
			break;
		}

		(*preIter)->setVisible(true);
		(*preIter)->setPosition(posPre);
	}
}

bool ScrollingBackground::bChangeBg(const CCPoint& posCur,const CCPoint& achorPointCur)
{
	if(m_velocity>0.0f)
	{
		switch(m_direction)
		{
		case SD_RIGHT_TO_LEFT:
			return  (posCur.x < (-1.0f + achorPointCur.x) * m_visibleSize.width);

		case SD_LEFT_TO_RIGHT:
			return  (posCur.x > (1.0f + achorPointCur.x) * m_visibleSize.width);

		case SD_TOP_TO_BOTTOM:
			return  (posCur.y < (-1.0f + achorPointCur.y) * m_visibleSize.height);

		case SD_BOTTOM_TO_TOP:
			return  (posCur.y > (1.0f + achorPointCur.y) * m_visibleSize.height);

		default:
			return true;
		}
	}
	else
	{
		switch(m_direction)
		{
		case SD_RIGHT_TO_LEFT:
			return  (posCur.x > achorPointCur.x * m_visibleSize.width);
			
		case SD_LEFT_TO_RIGHT:
			return  (posCur.x < achorPointCur.x * m_visibleSize.width);

		case SD_TOP_TO_BOTTOM:
			return  (posCur.y > achorPointCur.y * m_visibleSize.height);

		case SD_BOTTOM_TO_TOP:
			return  (posCur.y < achorPointCur.y * m_visibleSize.height);

		default:
			return true;
		}
	}
}

void ScrollingBackground::moveAction(float d)
{
	if(!m_isScrolling)
		return;

	CCPoint posCur  = (*m_curIter)->getPosition();
	CCPoint aPosCur = (*m_curIter)->getAnchorPoint();

	if(bChangeBg(posCur,aPosCur))
		changeCurBgBlock();
	else
	{
		switch(m_direction)
		{
		case SD_RIGHT_TO_LEFT:
			posCur.x-=m_velocity*d;
			break;

		case SD_LEFT_TO_RIGHT:
			posCur.x+=m_velocity*d;
			break;

		case SD_TOP_TO_BOTTOM:
			posCur.y-=m_velocity*d;
			break;

		case SD_BOTTOM_TO_TOP:
			posCur.y+=m_velocity*d;
			break;
		}

		if(m_bScrollBy)
		{
			m_distance-=abs(m_velocity)*d;
			if(m_distance<0.0f)
			{
				m_isScrolling = false;
				m_bScrollBy   = false;
			}
		}

		(*m_curIter)->setPosition(posCur);
	}

	if(m_bgBlockList.size()>1)
		setFollowNodePosition();
}

bool ScrollingBackground::addBackgroundBlock(CCNode* bgBlock)
{
	if(m_bgBlockList.size()>1 && (*m_curIter) == m_bgBlockList.back())
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
	if(m_bgBlockList.empty())
	{
		//设置第一个画面
		CCPoint anchorPoint = bgBlock->getAnchorPoint();
		CCPoint pos(anchorPoint.x * m_visibleSize.width,
				    anchorPoint.y * m_visibleSize.height);

		bgBlock->setVisible(true);
		bgBlock->setPosition(pos);

		m_bgBlockList.push_back(bgBlock);
		m_curIter = m_bgBlockList.begin();
	}
	else
		m_bgBlockList.push_back(bgBlock);
}

void ScrollingBackground::changeCurBgBlock()
{
	CCNode* bgLeave;

	if(m_velocity>0.0f)
	{
		bgLeave = (*m_curIter);
		(*m_curIter)->setVisible(false);

		m_curIter++;
		if(m_curIter == m_bgBlockList.end())
			m_curIter = m_bgBlockList.begin();

		m_followIter = m_curIter;
		m_followIter++;
		if(m_followIter == m_bgBlockList.end())
			m_followIter = m_bgBlockList.begin();
	}
	else
	{
		bgLeave = (*m_followIter);
		(*m_followIter)->setVisible(false);

		preSetCurPos();
		m_followIter = m_curIter;

		if(m_curIter == m_bgBlockList.begin())
			m_curIter = m_bgBlockList.end();

		m_curIter--;
	}
	
	if(m_bScrollToNext)
	{
		m_isScrolling   = false;
		m_bScrollToNext = false;
		setFullScreen(m_velocity>0.0f?(*m_curIter):(*m_followIter));
	}

	if(m_eventListener)
	{
		CCNode* pCur    = m_curIter!=m_bgBlockList.end() ? (*m_curIter):0;
		CCNode* pFollow = m_followIter!=m_bgBlockList.end() ? (*m_followIter):0;
		(m_eventListener->*m_eventSelector)(pCur,pFollow,bgLeave);
	}
}

void ScrollingBackground::setFollowNodePosition()
{
	CCPoint aPointCur = (*m_curIter)->getAnchorPoint();
	CCPoint posCur    = (*m_curIter)->getPosition();

	CCPoint aPointFollow = (*m_followIter)->getAnchorPoint();
	CCPoint posFollow;

	switch(m_direction)
	{
	case SD_RIGHT_TO_LEFT:
		posFollow.x = posCur.x + (1.0f - aPointCur.x) * m_visibleSize.width + aPointFollow.x * m_visibleSize.width;
		posFollow.y = aPointFollow.y * m_visibleSize.height;
		break;

	case SD_LEFT_TO_RIGHT:
		posFollow.x = posCur.x - aPointCur.x * m_visibleSize.width - (1.0f - aPointFollow.x) * m_visibleSize.width;
		posFollow.y = aPointFollow.y * m_visibleSize.height;
		break;

	case SD_TOP_TO_BOTTOM:
		posFollow.x = aPointFollow.x * m_visibleSize.width;
		posFollow.y = posCur.y + (1.0f - aPointCur.y) * m_visibleSize.height + aPointFollow.y * m_visibleSize.height;
		break;

	case SD_BOTTOM_TO_TOP:
		posFollow.x = aPointFollow.x * m_visibleSize.width;
		posFollow.y = posCur.y - aPointCur.y * m_visibleSize.height - (1.0f - aPointFollow.y) * m_visibleSize.height;
		break;
	}

	(*m_followIter)->setVisible(true);
	(*m_followIter)->setPosition(posFollow);
}

void ScrollingBackground::setFullScreen(CCNode* bgBlock)
{
	CCPoint anchorPoint = bgBlock->getAnchorPoint();
	CCPoint pos(anchorPoint.x * m_visibleSize.width,
				anchorPoint.y * m_visibleSize.height);

	bgBlock->setVisible(true);
	bgBlock->setPosition(pos);
}

bool ScrollingBackground::removeBgBlock(CCNode* bgBlock)
{
	if(m_bgBlockList.size()>2 && bgBlock!=(*m_curIter) && bgBlock!=(*m_followIter))
	{
		m_bgBlockList.remove(bgBlock);
		this->removeChild(bgBlock);
		return true;
	}
	return false;
}