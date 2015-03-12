#ifndef _TEST_FRAGMENT_EFFECT_H_
#define _TEST_FRAGMENT_EFFECT_H_

#include "cocos2d.h"
#include "TestBase.h"
#include "cocos2d\cocos\editor-support\cocostudio\CocoStudio.h"
#include "cocos2d\cocos\ui\CocosGUI.h"
#include "FragmentEffect.h"

class TestFragmentEffect : public TestBase
{
public:
    bool init();
	void toggleCallback(cocos2d::Ref* pSender);
	void gotoUserDefine(cocos2d::Ref*);

    CREATE_FUNC(TestFragmentEffect);
    CREATE_SCENE(TestFragmentEffect);

private:
	int m_indexSprite;
	int m_indexArmature;
	cocos2d::Sprite* m_bgSprite;
	cocos2d::MenuItemToggle* m_sel;
	cocostudio::Armature* m_armature;
};

#endif
