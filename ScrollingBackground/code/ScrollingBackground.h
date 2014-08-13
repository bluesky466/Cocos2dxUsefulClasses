#ifndef __SCROLLING_BACKGROUND_H__
#define __SCROLLING_BACKGROUND_H__

#include "cocos2d.h"
USING_NS_CC;

typedef void (CCObject::*SEL_RemoveBgBlockEvent)();
#define removeBgBlockEvent_selector(_SELECTOR) (SEL_RemoveBgBlockEvent)(&_SELECTOR)

///背景滚动的方向
enum ScrollDirection{SD_RIGHT_TO_LEFT,SD_LEFT_TO_RIGHT,SD_TOP_TO_BOTTOM,SD_BOTTOM_TO_TOP};

///背景滚动的模式
enum ScrollType{
	///背景单向滚动，每个画面只出现一次，滚出屏幕后清除
	///可以调用setRemoveEventListener设置回调函数，在一个画面被清除后再调用addBackgroundBlock增加一个新的画面到列表尾部
	ST_ONE_WAY,
	
	///背景循环滚动，滚动到最后一个画面之后跳回第一个画面
	///需要在调用beginScroll之前调用addBackgroundBlock设置好至少一个画面
	ST_CYCLE};


///节点储存线性表（ST_ONE_WAY模式为单链表,ST_CYCLE模式为单向循环列表）
struct NodeList{
	CCNode* _pNode;
	NodeList* _next;

	NodeList():
		_pNode(0),
		_next(0)
	{}
};

/**
 *  滚动背景
 *  有单向滚动和循环滚动两种模式
 *  先用addBackgroundBlock增加画面,再用beginScroll开始滚动
 *  至少需要两个画面才能正确运行
 */
class ScrollingBackground : public CCNode
{
public:
	ScrollingBackground();
	virtual ~ScrollingBackground();

	///增加背景画面,传入的节点会自动铺满整个屏幕
	bool addBackgroundBlock(CCNode* bgBlock);

	///开始滚动
	bool beginScroll();

	///暂停滚动
	void pauseScroll()  {m_isScrolling = false;}

	///恢复滚动
	void resumeScroll() {m_isScrolling = true;}

	///（只有ST_ONE_WAY模式起作用）增加画面清除的时候的回调函数,当一个画面滚出屏幕之后会调用selector
	///可以在这时用addBackgroundBlock增加新的画面到列表尾部
	void setRemoveEventListener(CCObject *target, SEL_RemoveBgBlockEvent selector);

	/**
	 *  一个静态方法,用于创建ScrollingBackground
	 *@param type 滚动模式
	 *@param ScrollDirection 滚动方向 
	 *@param velocity 滚动的速度
	 */
	static ScrollingBackground* create(ScrollType type,ScrollDirection direct,float velocity);
protected:
	ScrollDirection m_direct;        ///<滚动方向
	ScrollType		m_type;			 ///<滚动模式
	NodeList*		m_blockList;     ///<节点储存线性表
	NodeList*		m_lastListNode;  ///<线性表表尾部
	NodeList*		m_thisListNode;  ///<当前出现在屏幕上的两个画面的前一个（先滚出屏幕的那个）
	CCSize			m_visibleSize;   ///<储存屏幕的大小,用于将各个画面缩放到铺满屏幕
	float           m_velocity;      ///<滚动的速度
	bool			m_isScrolling;   ///<是否正在滚动

	CCObject*              m_eventListener;
    SEL_RemoveBgBlockEvent m_eventSelector;

	bool init(ScrollType type,ScrollDirection direct,float velocity);

	///创建节点并将节点插入列表
	void addListNode(CCNode* bgBlock);

	///一个画面moveby完成后的回调函数
	void nodeOutEvent();

	///每一帧调用一次,设置画面的位置
	void moveAction(float d);

	///设置第二个画面的位置
	void setLastNodePosition();

	///判断一个画面是否不在屏幕范围内
	bool isOutOfScreen(const CCPoint& pos,const CCPoint& achorPoint);
};

inline void ScrollingBackground::setRemoveEventListener(CCObject *target, SEL_RemoveBgBlockEvent selector)
{
	m_eventListener = target;
	m_eventSelector = selector;
}

#endif