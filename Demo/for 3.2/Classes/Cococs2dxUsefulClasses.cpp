#include "Cococs2dxUsefulClasses.h"

USING_NS_CC;

bool Cococs2dxUsefulClasses::init()
{
	if(!TestBase::init())
    {
        return false;
    }
	
	m_fontSize = 20.0f;
	float ereaHeight = m_fontSize + 5.0f;

	Vec2 basePos(m_visibleCenter.x,
				 m_visibleCenter.y - DemoNumber / 2 * ereaHeight);

	Vector<MenuItem*> itemVector;
	MenuItemFont::setFontSize(m_fontSize);
	for(int i = 0 ; i<DemoNumber ; i++)
	{
		MenuItemFont* item = MenuItemFont::create(DemoNames[i],CC_CALLBACK_1(Cococs2dxUsefulClasses::selectDemo,this));
		item->setTag(i);
		item->setAnchorPoint(Vec2(0.5f,0.5f));
		item->setPosition(basePos);
		itemVector.pushBack(item);
		
		basePos.y += ereaHeight;
	}
	
	Menu* menu = Menu::createWithArray(itemVector);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void Cococs2dxUsefulClasses::selectDemo(Ref* ref)
{
	switch(((Node*)ref)->getTag())
	{
	case Cococs2dxUsefulClasses::DS_AStar:
		Director::getInstance()->replaceScene(TestAStar::createScene());
		break;
	case Cococs2dxUsefulClasses::DS_FragmentEffect:
		Director::getInstance()->replaceScene(TestFragmentEffect::createScene());
		break;
	case Cococs2dxUsefulClasses::DS_FrameManager:
		Director::getInstance()->replaceScene(TestFrameManager::createScene());
		break;
	case Cococs2dxUsefulClasses::DS_Joystick:
		Director::getInstance()->replaceScene(TestJoystick::createScene());
		break;
	case Cococs2dxUsefulClasses::DS_ScrollingBackground:
		Director::getInstance()->replaceScene(TestScrollingBackground::createScene());
		break;
	case Cococs2dxUsefulClasses::DS_NumberCount:
		Director::getInstance()->replaceScene(TestNumberCount::createScene());
		break;
	case Cococs2dxUsefulClasses::DS_Subtitles:
		Director::getInstance()->replaceScene(TestSubtitles::createScene());
		break;
	default:
		break;
	}
}

void Cococs2dxUsefulClasses::btBackCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}