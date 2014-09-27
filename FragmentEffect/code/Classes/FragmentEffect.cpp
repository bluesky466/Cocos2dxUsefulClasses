#include "FragmentEffect.h"

GLfloat transformMatrix[][16] = 
{
	{
		1.0, 0.0, 0.0, 0.0,						
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	},

	{
		0.299, 0.587, 0.184, 0.0,
		0.299, 0.587, 0.184, 0.0,
		0.299, 0.587, 0.184, 0.0,
		0.0,   0.0,   0.0,   1.0
	},

	{
		0.299, 0.587, 0.184, 0.3137,
		0.299, 0.587, 0.184, 0.1686,
		0.299, 0.587, 0.184,-0.0901,
		0.0,   0.0,   0.0,   1.0
	},

	{
		-1.0, 0.0, 0.0, 1.0,
		0.0,-1.0, 0.0, 1.0,
		0.0, 0.0,-1.0, 1.0,
		0.0, 0.0, 0.0, 1.0
	},

	{
		1.0,  0.0, 0.0, 0.0,
		0.0,  0.6, 0.0, 0.0,
		0.0,  0.0, 0.6, 0.0,
		1.0,  0.0, 0.0, 1.0
	},

	{
		0.6, 0.0, 0.0, 0.0,
		0.0,  1.0, 0.0, 0.0,
		0.0,  0.0, 0.6, 0.0,
		0.0,  1.0, 0.0, 1.0
	},

	{
		0.6,  0.0, 0.0, 0.0,
		0.0,  0.6, 0.0, 0.0,
		0.0,  0.0, 1.0, 0.0,
		0.0,  0.0, 1.0, 1.0
	}
};

bool FragmentEffect::setShaderProgram(CCNode* pNode)
{
	do
	{
		m_program = new CCGLProgram();
		GLchar* pszFragSource =
				"#ifdef GL_ES													  \n \
				precision mediump float;										  \n \
				#endif															  \n \
				uniform sampler2D u_texture;									  \n \
				varying vec2 v_texCoord;										  \n \
				varying vec4 v_fragmentColor;									  \n \
				uniform mat4 matrixEffect;										  \n \
				void main(void)													  \n \
				{																  \n \
					gl_FragColor = texture2D(u_texture, v_texCoord)*matrixEffect; \n \
				}";

		m_program->initWithVertexShaderByteArray(ccPositionTextureColor_vert, //顶点找色器,这里是引擎自带的
												 pszFragSource);              //像素找色器,这里是自己写的

		//将生成的shader程序传给CCSprite类
		pNode->setShaderProgram(m_program);
		
		//要release一次,在上面的setShaderProgram将pProgram托管给CCSprite
		//setShaderProgram只调用了pProgram->retain(),没有release(),现在pProgram的m_uReference是2
		m_program->release();
		
		CHECK_GL_ERROR_DEBUG();
		
		//启用顶点着色器的attribute变量，坐标、纹理坐标、颜色 ,去掉其中一个或两个还能工作
		m_program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		m_program->addAttribute(kCCAttributeNameColor,    kCCVertexAttrib_Color);
		m_program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		
		CHECK_GL_ERROR_DEBUG();

		//自定义着色器链接
		m_program->link();
		CHECK_GL_ERROR_DEBUG();

		// 设置移动、缩放、旋转矩阵  
		m_program->updateUniforms();
		CHECK_GL_ERROR_DEBUG();

		m_program->use();
		m_matrixEffect =  m_program->getUniformLocationForName("matrixEffect");
		m_program->setUniformLocationWithMatrix4fv(m_matrixEffect,transformMatrix[0],1);

		return true;

	} while (0);
	
	return false;
}

///选择效果
void FragmentEffect::setEffect(int sel)
{
	m_program->use();
	m_program->setUniformLocationWithMatrix4fv(m_matrixEffect,transformMatrix[sel],1);
}

///选择效果
void FragmentEffect::setEffect(EffectSelect sel)
{
	m_program->use();
	m_program->setUniformLocationWithMatrix4fv(m_matrixEffect,transformMatrix[sel],1);
}