#include "StdAfx.h"
#include "GLShader.h"
#include <fstream>
#include <sstream>
#include <sys/stat.h>

GLShader::GLShader(void)
{
	m_bLinked = false;
	m_hProgram = 0;
	m_sLog = "";
}


GLShader::~GLShader(void)
{
	if( m_hProgram > 0)
		glDeleteProgram(m_hProgram);
}

bool GLShader::Load(	const char * path0, GLSLShader::GLSLShaderType type0, 
						const char * path1, GLSLShader::GLSLShaderType type1	)
{
	if( ! CompileShaderFromFile(path0,type0) )
	{
		printf("Vertex shader failed to compile!\n%s", GetLog().c_str());
		getchar();
		getchar();
		exit(1);
	}
	if( ! CompileShaderFromFile(path1,type1) )
	{
		printf("Fragment shader failed to compile!\n%s",GetLog().c_str());
		getchar();
		getchar();
		exit(1);
	}
	if( !Link() )
	{
		printf("Shader program failed to link!\n%s",GetLog().c_str());
		getchar();
		getchar();
		exit(1);
	}
	return true;
}

bool GLShader::Load(	const char * path0, GLSLShader::GLSLShaderType type0, 
						const char * path1, GLSLShader::GLSLShaderType type1, 
						const char * path2, GLSLShader::GLSLShaderType type2	)
{
	if( ! CompileShaderFromFile(path0,type0) )
	{
		printf("Vertex shader failed to compile!\n%s", GetLog().c_str());
		getchar();
		exit(1);
	}
	if( ! CompileShaderFromFile(path1,type1) )
	{
		printf("Geometry shader failed to compile!\n%s",GetLog().c_str());
		getchar();
		exit(1);
	}
	if( ! CompileShaderFromFile(path2,type2) )
	{
		printf("Fragment shader failed to compile!\n%s",GetLog().c_str());
		getchar();
		exit(1);
	}
	if( !Link() )
	{
		printf("Shader program failed to link!\n%s",GetLog().c_str());
		getchar();
		exit(1);
	}
	return true;
}

bool GLShader::CompileShaderFromString( const std::string & source, GLSLShader::GLSLShaderType type )
{
	if( m_hProgram <= 0 ) {
		m_hProgram = glCreateProgram();
		if( m_hProgram == 0) {
			m_sLog = "Unable to create shader program.";
			return false;
		}
	}
	GLuint shaderHandle = 0;

	switch( type ) {
	case GLSLShader::VERTEX:
		shaderHandle = glCreateShader(GL_VERTEX_SHADER);
		break;
	case GLSLShader::FRAGMENT:
		shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case GLSLShader::GEOMETRY:
		shaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case GLSLShader::TESS_CONTROL:
		shaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
		break;
	case GLSLShader::TESS_EVALUATION:
		shaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
		break;
	default:
		return false;
	}

	const char * c_code = source.c_str();
	glShaderSource( shaderHandle, 1, &c_code, NULL );

	// Compile the shader
	glCompileShader(shaderHandle );

	// Check for errors
	int result;
	glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &result );
	if( GL_FALSE == result ) {
		// Compile failed, store log and return false
		int length = 0;
		m_sLog = "";
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length );
		if( length > 0 ) {
			char * c_log = new char[length];
			int written = 0;
			glGetShaderInfoLog(shaderHandle, length, &written, c_log);
			m_sLog = c_log;
			delete [] c_log;
		}

		return false;
	} else {
		// Compile succeeded, attach shader and return true
		glAttachShader(m_hProgram, shaderHandle);
		return true;
	}
}



bool GLShader::CompileShaderFromFile( const char * fileName, GLSLShader::GLSLShaderType type )
{
	if( ! FileExists(fileName) )
	{
		m_sLog = "File not found.";
		return false;
	}

	if( m_hProgram <= 0 ) {
		m_hProgram = glCreateProgram();
		if( m_hProgram == 0) {
			m_sLog = "Unable to create shader program.";
			return false;
		}
	}

	std::ifstream inFile( fileName, std::ios::in );
	if( !inFile ) {
		return false;
	}

	std::ostringstream code;
	while( inFile.good() ) {
		int c = inFile.get();
		if( ! inFile.eof() )
			code << (char) c;
	}
	inFile.close();

	return CompileShaderFromString(code.str(), type);
}

bool GLShader::Link()
{
	if( m_bLinked ) return true;
	if( m_hProgram <= 0 ) return false;

	glLinkProgram(m_hProgram);

	int status = 0;
	glGetProgramiv( m_hProgram, GL_LINK_STATUS, &status);
	if( GL_FALSE == status ) {
		// Store log and return false
		int length = 0;
		m_sLog = "";

		glGetProgramiv(m_hProgram, GL_INFO_LOG_LENGTH, &length );

		if( length > 0 ) {
			char * c_log = new char[length];
			int written = 0;
			glGetProgramInfoLog(m_hProgram, length, &written, c_log);
			m_sLog = c_log;
			delete [] c_log;
		}

		return false;
	} else {
		m_bLinked = true;
		return m_bLinked;
	}
}

void GLShader::Use()
{
	if( m_hProgram <= 0 || (! m_bLinked) ) return;
	glUseProgram( m_hProgram );
}

std::string GLShader::GetLog()
{
	return m_sLog;
}

int GLShader::GetHandle()
{
	return m_hProgram;
}

bool GLShader::IsLinked()
{
	return m_bLinked;
}

void GLShader::BindAttribLocation( GLuint location, const char * name)
{
	glBindAttribLocation(m_hProgram, location, name);
}

void GLShader::BindFragDataLocation( GLuint location, const char * name )
{
	glBindFragDataLocation(m_hProgram, location, name);
}

void GLShader::SetUniform( const char *name, float x, float y, float z)
{
	int loc = GetUniformLocation(name);
	if( loc >= 0 ) {
		glUniform3f(loc,x,y,z);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLShader::SetUniform( const char *name, const glm::vec3 & v)
{
	this->SetUniform(name,v.x,v.y,v.z);
}

void GLShader::SetUniform( const char *name, const glm::vec4 & v)
{
	int loc = GetUniformLocation(name);
	if( loc >= 0 ) {
		glUniform4f(loc,v.x,v.y,v.z,v.w);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLShader::SetUniform( const char *name, const glm::vec2 & v)
{
	int loc = GetUniformLocation(name);
	if( loc >= 0 ) {
		glUniform2f(loc,v.x,v.y);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLShader::SetUniform( const char *name, const glm::mat4 & m)
{
	int loc = GetUniformLocation(name);
	if( loc >= 0 )
	{
		glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLShader::SetUniform( const char *name, const glm::mat3 & m)
{
	int loc = GetUniformLocation(name);
	if( loc >= 0 )
	{
		glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLShader::SetUniform( const char *name, float val )
{
	int loc = GetUniformLocation(name);
	if( loc >= 0 )
	{
		glUniform1f(loc, val);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLShader::SetUniform( const char *name, int val )
{
	int loc = GetUniformLocation(name);
	if( loc >= 0 )
	{
		glUniform1i(loc, val);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLShader::SetUniform( const char *name, bool val )
{
	int loc = GetUniformLocation(name);
	if( loc >= 0 )
	{
		glUniform1i(loc, val);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

int GLShader::GetUniformLocation(const char * name )
{
	return glGetUniformLocation(m_hProgram, name);
}

bool GLShader::FileExists( const std::string & fileName )
{
	struct stat info;
	int ret = -1;

	ret = stat(fileName.c_str(), &info);
	return 0 == ret;
}
