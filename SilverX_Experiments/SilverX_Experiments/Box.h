#pragma once
#include <glew.h>

class Box
{
public:
	Box();
	~Box();

	void Init();
	void Render();

private:
	GLuint hVAO;
	GLuint hVBO[2];
};