/****************************************************************************
 for cocos2d-x 3.2

 author : LuoYu
 
 e-mail : 466474482@qq.com

 github : https://github.com/bluesky466/cocos2dxUsefulClasses
 ****************************************************************************/

#ifndef _FRAME_MANAGER_H_
#define _FRAME_MANAGER_H_

#include "cocos2d.h"
#include "editor-support\cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"

//当触点在对话框外的时候的回调函数
typedef void (cocos2d::Ref::*SEL_TouchOutOfFrameEvent)(cocos2d::ui::Widget*);
#define TouchOutOfFrameEvent_selector(_SELECTOR) (SEL_TouchOutOfFrameEvent)(&_SELECTOR)

/**
 *  对话框管理类
 *  功能:管理从cocostudio导出的对话框UI层容器
 *       如果鼠标点击位置在对话框之外，自动隐藏对话框，并使得内部控件不可触碰
 */
class FrameManager : public cocos2d::Ref
{
public:
	FrameManager();
	~FrameManager()
	{
		cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(m_touchListener);
	}

	/**
	 * 传入对话框的跟节点和背景节点
	 *@param frameLayer 对话框层容器
	 *@param frameBg 对话框的背景节点,用它来判断触点是否在对话框外
	 */
	bool setFrame(cocos2d::ui::Widget* frameLayer,cocos2d::ui::Widget* frameBg);

	void setFrameVisible(bool bVisible);

	bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

	cocos2d::ui::Widget* getFrameLayer() {return m_frameLayer;}

	void setTouchOutOfFrameEventListener(cocos2d::Ref* target,SEL_TouchOutOfFrameEvent selector)
	{
		m_eventTarget   = target;
		m_eventSelector = selector;
	}

	void setTouchOutOfFrameEventListener(std::function<void(cocos2d::ui::Widget*)> callback)
	{
		m_eventCallback = callback;
	}

	///递归设置子控件是否可触碰,3.2版本的似乎当根对话框不可见之后子控件就自动不可触碰了，并不需要调用这个方法
	void setChildrenTouchEnabled(bool bTouchEnabled)
	{
		setChildrenTouchEnabled(m_frameLayer,bTouchEnabled);
	}


protected:
	cocos2d::ui::Widget*  m_frameLayer;
	cocos2d::ui::Widget*  m_frameBg;
	cocos2d::Size   m_frameSize;
	cocos2d::Point  m_anchorPoint;  ///<锚点在背景坐标系的坐标,坐标范围是(0.0f,0.0f)到m_frameSize
	bool m_bFrameVisible;

	cocos2d::EventListenerTouchOneByOne* m_touchListener;
	std::function<void(cocos2d::ui::Widget*)> m_eventCallback;

	cocos2d::Ref* m_eventTarget;
	SEL_TouchOutOfFrameEvent m_eventSelector;

	bool isOutsideFrame(cocos2d::Touch *pTouch);
	void setChildrenTouchEnabled(cocos2d::Node* pNode, bool bTouchEnabled);
};

#endif