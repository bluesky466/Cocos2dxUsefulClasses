#ifndef _TEST_FRAME_MANAGER_H_
#define _TEST_FRAME_MANAGER_H_

#include "cocos2d.h"
#include "TestBase.h"
#include "FrameManager.h"

class TestFrameManager : public TestBase
{
public:
    bool init();  
    void touchOutOfFrame(cocos2d::ui::Widget*);
	void btVisbleCallback(cocos2d::Ref*);
	void touchUiItemCallBack(cocos2d::Ref*,cocos2d::ui::Widget::TouchEventType);

    CREATE_FUNC(TestFrameManager);
    CREATE_SCENE(TestFrameManager);

private:
	FrameManager m_frame;
};

#endif
