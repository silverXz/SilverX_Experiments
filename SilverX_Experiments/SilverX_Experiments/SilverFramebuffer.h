#pragma once

#include <glew.h>
#include <vector>

class SilverFramebuffer
{
public:
	SilverFramebuffer();
	~SilverFramebuffer();

	

private:
	GLuint m_hHandle;
	std::vector<GLuint> m_hArray;
};