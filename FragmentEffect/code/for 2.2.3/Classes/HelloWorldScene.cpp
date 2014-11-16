#include "HelloWorldScene.h"




CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	//都是菜单设置，可以不看
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));
	
	CCMenuItemFont* MIF0 = CCMenuItemFont::create("ES_NONE");
	MIF0->setColor(ccRED);
	MIF0->setTag(0);
	CCMenuItemFont* MIF1 = CCMenuItemFont::create("ES_BLACK_AND_WHITE");
	MIF1->setColor(ccRED);
	MIF1->setTag(0);
	CCMenuItemFont* MIF2 = CCMenuItemFont::create("ES_OLD_PHOTOS");
	MIF2->setColor(ccRED);
	MIF2->setTag(0);
	CCMenuItemFont* MIF3 = CCMenuItemFont::create("ES_INVERT");
	MIF3->setColor(ccRED);
	MIF3->setTag(0);
	CCMenuItemFont* MIF4 = CCMenuItemFont::create("ES_BURNS");
	MIF4->setColor(ccRED);
	MIF4->setTag(0);
	CCMenuItemFont* MIF5 = CCMenuItemFont::create("ES_POISONING");
	MIF5->setColor(ccRED);
	MIF5->setTag(0);
	CCMenuItemFont* MIF6 = CCMenuItemFont::create("ES_COLD");
	MIF6->setColor(ccRED);
	MIF6->setTag(0);
	CCMenuItemFont* MIF7 = CCMenuItemFont::create("ES_USER_DEFINED");
	MIF7->setColor(ccRED);
	MIF7->setTag(0);
	m_sel = CCMenuItemToggle::createWithTarget(this,
											menu_selector(HelloWorld::toggleCallback),
											MIF0,MIF1,MIF2,MIF3,MIF4,MIF5,MIF6,MIF7,
											NULL);
	m_sel->setTag(0);
	m_sel->setAnchorPoint(ccp(0.5f,1.0f));
	m_sel->setPosition(ccp(visibleSize.width/2, visibleSize.height-7));
	
	CCMenuItemImage* last = CCMenuItemImage::create("lastNor.png","lastCli.png",0,this,menu_selector(HelloWorld::toggleCallback));
	last->setTag(-1);
	last->setAnchorPoint(ccp(0.5f,1.0f));
	last->setPosition(ccp(visibleSize.width/10*1, visibleSize.height));

	CCMenuItemImage* next = CCMenuItemImage::create("nextNor.png","nextCli.png",0,this,menu_selector(HelloWorld::toggleCallback));
	next->setTag(1);
	next->setAnchorPoint(ccp(0.5f,1.0f));
	next->setPosition(ccp(visibleSize.width/10*9, visibleSize.height));

	CCMenuItemFont* UDF = CCMenuItemFont::create("goto ES_USER_DEFINED",this,menu_selector(HelloWorld::gotoUserDefine));
	UDF->setColor(ccRED);
	UDF->setTag(0);
	UDF->setPosition(ccp(visibleSize.width/2, visibleSize.height-100));

    CCMenu* pMenu = CCMenu::create(pCloseItem,m_sel,last,next,UDF,NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);


    CCSprite* sprite= CCSprite::create("HelloWorld.png");
    sprite->setPosition(ccp(visibleSize.width/2, visibleSize.height/2));
    this->addChild(sprite, 0);
	
	//加载骨骼动画
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Hero0.png","Hero0.plist","Hero.ExportJson");
	m_armature = CCArmature::create("Hero");
	m_armature->getAnimation()->playByIndex(0);
	m_armature->setPosition(ccp(visibleSize.width/2, 0.0f));
	m_armature->setAnchorPoint(ccp(0.5f,0.0f));
	this->addChild(m_armature,1);

	//淡出淡入这样的动作依然可以使用
	m_armature->runAction(CCSequence::create(CCFadeOut::create(0.5f),CCFadeIn::create(0.5f),0));

	//就这几句是设置效果的，toggleCallback 和 gotoUserDefine方法里面也有
	m_index = FragmentEffect::getInstance()->addEffectNode(m_armature); //m_index接收返回的序号，setEffect 和 getEffectSelect 的时候使用

	Matrix44 m = {
		0.5f,0.0f,0.0f,0.0f,
		0.0f,0.5f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f
	};

	FragmentEffect::getInstance()->addEffectMatrix(m);//增加自定义特效矩阵,序号从 ES_USER_DEFINED 开始增加

    return true;
	
}

void HelloWorld::toggleCallback(CCObject* pSender)
{
	int sel = m_sel->getSelectedIndex();

	int tag = ((CCNode*)pSender)->getTag();

	if(tag<0)
	{
		sel--;
		if(sel<0)
			sel = 7;

		m_sel->setSelectedIndex(sel);
	}
	else if(tag>0)
	{
		sel++;
		sel%=8;

		m_sel->setSelectedIndex(sel);
	}

	//FragmentEffect::getInstance()->setEffect(m_armature,sel);
	//用m_index速度会快很多
	FragmentEffect::getInstance()->setEffect(m_index,sel);
	
}

void HelloWorld::gotoUserDefine(CCObject*)
{
	m_sel->setSelectedIndex(7);

	//FragmentEffect::getInstance()->setEffect(m_armature,ES_USER_DEFINED+0);
	//用m_index速度会快很多
	FragmentEffect::getInstance()->setEffect(m_index,ES_USER_DEFINED+0);
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
