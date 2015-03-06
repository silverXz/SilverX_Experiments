#pragma once

#include <gl/glew.h>

class Quad
{
private:
	GLuint vao;
	GLuint vbo[2];
public:
	Quad();
	~Quad();

	void render();
};