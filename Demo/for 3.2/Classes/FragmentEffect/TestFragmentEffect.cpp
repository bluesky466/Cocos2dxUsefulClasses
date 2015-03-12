#include "TestFragmentEffect.h"

USING_NS_CC;
using namespace cocostudio;

bool TestFragmentEffect::init()
{
    if ( !TestBase::init() )
    {
        return false;
    }
    
    //都是菜单设置，可以不看
	MenuItemFont* MIF0 = MenuItemFont::create("ES_NONE");
	MIF0->setColor(Color3B::RED);
	MIF0->setTag(0);
	MenuItemFont* MIF1 = MenuItemFont::create("ES_BLACK_AND_WHITE");
	MIF1->setColor(Color3B::RED);
	MIF1->setTag(0);
	MenuItemFont* MIF2 = MenuItemFont::create("ES_OLD_PHOTOS");
	MIF2->setColor(Color3B::RED);
	MIF2->setTag(0);
	MenuItemFont* MIF3 = MenuItemFont::create("ES_INVERT");
	MIF3->setColor(Color3B::RED);
	MIF3->setTag(0);
	MenuItemFont* MIF4 = MenuItemFont::create("ES_BURNS");
	MIF4->setColor(Color3B::RED);
	MIF4->setTag(0);
	MenuItemFont* MIF5 = MenuItemFont::create("ES_POISONING");
	MIF5->setColor(Color3B::RED);
	MIF5->setTag(0);
	MenuItemFont* MIF6 = MenuItemFont::create("ES_COLD");
	MIF6->setColor(Color3B::RED);
	MIF6->setTag(0);
	MenuItemFont* MIF7 = MenuItemFont::create("ES_USER_DEFINED");
	MIF7->setColor(Color3B::RED);
	MIF7->setTag(0);
	m_sel = MenuItemToggle::createWithCallback(CC_CALLBACK_1(TestFragmentEffect::toggleCallback,this),MIF0,MIF1,MIF2,MIF3,MIF4,MIF5,MIF6,MIF7,NULL);

	m_sel->setTag(0);
	m_sel->setAnchorPoint(Vec2(0.5f,1.0f));
	m_sel->setPosition(Vec2(m_visibleOrigin.x + m_visibleSize.width / 2, 
							m_visibleOrigin.y + m_visibleSize.height - 7));
	
	MenuItemImage* last = MenuItemImage::create("FragmentEffect/lastNor.png","FragmentEffect/lastCli.png","",CC_CALLBACK_1(TestFragmentEffect::toggleCallback,this));
	last->setTag(-1);
	last->setAnchorPoint(Vec2(0.5f,1.0f));
	last->setPosition(Vec2(m_visibleOrigin.x + m_visibleSize.width / 10 * 1,
						   m_visibleOrigin.y + m_visibleSize.height));

	MenuItemImage* next = MenuItemImage::create("FragmentEffect/nextNor.png","FragmentEffect/nextCli.png","",CC_CALLBACK_1(TestFragmentEffect::toggleCallback,this));
	next->setTag(1);
	next->setAnchorPoint(Vec2(0.5f,1.0f));
	next->setPosition(Vec2(m_visibleOrigin.x + m_visibleSize.width / 10 * 9, 
						   m_visibleOrigin.y + m_visibleSize.height));

	MenuItemFont* UDF = MenuItemFont::create("goto ES_USER_DEFINED",CC_CALLBACK_1(TestFragmentEffect::gotoUserDefine,this));
	UDF->setColor(Color3B::RED);
	UDF->setTag(0);
	UDF->setPosition(Vec2(m_visibleOrigin.x + m_visibleSize.width / 2, 
						  m_visibleOrigin.y + m_visibleSize.height - 50.0f));

    Menu* pMenu = Menu::create(m_sel,last,next,UDF,NULL);
	pMenu->setPosition(Vec2::ZERO);
    this->addChild(pMenu, 1);


    m_bgSprite = Sprite::create("FragmentEffect/HelloWorld.png");
    m_bgSprite->setPosition(Vec2(m_visibleOrigin.x + m_visibleSize.width / 2, 
								 m_visibleOrigin.y + m_visibleSize.height / 2));
    this->addChild(m_bgSprite, 0);
	
	//加载骨骼动画
	ArmatureDataManager::getInstance()->addArmatureFileInfo("FragmentEffect/Hero0.png","FragmentEffect/Hero0.plist","FragmentEffect/Hero.ExportJson");
	m_armature = Armature::create("Hero");
	m_armature->getAnimation()->playWithIndex(0);
	m_armature->setPosition(Vec2(m_visibleOrigin.x+m_visibleSize.width/2, 0.0f));
	m_armature->setAnchorPoint(Vec2(0.5f,0.0f));
	this->addChild(m_armature,1);

	//淡出淡入这样的动作依然可以使用
	m_armature->runAction(Sequence::create(FadeOut::create(0.5f),FadeIn::create(0.5f),0));

	//就这几句是设置效果的，toggleCallback 和 gotoUserDefine方法里面也有
	m_indexSprite   = FragmentEffect::getInstance()->addEffectNode(m_bgSprite); //m_indexSprite接收返回的序号，setEffect 和 getEffectSelect 的时候使用
	m_indexArmature = FragmentEffect::getInstance()->addEffectNode(m_armature); //m_indexArmature接收返回的序号，setEffect 和 getEffectSelect 的时候使用


	float f[] = {
		0.5f,0.0f,0.0f,0.0f,
		0.0f,0.5f,0.0f,0.0f,
		0.0f,0.0f,0.5f,0.0f,
		0.0f,0.0f,0.0f,0.5f
	};
	FragmentEffect::getInstance()->addEffectMatrix(Mat4(f));//增加自定义特效矩阵,序号从 ES_USER_DEFINED 开始增加

    return true;
}

void TestFragmentEffect::gotoUserDefine(Ref* ref)
{
	m_sel->setSelectedIndex(7);

	//FragmentEffect::getInstance()->setEffect(m_armature,ES_USER_DEFINED+0);
	//用m_indexArmature速度会快很多
	FragmentEffect::getInstance()->setEffect(m_indexArmature,ES_USER_DEFINED+0);

	FragmentEffect::getInstance()->setEffect(m_indexSprite,ES_USER_DEFINED+0);
}

void TestFragmentEffect::toggleCallback(Ref* pSender)
{
	int sel = m_sel->getSelectedIndex();

	int tag = ((Node*)pSender)->getTag();

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
	//用m_indexArmature速度会快很多
	FragmentEffect::getInstance()->setEffect(m_indexArmature,sel);

	FragmentEffect::getInstance()->setEffect(m_bgSprite,sel);
	
}