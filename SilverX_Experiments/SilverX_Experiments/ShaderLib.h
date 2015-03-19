#pragma once

class GLShader;

enum E_SHADER_TYPE {
	E_GENERAL_SHADER = 0,
	E_MRT_SHADER,
	E_QUAD_SHADER,
	E_SSAOP1_SHADER,
	E_SSAOP2_SHADER,
	E_SSAOP3_SHADER,
	E_SSAOP4_SHADER,
	E_SHADER_NUMBER
};

class ShaderLib
{
private:
	GLShader* m_pShaderLib[E_SHADER_NUMBER];

public:
	ShaderLib();
	~ShaderLib();

	void CompileAndLinkAllShaderPrograms(void);
	GLShader* GetShaderProgram(E_SHADER_TYPE effect) const;
};