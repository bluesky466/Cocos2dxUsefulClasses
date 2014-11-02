#include "HelloWorldScene.h"

USING_NS_CC;
using namespace std;


Scene* HelloWorld::scene()
{
	Scene* scene = Scene::create();

	HelloWorld* layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}


bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	//初始化
	m_row = 15;
	m_col = 15;

	m_aStar.SetMapSize(m_row,m_col);
	m_aStar.ClearObstacles();
	m_aStar.SetDiagonalEnable(false);


	//起点
	Vec2 startPos(100.0f,100.0f);
	m_aStar.SetStart(computeASCOORD(startPos));

	m_people = Sprite::create("people.png");
	m_people->setAnchorPoint(Vec2(0.0f,0.0f));
	m_people->setPosition(correctPosition(startPos));
	this->addChild(m_people);
	

	//终点
	Vec2 targetPos(300.0f,300.0f);
	m_aStar.SetTarget(computeASCOORD(targetPos));
	
	m_target = Sprite::create("CloseNormal.png");
	m_target->setAnchorPoint(Vec2(0.0f,0.0f));
	m_target->setPosition(correctPosition(targetPos));
	this->addChild(m_target);


	//障碍物层
	m_obstacles = Layer::create();
	m_obstacles->setPosition(0,0);
	this->addChild(m_obstacles);

	//上边的墙
	for(int i = 0 ; i<m_col ; i++)
	{
		Sprite* block = Sprite::create("block.png");
		block->setAnchorPoint(Vec2(0.0f,0.0f));
		block->setPosition(i*40,m_row*40);
		this->addChild(block);
	}

	//右边的墙
	for(int i = 0 ; i<m_col ; i++)
	{
		Sprite*block = Sprite::create("block.png");
		block->setAnchorPoint(Vec2(0.0f,0.0f));
		block->setPosition(m_col*40,i*40);
		this->addChild(block);
	}
	
	//菜单
	MenuItemFont* goItem = MenuItemFont::create("GO!",CC_CALLBACK_1(HelloWorld::goCallback,this));
	goItem->setAnchorPoint(Vec2(0.0f,0.0f));
	goItem->setPosition(Vec2((m_col+2)*40,300));

	MenuItemFont* clearItem = MenuItemFont::create("RESET!",CC_CALLBACK_1(HelloWorld::clearCallback,this));
	clearItem->setAnchorPoint(Vec2(0.0f,0.0f));
	clearItem->setPosition(Vec2((m_col+2)*40,400));

	Menu* menu = Menu::create(goItem,clearItem,NULL);

	menu->setPosition(Vec2(0,0));
	this->addChild(menu);

	//触碰事件
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan,this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);


    return true;
}

void HelloWorld::clearCallback(Ref*)
{
	m_aStar.ClearObstacles();
	m_people->stopAllActions();

	//起点
	Vec2 startPos(100.0f,100.0f);
	m_aStar.SetStart(computeASCOORD(startPos));
	m_people->setPosition(correctPosition(startPos));
	
	//障碍物层
	this->removeChild(m_obstacles);
	m_obstacles = Layer::create();
	m_obstacles->setPosition(0,0);
	this->addChild(m_obstacles);
}

void HelloWorld::stepCallback()
{
	if(m_routeStep<m_route.size()-1)
	{
		m_people->runAction(Sequence::create(CCMoveTo::create(0.2f,Vec2(m_route[m_routeStep]._x*40.0f,m_route[m_routeStep]._y*40.0f)),CallFunc::create(CC_CALLBACK_0(HelloWorld::stepCallback,this)),NULL));
		m_routeStep++;
	}
}

void HelloWorld::goCallback(Ref*)
{
	if(m_aStar.ComputeRoute())
	{
		m_route.clear();
		m_routeStep = 0;
		m_aStar.GetRoute(&m_route);
	
		m_people->runAction(Sequence::create(CCMoveTo::create(0.2f,Vec2(m_route[m_routeStep]._x*40.0f,m_route[m_routeStep]._y*40.0f)),CallFunc::create(CC_CALLBACK_0(HelloWorld::stepCallback,this)),NULL));
		m_routeStep++;
	}
	else
		MessageBox("NO ROUTE!","NO ROUTE!");
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* e)
{
	ASCOORD coord = computeASCOORD(touch->getLocation());

	if(coord._x<m_col && coord._y<m_row && coord!=m_aStar.GetStart() && coord!=m_aStar.GetTarget())
	{
		Vec2 pos = correctPosition(touch->getLocation());

		Sprite* block = Sprite::create("block.png");
		block->setAnchorPoint(Vec2(0.0f,0.0f));
		block->setPosition(pos);
		m_obstacles->addChild(block);

		m_aStar.SetObstacle(coord);
	}
	
	return false;
}