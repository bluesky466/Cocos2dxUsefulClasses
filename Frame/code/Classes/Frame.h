#ifndef _CHAT_FRAME_H_
#define _CHAT_FRAME_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

/**
 *  对话框基类
 *  抽象类,子类必须实现setControls方法,将所有的控件push_back到m_controls中
 *  功能:如果鼠标点击位置在对话框之外，自动隐藏对话框，并使得内部控件不可触碰
 */
class Frame : public CCObject,public CCTargetedTouchDelegate 
{
public:
	Frame();
	~Frame();

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	bool isOutsideFrame(CCTouch *pTouch);

	///设置对话框是否可见的同时设置对话框中的控件是否可触碰(防止对话框不可见,但控件仍收到触碰消息)
	void setFrameVisble(bool bVisible);

	virtual bool init(UILayer* frameLayer,CCNode* frameBg);

	UILayer* getFrameLayer() {return m_frameLayer;}
protected:
	UILayer*   m_frameLayer;
	CCNode*    m_frameBg;
	CCSize     m_frameSize;
	CCPoint    m_anchorPoint;  ///<锚点在背景坐标系的坐标,坐标范围是(0.0f,0.0f)到m_frameSize
	bool       m_bFrameVisible;
	std::vector<Widget*> m_controls;

	///纯虚函数,子类必须实现,将m_frameLayer的所有控件push_back到m_controls中
	virtual void setControls() = 0;
};


class RegisterFrame : public Frame
{
public:
	bool init(UILayer* frameLayer,CCNode* frameBg);
private:
	void setControls();
};
#endif