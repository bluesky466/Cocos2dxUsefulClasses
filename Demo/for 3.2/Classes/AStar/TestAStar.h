#ifndef _TEST_ASTAR_H_
#define _TEST_ASTAR_H_

#include "cocos2d.h"
#include "TestBase.h"
#include "AStar.h"

#define FILE_TARGET_TEX "AStar/CloseNormal.png"
#define FILE_PEOPLE_TEX "AStar/people.png"
#define FILE_BLOCK_TEX  "AStar/block.png"

class TestAStar : public TestBase
{
public:
    bool init();  
	
	void goCallback(cocos2d::Ref*);
	void clearCallback(cocos2d::Ref*);
	void stepCallback();
	bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);

	CREATE_FUNC(TestAStar);
    CREATE_SCENE(TestAStar);

private:
	int    m_row;
	int    m_col;
	AStar  m_aStar;
	
	cocos2d::Size    m_blockSize;
	cocos2d::Layer*  m_obstacles;
	cocos2d::Sprite* m_people;
	cocos2d::Sprite* m_target;
	
	ASCOORD m_startCoord;
	ASCOORD m_targetCoord;

	int m_routeStep;
	std::vector<ASCOORD> m_route;

	void setWalls();
	void setDesignResolutionSize();

	//是否点击在围墙里面
	bool isTouchInWalls(const ASCOORD& coord)
	{
		return  coord._x>0 && coord._y>0 &&coord._x<m_col && coord._y<m_row;
	}

	//偏移到格子的左下角
	cocos2d::Vec2 correctPosition(const cocos2d::Vec2& pos)   
	{
		return cocos2d::Vec2(((int)(pos.x / m_blockSize.width)) * m_blockSize.width, 
					         ((int)(pos.y / m_blockSize.height)) * m_blockSize.height);
	}
	
	ASCOORD Vec2ToAscoord(const cocos2d::Vec2& pos)
	{
		return ASCOORD((int)(pos.x/m_blockSize.width),
					   (int)(pos.y/m_blockSize.height));
	}

	cocos2d::Vec2 ascoordToVec2(const ASCOORD& coord)  
	{
		return cocos2d::Vec2(coord._x * m_blockSize.width, 
					         coord._y * m_blockSize.height);
	}
};

#endif
