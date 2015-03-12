#ifndef _COCOS2DX_USEFUL_CLASSES_H_
#define _COCOS2DX_USEFUL_CLASSES_H_

#include "cocos2d.h"
#include "TestBase.h"

#include "AStar/TestAStar.h"
#include "FragmentEffect/TestFragmentEffect.h"
#include "FrameManager/TestFrameManager.h"
#include "Joystick/TestJoystick.h"
#include "ScrollingBackground/TestScrollingBackground.h"
#include "NumberCount/TestNumberCount.h"
#include "Subtitles/TestSubtitles.h"

const char* const DemoNames[] = {"AStar",
								"FragmentEffect",
								"FrameManager",
								"Joystick",
								"ScrollingBackground",
								"NumberCount",
								"Subtitles"};

const int DemoNumber = 7;

class Cococs2dxUsefulClasses : public TestBase
{
private:
	enum DemoSelect
	{
		DS_AStar = 0,
		DS_FragmentEffect,
		DS_FrameManager,
		DS_Joystick,
		DS_ScrollingBackground,
		DS_NumberCount,
		DS_Subtitles
	};
public:
	bool init();
	void selectDemo(cocos2d::Ref*);
	void btBackCallback(cocos2d::Ref*);

    CREATE_FUNC(Cococs2dxUsefulClasses);
	CREATE_SCENE(Cococs2dxUsefulClasses);

private:
	cocos2d::MenuItemToggle* m_demoSelItem;
	float m_fontSize;
};


#endif