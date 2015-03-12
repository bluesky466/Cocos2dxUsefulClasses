#include "FragmentEffect.h"

USING_NS_CC;
using namespace cocostudio;

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
	float f0[] = {
		1.0f, 0.0f, 0.0f, 0.0f,						
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	m_matrices.push_back(Mat4(f0));

	//黑白化
	float f1[] = {
		0.299f, 0.587f, 0.184f, 0.0f,
		0.299f, 0.587f, 0.184f, 0.0f,
		0.299f, 0.587f, 0.184f, 0.0f,
		0.0f,   0.0f,   0.0f,   1.0f
	};
	m_matrices.push_back(Mat4(f1));

	//老照片
	float f2[] = {
		0.299f, 0.587f, 0.184f, 0.3137f,
		0.299f, 0.587f, 0.184f, 0.1686f,
		0.299f, 0.587f, 0.184f,-0.0901f,
		0.0f,   0.0f,   0.0f,   1.0f
	};
	m_matrices.push_back(Mat4(f2));

	//反相
	float f3[] = {
	   -1.0f, 0.0f, 0.0f, 1.0f,
		0.0f,-1.0f, 0.0f, 1.0f,
		0.0f, 0.0f,-1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	m_matrices.push_back(Mat4(f3));

	//灼伤（偏红）
	float f4[] = {
		1.0f,  0.0f, 0.0f, 0.0f,
		0.0f,  0.6f, 0.0f, 0.0f,
		0.0f,  0.0f, 0.6f, 0.0f,
		1.0f,  0.0f, 0.0f, 1.0f
	};
	m_matrices.push_back(Mat4(f4));

	//中毒（偏绿）
	float f5[] = {
		0.6f,  0.0f, 0.0f, 0.0f,
		0.0f,  1.0f, 0.0f, 0.0f,
		0.0f,  0.0f, 0.6f, 0.0f,
		0.0f,  1.0f, 0.0f, 1.0f
	};
	m_matrices.push_back(Mat4(f5));

	//寒冷（偏蓝）
	float f6[] = {
		0.6f,  0.0f, 0.0f, 0.0f,
		0.0f,  0.6f, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 1.0f
	};
	m_matrices.push_back(Mat4(f6));
}

int FragmentEffect::addEffectNode(Sprite* sprite)
{
	do
	{
		GLProgram* program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert,szEffectFragSource);
		sprite->setGLProgram(program);
		sprite->setGLProgramState(GLProgramState::create(program));
		sprite->getGLProgramState()->setUniformMat4(EFFECT_MATRIX_NAME,m_matrices[ES_NONE]);

		EffectListNode listNode;
		listNode._pNode         = sprite;
		listNode._type			= NT_SPRITE;
		listNode._effectSelect  = 0;
		m_nodeList.push_back(listNode);

		return m_nodeList.size()-1;

	} while (0);
	
	return -1;
}

int FragmentEffect::addEffectNode(cocostudio::Armature* armature)
{
	do
	{
		Vector<Node*> children = armature->getChildren();

		for (auto& object : children)
		{
			if (Bone *bone = dynamic_cast<Bone*>(object))
			{
				Node *node = bone->getDisplayRenderNode();
            
				if (nullptr == node)
					continue;
            
				GLProgram* program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert,szEffectFragSource);

				Skin *skin = static_cast<Skin *>(node);
				skin->setGLProgram(program);
				skin->setGLProgramState(GLProgramState::create(program));
				skin->getGLProgramState()->setUniformMat4(EFFECT_MATRIX_NAME,m_matrices[ES_NONE]);
			}
		}

		EffectListNode listNode;
		listNode._pNode         = armature;
		listNode._type			= NT_ARMATURE;
		listNode._effectSelect  = 0;
		m_nodeList.push_back(listNode);

		return m_nodeList.size()-1;

	} while (0);
	
	return -1;
}

void FragmentEffect::selectEffect(cocos2d::Node* pNode, NodeType type, unsigned int sel)
{
	if(type == NT_SPRITE)
	{
		pNode->getGLProgramState()->setUniformMat4(EFFECT_MATRIX_NAME,m_matrices[sel]);
	}
	else if(type == NT_ARMATURE)
	{
		Vector<Node*> children = pNode->getChildren();

		for (auto& object : children)
		{
			if (Bone *bone = dynamic_cast<Bone*>(object))
			{
				Node *node = bone->getDisplayRenderNode();
            
				if (nullptr == node)
					continue;

				Skin *skin = static_cast<Skin *>(node);
				skin->getGLProgramState()->setUniformMat4(EFFECT_MATRIX_NAME,m_matrices[sel]);
			}
		}
	}
}

bool FragmentEffect::setEffect(unsigned int nodeIndex,unsigned int sel)
{
	if(nodeIndex<0 || nodeIndex>=m_nodeList.size() || sel<0 || sel>=m_matrices.size())
		return false;

	selectEffect(m_nodeList[nodeIndex]._pNode,m_nodeList[nodeIndex]._type,sel);
	m_nodeList[nodeIndex]._effectSelect = sel;

	return true;
}

bool FragmentEffect::setEffect(Node* pNode,unsigned int sel)
{
	if(sel<0 || sel>=m_matrices.size())
		return false;

	std::vector<EffectListNode>::iterator i   = m_nodeList.begin();
	std::vector<EffectListNode>::iterator end = m_nodeList.end();

	while (i != end)
	{
		if(i->_pNode == pNode)
		{
			selectEffect(i->_pNode,i->_type,sel);
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

int FragmentEffect::getEffectSelect(Node* pNode)
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