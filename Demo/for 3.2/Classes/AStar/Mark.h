/****************************************************************************
 author : LuoYu
 
 e-mail : 466474482@qq.com

 github : https://github.com/bluesky466/cocos2dxUsefulClasses
 ****************************************************************************/
#ifndef _MARK_H_
#define _MARK_H_

#include <string.h>

/**
 * 二维标记类
 * 相当于一个二维的bool数组
 * 每一个二进制位代表一个项，如果该二进制位为1则为true，为0则为false
 */
class Mark
{
public:
	Mark();
	~Mark();

	///设置大小,返回m_marks数组的大小
	int setSize(unsigned int row, unsigned int col); 

	///标记某坐标,就是设置某坐标标志位为1
	void setMark(unsigned int row, unsigned int col);

	///取消标记某坐标,就是设置某坐标标志位为0
	void cancelMark(unsigned int row, unsigned int col);

	///查询是否某坐标是否被标记
	bool isMask(unsigned int row, unsigned int col)const;
	
	///取消所有标记
	void clearMarks();

	const Mark& operator = (const Mark& mark);

	unsigned int getRow() {return m_row;}
	unsigned int getCol() {return m_col;}

private:
	unsigned char* m_marks;
	unsigned int m_row;
	unsigned int m_col;
};

inline const Mark& Mark::operator = (const Mark& mark)
{
	int size = setSize(mark.m_row,mark.m_col);
	memcpy(m_marks,mark.m_marks,size*sizeof(unsigned char));
	return *this;
}

#endif