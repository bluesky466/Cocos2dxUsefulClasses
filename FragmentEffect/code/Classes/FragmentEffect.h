#ifndef __SIMPLE_EFFECT_H__
#define __SIMPLE_EFFECT_H__

#include "cocos2d.h"
#include "cocoa/CCGeometry.h"

USING_NS_CC;

enum EffectSelect{ES_NONE              = 0,  //没有特效
				  ES_BLACK_AND_WHITE   = 1,  //黑白化
				  ES_OLD_PHOTOS        = 2,  //老照片
				  ES_INVERT            = 3,  //反相
				  ES_BURNS			   = 4,  //灼伤（偏红）
				  ES_POISONING		   = 5,  //中毒（偏绿）
				  ES_COLD			   = 6}; //寒冷（偏蓝）

/**
 * 简单的片段渲染效果
 * 先调用 setEffect 方法设置shader代码，再调用 setEffect 选择效果
 */
class FragmentEffect
{
public:
	///设置shader代码
	bool setShaderProgram(CCNode*);

	///选择效果
	void setEffect(int sel)
	{
		m_program->use();
		m_program->setUniformLocationWith1i(m_maxSel,sel);
	}

	///选择效果
	void setEffect(EffectSelect sel)
	{
		m_program->use();
		m_program->setUniformLocationWith1i(m_maxSel,sel);
	}

private:
	GLint		 m_maxSel;
	CCGLProgram* m_program;
};

#endif
