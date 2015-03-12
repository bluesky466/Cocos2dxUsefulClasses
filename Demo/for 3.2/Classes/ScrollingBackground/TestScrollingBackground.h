#ifndef _TEST_SCROLLING_BACKGROUND_H_
#define _TEST_SCROLLING_BACKGROUND_H_

#include "cocos2d.h"
#include "TestBase.h"
#include "ScrollingBackground.h"

/**
 * SpriteBackground类的演示demo
 * 有两种模式,循环滚动和丢弃滚动
 * 创建背景的操作都在 void okCallback(CCObject*) 这里进行的,注意看这里
 * setBgSpriteScrollBy 和 setFullScreen 方法没有做出演示,不过这两个方法也很简单,看看注释就会用了
 */
class TestScrollingBackground : public TestBase
{
public:
    virtual bool init();  

	///这里是菜单的设置,不用看
	void createInitMenu();//方向选择
	void dirChangeCallback(cocos2d::Ref*);

	///模式选择,循环播放模式和边滚动边丢弃精灵模式
	void modeChangeCallback(cocos2d::Ref*);

	void toggleCallback(cocos2d::Ref*);

	///确认选择按钮,创建背景的操作都在这里进行
	void okCallback(cocos2d::Ref*);

	///返回按钮,重新选择方向和模式
	void btBackCallback(cocos2d::Ref*);

	///滚动到下个画面或者上个画面
	void nextBgCallback(cocos2d::Ref*);

	///暂停和开始
	void pauseCallback(cocos2d::Ref*);

	//设置背景反方向滚动
	void reverseCallback(cocos2d::Ref*);

	///增加精灵背景
	void addBgSpriteCallback(cocos2d::Ref*);

	///循环模式当有一个背景彻底离开屏幕的时候调用
	void loopBgChangeCallback(cocos2d::Sprite* current,cocos2d::Sprite* follow,cocos2d::Sprite* leave);
	
	///丢弃模式当有一个背景彻底离开屏幕的时候调用
	void dropBgChangeCallback(cocos2d::Sprite* current,cocos2d::Sprite* follow,cocos2d::Sprite* leave);

	///每一帧背景移动的时候调用
	void bgMoveCallback(cocos2d::Sprite* current,cocos2d::Sprite* follow);
	
    CREATE_FUNC(TestScrollingBackground);
    CREATE_SCENE(TestScrollingBackground);

private:
	bool m_isRunning;

	ScrollingBackground* m_bg;
	ScrollDirection      m_direction;

	//开始的方向和模式选择菜单
	cocos2d::Layer*          m_setLayer;
	cocos2d::MenuItemToggle* m_dirSelItem;
	cocos2d::MenuItemToggle* m_modeSelItem;

	//背景创建之后的菜单
	cocos2d::Layer*   m_runLayer;
	cocos2d::Label*   m_itemTipsLabel; ///<item位置提示
	cocos2d::Label*   m_addTipsLabel;  ///<增加背景是否成功的提示
	cocos2d::Label*   m_numBgLabel;	   ///<背景数目的提示
	cocos2d::Sprite*  m_bgSprite;      ///<当前精灵背景
	cocos2d::Sprite*  m_pItem;         ///<一个物体,注意他并不是背景精灵的child,只不过跟随背景滚动而已,他是HelloWorld的child
	float	 m_velocity;

	//增加背景精灵
	bool addBgSprite(int index);

	//设置文字提示
	void setTextTips(cocos2d::Sprite* bgSprite);
};

#endif
