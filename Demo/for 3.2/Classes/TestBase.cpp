#include "TestBase.h"
#include "Cococs2dxUsefulClasses.h"

USING_NS_CC;

bool TestBase::init()
{
	if(!Layer::init())
		return false;

	setDesignResolutionSize();

	m_visibleSize   = Director::getInstance()->getVisibleSize();
    m_visibleOrigin = Director::getInstance()->getVisibleOrigin();
	m_visibleCenter = Vec2(m_visibleSize.width/2 + m_visibleOrigin.x, 
						   m_visibleSize.height/2 + m_visibleOrigin.y);

	auto item = MenuItemFont::create("BACK", CC_CALLBACK_1(TestBase::btBackCallback, this));
    
	item->setPosition(Vec2(m_visibleOrigin.x + m_visibleSize.width  - item->getContentSize().width / 2 ,
						   m_visibleOrigin.y + m_visibleSize.height - item->getContentSize().height / 2));

    auto menu = Menu::create(item, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	return true;
}

void TestBase::btBackCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(Cococs2dxUsefulClasses::createScene());
}
