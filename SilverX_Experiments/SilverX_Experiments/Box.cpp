#include "stdafx.h"
#include "Box.h"



Box::Box()
{
	hVAO = 0;
	hVBO[0] = 0;
	hVBO[1] = 0;
	Init();
}

Box::~Box()
{
	if( hVBO > 0 )
		glDeleteVertexArrays(1,&hVAO);
	for( int i = 0 ; i < 2 ; ++i )
	{
		if( hVBO[i] > 0 )
			glDeleteBuffers(1,&hVBO[i]);
	}
}

void Box::Init()
{
	glGenVertexArrays(1,&hVAO);
	glBindVertexArray(hVAO);

	glGenBuffers(2,hVBO);

	
	const float edge = 500.0f;
	float vertices[] = {
		-edge,-edge,edge,   /* 0 */
		edge,-edge,edge,	/* 1 */
		edge,edge,edge,		/* 6 */    

		-edge,-edge,edge,   /* 0 */
		edge,edge,edge,		/* 6 */
		-edge,edge,edge,	/* 5 */		//front

		edge,-edge,edge,	/* 1 */
		edge,-edge,-edge,	/* 2 */
		edge,edge,-edge,	/* 7 */

		edge,-edge,edge,	/* 1 */
		edge,edge,-edge,	/* 7 */
		edge,edge,edge,		/* 6 */		//right

		-edge,edge,edge,	/* 5 */
		edge,edge,edge,		/* 6 */
		edge,edge,-edge,	/* 7 */

		-edge,edge,edge,	/* 5 */
		edge,edge,-edge,	/* 7 */
		-edge,edge,-edge,	/* 4 */			//top

		-edge,-edge,-edge,	/* 3 */
		-edge,-edge,edge,   /* 0 */
		-edge,edge,edge,	/* 5 */

		-edge,-edge,-edge,	/* 3 */
		-edge,edge,edge,	/* 5 */
		-edge,edge,-edge,	/* 4 */			//left

		edge,-edge,-edge,	/* 2 */
		-edge,-edge,-edge,	/* 3 */
		-edge,edge,-edge,	/* 4 */	

		edge,-edge,-edge,	/* 2 */
		-edge,edge,-edge,	/* 4 */
		edge,edge,-edge,	/* 7 */			//back

		edge,-edge,-edge,	/* 2 */
		edge,-edge,edge,	/* 1 */
		-edge,-edge,edge,   /* 0 */

		-edge,-edge,edge,   /* 0 */
		-edge,-edge,-edge,	/* 3 */
		edge,-edge,-edge	/* 2 */			//bottom
	};

	/*
			4 ----  7
		   /       /|
		  /       / |
		 5-------6  2
		 |		 |  /
		 |       | /
		 |       |/
		 0-------1
	 */

	float normals[] = {
		// Front
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,

		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,

		// Right
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,

		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,

		// TOp
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,

		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,

		// Left
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,

		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,

		// Back
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,

		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,

		//Bottom
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,

		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER,hVBO[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

	glBindBuffer(GL_ARRAY_BUFFER,hVBO[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(normals),normals,GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

	glBindVertexArray(0);
}

void Box::Render()
{
	glBindVertexArray(hVAO);
	glDrawArrays(GL_TRIANGLES,0,36);
}


