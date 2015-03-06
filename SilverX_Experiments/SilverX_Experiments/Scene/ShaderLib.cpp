#include "stdafx.h"
#include "ShaderLib.h"

#include "glslprogram.h"

ShaderLib::ShaderLib()
{
	for(int i = 0 ; i < E_SHADER_NUMBER ; i++)
		m_pShaderLib[i] = NULL;
}

ShaderLib::~ShaderLib()
{
	for (int i = 0 ; i < E_SHADER_NUMBER ; i++)
	{
		if(m_pShaderLib[i])
			delete m_pShaderLib[i],m_pShaderLib[i] = NULL;
	}
}

void ShaderLib::CompileAndLinkAllShaderPrograms()
{
	m_pShaderLib[E_GENERAL_SHADER] = new GLSLProgram;

	//The very original shader we use! Very messy. Don't like it.
	if( ! m_pShaderLib[E_GENERAL_SHADER]->compileShaderFromFile("Shaders/vertex.vert",GLSLShader::VERTEX) )
	{
		printf("Vertex shader failed to compile!\n%s",
			m_pShaderLib[E_GENERAL_SHADER]->log().c_str());
		exit(1);
	}
	if( ! m_pShaderLib[E_GENERAL_SHADER]->compileShaderFromFile("Shaders/fragment.frag",GLSLShader::FRAGMENT))
	{
		printf("Fragment shader failed to compile!\n%s",
			m_pShaderLib[E_GENERAL_SHADER]->log().c_str());
		exit(1);
	}
	if( ! m_pShaderLib[E_GENERAL_SHADER]->link() )
	{
		printf("Shader program failed to link!\n%s",
			m_pShaderLib[E_GENERAL_SHADER]->log().c_str());
		exit(1);
	}
}

GLSLProgram* ShaderLib::GetShaderProgram(E_SHADER_TYPE effect) const
{
	return m_pShaderLib[effect];
}