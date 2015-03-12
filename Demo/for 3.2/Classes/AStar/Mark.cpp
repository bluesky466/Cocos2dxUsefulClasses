#include "Mark.h"

Mark::Mark():
	m_marks(0),
	m_row(0),
	m_col(0)
{
}

Mark::~Mark()
{
	if(m_marks)
		delete[] m_marks;
}

int Mark::setSize(unsigned int row, unsigned int col)
{
	if(m_marks)
		delete[] m_marks;

	m_row = row;
	m_col = col;

	int size = m_row * m_col / 8 + 1; //加一是为了留空间给m_row*m_col/8的余数部分
	m_marks  = new unsigned char[size];
	memset(m_marks,0,size);

	return size;
}

void Mark::setMark(unsigned int row, unsigned int col)
{
	if(row<m_row && col<m_col)
	{
		int index = row*m_col+col;

		//index&7 等价于 index%8
		m_marks[index/8] |= 1<<(index&7);
	}
}

void Mark::cancelMark(unsigned int row, unsigned int col)
{
	if(row<m_row && col<m_col)
	{
		int index = row*m_col+col;

		//index&7 等价于 index%8
		m_marks[index/8] &= ~(1<<(index&7));
	}
}

bool Mark::isMask(unsigned int row, unsigned int col)const
{
	if(row<m_row && col<m_col)
	{
		int index = row*m_col+col;

		//index&7 等价于 index%8
		return m_marks[index/8] & (1<<(index&7));
	}

	return true;
}

void Mark::clearMarks()
{
	if(m_marks)
		memset(m_marks,0,m_row*m_col/8+1);
}