#include "stdafx.h"
#include "ShaderLib.h"

#include "GLShader.h"

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
	m_pShaderLib[E_GENERAL_SHADER] = new GLShader;

	m_pShaderLib[E_GENERAL_SHADER]->Load( "shader/vertex.vert",		GLSLShader::VERTEX,
		"shader/fragment.frag",	GLSLShader::FRAGMENT );

	m_pShaderLib[E_MRT_SHADER] = new GLShader;

	m_pShaderLib[E_MRT_SHADER]->Load( "shader/MRT_vertex.vert",		GLSLShader::VERTEX,
		"shader/MRT_fragment.frag",	GLSLShader::FRAGMENT );

	m_pShaderLib[E_QUAD_SHADER] = new GLShader;

	m_pShaderLib[E_QUAD_SHADER]->Load( "shader/quad.vert",		GLSLShader::VERTEX,
		"shader/quad.frag",	GLSLShader::FRAGMENT );

	m_pShaderLib[E_SSAOP1_SHADER] = new GLShader;

	m_pShaderLib[E_SSAOP1_SHADER]->Load( "shader/SSAO_p1.vert",		GLSLShader::VERTEX,
		"shader/SSAO_p1.frag",	GLSLShader::FRAGMENT );

	m_pShaderLib[E_SSAOP2_SHADER] = new GLShader;

	m_pShaderLib[E_SSAOP2_SHADER]->Load( "shader/SSAO_p2.vert",		GLSLShader::VERTEX,
		"shader/SSAO_p2.frag",	GLSLShader::FRAGMENT );

	m_pShaderLib[E_SSAOP3_SHADER] = new GLShader;

	m_pShaderLib[E_SSAOP3_SHADER]->Load( "shader/SSAO_p3.vert",		GLSLShader::VERTEX,
		"shader/SSAO_p3.frag",	GLSLShader::FRAGMENT );

	m_pShaderLib[E_SSAOP4_SHADER] = new GLShader;

	m_pShaderLib[E_SSAOP4_SHADER]->Load( "shader/SSAO_p4.vert",		GLSLShader::VERTEX,
		"shader/SSAO_p4.frag",	GLSLShader::FRAGMENT );
}

GLShader* ShaderLib::GetShaderProgram(E_SHADER_TYPE effect) const
{
	return m_pShaderLib[effect];
}