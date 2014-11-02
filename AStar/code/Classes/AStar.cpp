#include "AStar.h"

AStar::AStar():
	m_numSurround(8),
	m_map(0),
	m_row(0),
	m_col(0)
{
	m_surround[0] = ASCOORD( 0,-1);
	m_surround[1] = ASCOORD( 0, 1);
	m_surround[2] = ASCOORD(-1, 0);
	m_surround[3] = ASCOORD( 1, 0);

	m_surround[4] = ASCOORD(-1,-1);
	m_surround[5] = ASCOORD( 1,-1);
	m_surround[6] = ASCOORD(-1, 1);
	m_surround[7] = ASCOORD( 1, 1);

	for(int i = 0 ; i<4 ; ++i)
		m_gAdd[i] = 10;

	for(int i = 4 ; i<8 ; ++i)
		m_gAdd[i] = 14;

	ClearObstacles();
}

void AStar::ClearObstacles()
{
	if(m_map)
	{
		int size = m_row*m_col/8+1;
		memset(m_map,0,size);
	}

	m_closeList.clear();
	m_openList.clear();
}

bool AStar::ComputeRoute()
{
	m_openList.clear();
	m_closeList.clear();

	if(m_row<=0 || m_col<=0 )
		return false;

	StepData sd(m_target);
	sd._g = 0;
	sd._h = computeH(m_target);

	m_openList.push_back(sd);

	while (!m_openList.empty())
	{
		popBestStep(&sd);
		m_closeList.push_back(sd);

		ASCOORD coord;
		
		for(int i = 0 ; i<m_numSurround ; ++i)
		{
			coord  = sd._coord;
			coord += m_surround[i];
			
			if(judgeSurround(coord,sd._coord,sd._g+m_gAdd[i]))
				return true;
		}
	}
	return false;
}

bool AStar::judgeSurround(const ASCOORD& coord,const ASCOORD& parentCoord,int G)
{
	if(coord._x>=0 && coord._x<m_col && coord._y>=0 && coord._y<m_row && !isInList(m_closeList,coord) && !IsObstacle(coord))
	{
		StepData* pSD = findFromList(m_openList,coord);
		if (pSD && pSD->_g > G)
		{
			pSD->_g = G;
			pSD->_parentCoord = parentCoord;
		}
		else if(!pSD)
		{
			StepData newSD(coord);
			newSD._g = G;
			newSD._h = computeH(coord);
			newSD._parentCoord = parentCoord;

			m_openList.push_back(newSD);

			if(newSD._coord == m_start)
				return true;
		}
	}

	return false;
}

bool AStar::popBestStep(StepData* pSD)
{
	std::list<StepData>::iterator       i   = m_openList.begin();
	std::list<StepData>::const_iterator end = m_openList.end();

	if(i!=end)
	{
		pSD->_g       = i->_g;
		pSD->_h       = i->_h;
		pSD->_coord   = i->_coord;
		pSD->_parentCoord = i->_parentCoord;

		i++;

		while(i!=end)
		{
			if(pSD->_g+pSD->_h > i->_g+i->_h)
			{
				pSD->_g       = i->_g;
				pSD->_h       = i->_h;
				pSD->_coord   = i->_coord;
				pSD->_parentCoord = i->_parentCoord;
			}

			i++;
		}

		i = m_openList.begin();
		while(i->_coord != pSD->_coord)
		{
			i++;
		}

		m_openList.erase(i);
		return true;
	}
	return false;
}

bool AStar::isInList(std::list<StepData>& list,const ASCOORD& coord)
{
	std::list<StepData>::iterator       i   = list.begin();
	std::list<StepData>::const_iterator end = list.end();

	while(i!=end)
	{
		if(coord == i->_coord)
			return true;

		i++;
	}
	return false;
}

StepData* AStar::findFromList(std::list<StepData>& list, const ASCOORD& coord)
{
	std::list<StepData>::iterator       i   = list.begin();
	std::list<StepData>::const_iterator end = list.end();

	while(i!=end)
	{
		if(coord == i->_coord)
			return &(*i);

		i++;
	}

	return 0;
}

bool AStar::GetRoute(std::vector<ASCOORD>* list)
{
	list->clear();

	StepData* sd = findFromList(m_openList,m_start);
	int size = m_openList.size();
	if(sd!=NULL)
	{
		list->push_back(sd->_coord);

		sd = findFromList(m_closeList,sd->_parentCoord);
		while(sd->_coord!=m_target)
		{
			list->push_back(sd->_coord);
			sd = findFromList(m_closeList,sd->_parentCoord);
		}
		list->push_back(sd->_coord);
		return true;
	}

	return false;
}

bool AStar::SetMapSize(int row,int col)
{
	if(row<= 0 || col<=0)
		return false;

	if(m_map)
		delete[] m_map;

	int size = row*col/8+1;

	m_map = new unsigned char[size];

	memset(m_map,0,size);

	m_row = row;
	m_col = col;

	return true;
}

void AStar::SetObstacle(const ASCOORD& coord)
{
	if(coord._x>=0 && coord._x<m_col && coord._y>=0 && coord._y<m_row)
	{
		int index = coord._y*m_col+coord._x;

		m_map[index/8] |= 1<<(index%8);
	}
}

void AStar::CancleObstacle(const ASCOORD& coord)
{
	if(coord._x>=0 && coord._x<m_col && coord._y>=0 && coord._y<m_row)
	{
		int index = coord._y*m_col+coord._x;

		m_map[index/8] &= ~(1<<index%8);
	}
}

bool AStar::IsObstacle(const ASCOORD& coord)
{
	if(coord._x>=0 && coord._x<m_col && coord._y>=0 && coord._y<m_row)
	{
		int index = coord._y*m_col+coord._x;

		return m_map[index/8] & (1<<(index%8));
	}
	return true;
}