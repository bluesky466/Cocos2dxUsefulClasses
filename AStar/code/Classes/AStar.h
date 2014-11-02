/****************************************************************************
 author : LuoYu
 
 e-mail : 466474482@qq.com

 github : https://github.com/bluesky466/cocos2dxUsefulClasses
 ****************************************************************************/
#ifndef _A_STAR_H_
#define _A_STAR_H_

#include <math.h>
#include <list>
#include <vector>

struct ASCOORD
{
	int _x;
	int _y;

	ASCOORD()
	{
	}
	
	ASCOORD(int x,int y):
		_x(x),
		_y(y)
	{
	}

	inline bool operator == (const ASCOORD& coord)const
	{
		return this->_x == coord._x && this->_y == coord._y;
	}

	inline bool operator != (const ASCOORD& coord)const
	{
		return this->_x != coord._x || this->_y != coord._y;
	}

	inline void operator += (const ASCOORD& coord)
	{
		this->_x += coord._x ;
		this->_y += coord._y;
	}
};

struct StepData
{
	int _h;
	int _g;

	ASCOORD _coord;
	ASCOORD _parentCoord;

	StepData()
	{
	}

	StepData(const ASCOORD& coord):
		_coord(coord)
	{
	}
};

/**
 * A* 寻路算法
 * 必须先调用SetMapSize设置地图大小
 */
class AStar
{
public:
	AStar();

	///设置地图的大小,必须在一开始设置
	bool SetMapSize(int row,int col);

	/**
	 * 计算路径
	 * @return true - 有从起点到终点的路径,调用GetRoute方法获得路径; false - 找不到从起点到终点的路径 
	 */
	bool ComputeRoute();

	///获得从起点到终点的路径
	bool GetRoute(std::vector<ASCOORD>* list);

	///设置障碍物
	void SetObstacle(const ASCOORD& coord);

	///取消障碍物
	void CancleObstacle(const ASCOORD& coord);

	///清除所有障碍物
	void ClearObstacles();

	///判断所给坐标上有没有障碍物
	bool IsObstacle(const ASCOORD& coord);
	
	int  GetRow()	{return m_row;}
	int  GetCol()   {return m_col;}
	void SetStart(const ASCOORD& coord)  {m_start  = coord;}
	void SetTarget(const ASCOORD& coord) {m_target = coord;}
	const ASCOORD& GetStart()            {return m_start;}
	const ASCOORD& GetTarget()           {return m_target;}

	///设置可不可以走对角线
	void SetDiagonalEnable(bool bEnable) {m_numSurround = bEnable?8:4;}
	
private:
	unsigned char* m_map;
	int  m_row;
	int  m_col;

	ASCOORD m_target;
	ASCOORD m_start;

	//在计算四周方块的时候使用
	int     m_numSurround;
	int     m_gAdd[8];
	ASCOORD m_surround[8];
	
	std::list<StepData> m_openList;
	std::list<StepData> m_closeList;
	
	bool popBestStep(StepData* pSD); 
	bool isInList(std::list<StepData>& list, const ASCOORD& coord);
	bool judgeSurround(const ASCOORD& coord,const ASCOORD& parentCoord, int G);

	StepData* findFromList(std::list<StepData>& list, const ASCOORD& coord);

	int computeH(const ASCOORD& coord) {return abs(m_target._x-coord._x)*10 + abs(m_target._y-coord._y)*10;}
};

#endif