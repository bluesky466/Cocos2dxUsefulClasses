#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ScrollingBackground.h"

USING_NS_CC;
using namespace ui;

/**
 * SpriteBackground类的演示demo
 * 屏幕大小也可以在main.cpp设置,背景会自动填满屏幕
 * 有两种模式,循环滚动和丢弃滚动
 * 创建背景的操作都在 void okCallback(CCObject*) 这里进行的,注意看这里
 * setBgSpriteScrollBy 和 setFullScreen 方法没有做出演示,不过这两个方法也很简单,看看注释就会用了
 */
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
   
    virtual bool init();  
   
    void quitCallback(Ref*);

	///这里是菜单的设置,不用看
	void createInitMenu();//方向选择
	void dirChangeCallback(Ref*);

	///模式选择,循环播放模式和边滚动边丢弃精灵模式
	void modeChangeCallback(Ref*);

	void toggleCallback(Ref*);

	///确认选择按钮,创建背景的操作都在这里进行
	void okCallback(Ref*);

	///返回按钮,重新选择方向和模式
	void backCallback(Ref*);

	///滚动到下个画面或者上个画面
	void nextBgCallback(Ref*);

	///暂停和开始
	void pauseCallback(Ref*);

	//设置背景反方向滚动
	void reverseCallback(Ref*);

	///增加精灵背景
	void addBgSpriteCallback(Ref*);

	///循环模式当有一个背景彻底离开屏幕的时候调用
	void loopBgChangeCallback(Sprite* current,Sprite* follow,Sprite* leave);
	
	///丢弃模式当有一个背景彻底离开屏幕的时候调用
	void dropBgChangeCallback(Sprite* current,Sprite* follow,Sprite* leave);

	///每一帧背景移动的时候调用
	void bgMoveCallback(Sprite* current,Sprite* follow);

    CREATE_FUNC(HelloWorld);

private:
	ScrollingBackground* m_bg;
	ScrollDirection      m_direction;

	//开始的方向和模式选择菜单
	Layer*          m_setLayer;
	MenuItemToggle* m_dirSelItem;
	MenuItemToggle* m_modeSelItem;

	//背景创建之后的菜单
	Layer*   m_runLayer;
	Label*   m_itemTipsLabel; ///<item位置提示
	Label*   m_addTipsLabel;  ///<增加背景是否成功的提示
	Label*   m_numBgLabel;	   ///<背景数目的提示
	Sprite*  m_bgSprite;      ///<当前精灵背景
	Sprite*  m_pItem;         ///<一个物体,注意他并不是背景精灵的child,只不过跟随背景滚动而已,他是HelloWorld的child
	float	 m_velocity;

	//增加背景精灵
	bool addBgSprite(int index);

	//设置文字提示
	void setTextTips(Sprite* bgSprite);
};

#endif // __HELLOWORLD_SCENE_H__
