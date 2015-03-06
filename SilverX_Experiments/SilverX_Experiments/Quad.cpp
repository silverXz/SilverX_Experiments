#include "stdafx.h"
#include "Quad.h"


Quad::Quad()
{
	GLfloat ver[] = {
		-1.0f, -1.0f, 1.0f, //0
		1.0f, -1.0f, 1.0f,  //1
		-1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f,1.0f,1.0f
	};
	GLfloat colors[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

	};
	GLfloat coords[] = {
		0.0f,0.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		1.0f,1.0f
	};

	GLfloat verts[] = {
		-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
	};
	GLfloat tc[] = {
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

	

	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
	glVertexAttribPointer((GLuint)0,3,GL_FLOAT,FALSE,0,((GLubyte*)NULL + 0));

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
	glVertexAttribPointer( (GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );

	glBindVertexArray(0);
}

Quad::~Quad()
{
	if(vao > 0)
		glDeleteBuffers(1,&vao);
	for(int i = 0 ; i < 4 ; i++)
	{
		if(vbo[i] > 0)
			glDeleteBuffers(1,&vbo[i]);
	}
}

void Quad::render()
{

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,6);
}