#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos2d\cocos\editor-support\cocostudio\CocoStudio.h"
#include "cocos2d\cocos\ui\CocosGUI.h"
#include "FragmentEffect.h"

USING_NS_CC;
using namespace cocostudio;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    void menuCloseCallback(cocos2d::Ref* pSender);
	void toggleCallback(Ref* pSender);
	void gotoUserDefine(Ref*);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	MenuItemToggle* m_sel;
	int             m_indexSprite;
	int             m_indexArmature;
	Sprite*         m_bgSprite;
	Armature*       m_armature;
};

#endif // __HELLOWORLD_SCENE_H__
