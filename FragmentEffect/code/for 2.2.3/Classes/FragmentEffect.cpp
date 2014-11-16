#include "FragmentEffect.h"

USING_NS_CC;

FragmentEffect* FragmentEffect::s_instance = 0;

FragmentEffect* FragmentEffect::getInstance()
{
	if(!s_instance)
		s_instance = new FragmentEffect();

	return s_instance;
}

void FragmentEffect::destroy()
{
	if(s_instance)
	{
		delete s_instance;
		s_instance = 0;
	}
}

FragmentEffect::FragmentEffect()
{
	m_nodeList.clear();

	//先增加几个预定的效果矩阵

	//没有特效
	Matrix44 matrix0 = {
		1.0f, 0.0f, 0.0f, 0.0f,						
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	m_matrices.push_back(matrix0);

	//黑白化
	Matrix44 matrix1 = {
		0.299f, 0.587f, 0.184f, 0.0f,
		0.299f, 0.587f, 0.184f, 0.0f,
		0.299f, 0.587f, 0.184f, 0.0f,
		0.0f,   0.0f,   0.0f,   1.0f
	};
	m_matrices.push_back(matrix1);

	//老照片
	Matrix44 matrix2 = {
		0.299f, 0.587f, 0.184f, 0.3137f,
		0.299f, 0.587f, 0.184f, 0.1686f,
		0.299f, 0.587f, 0.184f,-0.0901f,
		0.0f,   0.0f,   0.0f,   1.0f
	};
	m_matrices.push_back(matrix2);

	//反相
	Matrix44 matrix3 = {
	   -1.0f, 0.0f, 0.0f, 1.0f,
		0.0f,-1.0f, 0.0f, 1.0f,
		0.0f, 0.0f,-1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	m_matrices.push_back(matrix3);

	//灼伤（偏红）
	Matrix44 matrix4 = {
		1.0f,  0.0f, 0.0f, 0.0f,
		0.0f,  0.6f, 0.0f, 0.0f,
		0.0f,  0.0f, 0.6f, 0.0f,
		1.0f,  0.0f, 0.0f, 1.0f
	};
	m_matrices.push_back(matrix4);

	//中毒（偏绿）
	Matrix44 matrix5 = {
		0.6f,  0.0f, 0.0f, 0.0f,
		0.0f,  1.0f, 0.0f, 0.0f,
		0.0f,  0.0f, 0.6f, 0.0f,
		0.0f,  1.0f, 0.0f, 1.0f
	};
	m_matrices.push_back(matrix5);

	//寒冷（偏蓝）
	Matrix44 matrix6 = {
		0.6f,  0.0f, 0.0f, 0.0f,
		0.0f,  0.6f, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 1.0f
	};
	m_matrices.push_back(matrix6);
}

int FragmentEffect::addEffectNode(CCNode* pNode)
{
	do
	{
		CCGLProgram* program = new CCGLProgram();

		program->initWithVertexShaderByteArray(ccPositionTextureColor_vert, //顶点找色器,这里是引擎自带的
											   szEffectFragSource);            //像素找色器,这里是自己写的

		//将生成的shader程序传给CCSprite类
		pNode->setShaderProgram(program);
		
		//要release一次,在上面的setShaderProgram将pProgram托管给CCSprite
		//setShaderProgram只调用了pProgram->retain(),没有release(),现在pProgram的m_uReference是2
		program->release();
		
		CHECK_GL_ERROR_DEBUG();
		
		//启用顶点着色器的attribute变量，坐标、纹理坐标、颜色 ,去掉其中一个或两个还能工作
		program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		program->addAttribute(kCCAttributeNameColor,    kCCVertexAttrib_Color);
		program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		
		CHECK_GL_ERROR_DEBUG();

		//自定义着色器链接
		program->link();
		CHECK_GL_ERROR_DEBUG();

		// 设置移动、缩放、旋转矩阵  
		program->updateUniforms();
		CHECK_GL_ERROR_DEBUG();

		EffectListNode listNode;
		listNode._pNode         = pNode;
		listNode._effectSelect  = 0;
		listNode._uniformMatrix = program->getUniformLocationForName("matrixEffect");

		program->use();
		program->setUniformLocationWithMatrix4fv(listNode._uniformMatrix,
												 m_matrices[0]._matrix,
												 1);
		m_nodeList.push_back(listNode);

		return m_nodeList.size()-1;

	} while (0);
	
	return -1;
}

bool FragmentEffect::setEffect(unsigned int nodeIndex,unsigned int sel)
{
	if(nodeIndex<0 || nodeIndex>=m_nodeList.size() || sel<0 || sel>=m_matrices.size())
		return false;

	CCGLProgram* program = m_nodeList[nodeIndex]._pNode->getShaderProgram();

	program->use();
	program->setUniformLocationWithMatrix4fv(m_nodeList[nodeIndex]._uniformMatrix,
											 m_matrices[sel]._matrix,
											 1);

	m_nodeList[nodeIndex]._effectSelect = sel;

	return true;
}

bool FragmentEffect::setEffect(CCNode* pNode,unsigned int sel)
{
	if(sel<0 || sel>=m_matrices.size())
		return false;

	std::vector<EffectListNode>::iterator i   = m_nodeList.begin();
	std::vector<EffectListNode>::iterator end = m_nodeList.end();

	while (i != end)
	{
		if(i->_pNode == pNode)
		{
			CCGLProgram* program = i->_pNode->getShaderProgram();

			program->use();
			program->setUniformLocationWithMatrix4fv(i->_uniformMatrix,
													 m_matrices[sel]._matrix,
													 1);

			i->_effectSelect = sel;

			return true;
		}

		i++;
	}

	return false;
}

int FragmentEffect::getEffectSelect(unsigned int nodeIndex)
{
	if(nodeIndex<0 || nodeIndex>=m_nodeList.size())
		return -1;
	else
		return m_nodeList[nodeIndex]._effectSelect;
}

int FragmentEffect::getEffectSelect(CCNode* pNode)
{
	std::vector<EffectListNode>::iterator i   = m_nodeList.begin();
	std::vector<EffectListNode>::iterator end = m_nodeList.end();

	while (i != end)
	{
		if(i->_pNode == pNode)
			return i->_effectSelect;

		i++;
	}

	return -1;
}