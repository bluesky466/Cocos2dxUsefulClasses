#ifndef __SCROLLING_BACKGROUND_H__
#define __SCROLLING_BACKGROUND_H__

#include "cocos2d.h"
#include <list>
USING_NS_CC;

typedef std::list<CCNode*> NodeList;
typedef std::list<CCNode*>::iterator ListIter;

typedef void (CCObject::*SEL_ChangeBgBlockEvent)(CCNode* cur,CCNode* follow,CCNode* leave);
#define removeBgBlockEvent_selector(_SELECTOR) (SEL_ChangeBgBlockEvent)(&_SELECTOR)

///背景滚动的方向
enum ScrollDirection{SD_RIGHT_TO_LEFT,SD_LEFT_TO_RIGHT,SD_TOP_TO_BOTTOM,SD_BOTTOM_TO_TOP};

/**
 *  滚动背景
 *  先用addBackgroundBlock增加画面,再用开始滚动
 *  至少需要两个画面才能正确运行
 */
class ScrollingBackground : public CCNode
{
public:
	ScrollingBackground();
	virtual ~ScrollingBackground();

	/**
	 * 增加背景画面,传入的节点会自动铺满整个屏幕,并加到链表尾
	 * 可以一边滚动一边增加背景画面节点,如果当前节点是链表尾,会失败并返回false
	 */
	bool addBackgroundBlock(CCNode* bgBlock);

	///开始连续滚动
	bool beginContinuousScroll(float velocity)    {return beginScroll(velocity);}

	///背景滚动到两个背景节点中的一个彻底离开屏幕
	bool scrollToNextBgBlock(float velocity);

	///设置背景移动设定的距离
	bool setBgBlockScrollBy(float velocity,float distance);

	///暂停滚动
	void pauseScroll()  {m_isScrolling = false;}

	///恢复滚动
	void resumeScroll(float velocity);

	///移除背景节点,但必须保证节点链表内的节点数大于2,且不能移除当前显示在屏幕内的两个节点
	bool removeBgBlock(CCNode* bgBlock);

	///设置背景全屏完整显示
	void setFullScreen(CCNode* bgBlock);

	void setVelocity(float velocity)   {m_velocity = velocity;}

	bool isListBack(CCNode* bgBlock)   {return bgBlock == m_bgBlockList.back();}
	bool isListFront(CCNode* bgBlock)  {return bgBlock == m_bgBlockList.front();}
	
	CCNode* getBackBgBlock()		   {return m_bgBlockList.back();}
	CCNode* getFrontBgBlock()		   {return m_bgBlockList.front();}
	CCNode* getCurBgBlock()			   {return m_curIter!=m_bgBlockList.end() ? (*m_curIter):0;}
	CCNode* getFollowBgBlock()		   {return m_followIter!=m_bgBlockList.end() ? (*m_followIter):0;}
	
	/**
	 * 当一个画面彻底离开屏幕的时候会发送这个消息
	 * SEL_ChangeBgBlockEvent 的 CCNode 参数传的是屏幕内的两个地图节点
	 */
	void setChangeBgEventListener(CCObject *target, SEL_ChangeBgBlockEvent selector);

	/**
	 *  一个静态方法,用于创建ScrollingBackground
	 *@param type 滚动模式
	 *@param ScrollDirection 滚动方向 
	 */
	static ScrollingBackground* create(ScrollDirection direction);
protected:
	ScrollDirection m_direction;     ///<滚动方向
	NodeList		m_bgBlockList;   ///<节点储存链表
	ListIter		m_curIter;       ///<当前出现在屏幕上的两个画面的前一个（正方向先滚出屏幕的那个）
	ListIter		m_followIter;    ///<当前出现在屏幕上的两个画面的后一个，跟随画面（正方向后滚出屏幕的那个）
	CCSize			m_visibleSize;   ///<储存屏幕的大小,用于将各个画面缩放到铺满屏幕
	bool			m_isScrolling;   ///<是否正在滚动
	bool		    m_bScrollToNext; ///<是否设置了背景滚动到两个背景节点中的一个彻底离开屏幕
	bool			m_bScrollBy;     ///<是否设定背景滚动一定距离
	float           m_velocity;      ///<滚动的速度
	float			m_distance;      ///<设定背景块的移动距离
	CCObject*              m_eventListener;
    SEL_ChangeBgBlockEvent m_eventSelector;

	bool init(ScrollDirection direct);

	///创建节点并将节点插入列表
	void addListNode(CCNode* bgBlock);

	///一个画面moveby完成后的回调函数
	void changeCurBgBlock();

	///每一帧调用一次,设置画面的位置
	void moveAction(float d);

	///设置跟随画面的位置
	void setFollowNodePosition();

	///判断一个画面是否不在屏幕范围内
	bool bChangeBg(const CCPoint& posCur,const CCPoint& achorPointCur);

	///在速度为负的时候,要先设置下一时刻 m_curIter 所指向的背景节点的位置
	void preSetCurPos();

	///滚动的时候的公共设置
	bool beginScroll(float velocity);
};

inline void ScrollingBackground::setChangeBgEventListener(CCObject *target, SEL_ChangeBgBlockEvent selector)
{
	m_eventListener = target;
	m_eventSelector = selector;
}

inline void ScrollingBackground::resumeScroll(float velocity) 
{
	m_velocity = velocity;
	m_isScrolling = true;
}

#endif