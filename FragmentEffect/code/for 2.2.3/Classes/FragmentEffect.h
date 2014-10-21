/****************************************************************************
 for cocos2d-x 2.2.3

 author : LuoYu
 
 e-mail : 466474482@qq.com

 github : https://github.com/bluesky466/cocos2dxUsefulClasses
 ****************************************************************************/

#ifndef __SIMPLE_EFFECT_H__
#define __SIMPLE_EFFECT_H__

#include "cocos2d.h"
#include "cocoa/CCGeometry.h"
#include <vector>

USING_NS_CC;

enum EffectSelect
{
	ES_NONE              = 0,  //没有特效
	ES_BLACK_AND_WHITE   = 1,  //黑白化
	ES_OLD_PHOTOS        = 2,  //老照片
	ES_INVERT            = 3,  //反相
	ES_BURNS			 = 4,  //灼伤（偏红）
	ES_POISONING		 = 5,  //中毒（偏绿）
	ES_COLD			     = 6,  //寒冷（偏蓝）

	ES_USER_DEFINED //自定义的特效矩阵序号从这里开始
};

struct Matrix44
{
	GLfloat _matrix[16];
};

struct EffectListNode
{
	CCNode* _pNode;
	GLint   _uniformMatrix;
	int		_effectSelect;
};

/**
 * 单例模式的简单的片段渲染效果类
 * 先调用 addEffectNode 增加需要渲染特效的节点
 * 然后可以调用 setEffect 设置特效或 getEffectSelect的到特效选择
 */
class FragmentEffect
{
public:
	static FragmentEffect* getInstance();
	static void destroy();

	/**
	 * 增加自定义的特效矩阵
	 *@param matrix 特效矩阵
	 *@return 返回节点在 m_nodeList 的序号,失败则返回-1
	 */
	int addEffectMatrix(const Matrix44& matrix);

	/**
	 * 设置节点的特效，这个方法比较高效
	 *@param nodeIndex 节点序号，即addEffectNode方法的返回值
	 *@param sel 特效的选择
	 */
	bool setEffect(unsigned int nodeIndex,unsigned int sel);

	/**
	 * 设置节点的特效，建议使用更加高效的 bool setEffect(int nodeIndex,int sel)
	 * 必须先调用 addEffectNode 将需要渲染特效的节点加到队列中
	 *@param pNode 节点指针
	 *@param sel 特效的选择
	 */
	bool setEffect(CCNode* pNode,unsigned int sel);

	///增加需要渲染特效的节点
	int  addEffectNode(CCNode* pNode);

	///得到节点的特效选择，若没有此节点则返回-1，这个方法比较高效
	int  getEffectSelect(unsigned int nodeInde);

	///得到节点的特效选择，若没有此节点则返回-1，建议使用更加高效的 int getEffectSelect(int nodeInde)
	int  getEffectSelect(CCNode* pNode);
	
private:
	static FragmentEffect* s_instance;

	FragmentEffect();
	~FragmentEffect(){}

	std::vector<Matrix44>		m_matrices;
	std::vector<EffectListNode> m_nodeList;
};

inline int FragmentEffect::addEffectMatrix(const Matrix44& matrix) 
{
	m_matrices.push_back(matrix);
	return m_matrices.size()-1;
}

#endif
